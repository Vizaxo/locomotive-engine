#include "PCH.h"

#include "OpenVRModule.h"

#include "Application.h"
#include "EngineMain.h"
#include "core/Log.h"
#include "types/Either.h"
#include "types/Pointers.h"
#include "types/Types.h"

namespace VR {

const bool enableVR = false;

Log::Channel vrChan = { "vr" };

std::string VRModule::getTrackedDeviceString(vr::TrackedDeviceIndex_t device, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *error)
{
	u32 bufLen = vr->GetStringTrackedDeviceProperty( device, prop, nullptr, 0, error);
	if (bufLen == 0)
		return "";

	char* buf = new char[bufLen];
	bufLen = vr->GetStringTrackedDeviceProperty( device, prop, buf, bufLen, error);
	std::string ret = buf;
	delete [] buf;
	return ret;
}

enum InitError : u8 {VRDisabled, InitFailed, RenderModelInterfaceFalied, InitCompositorFailed};
Either<OwningPtr<VRModule>, InitError> loadVR(RefPtr<RHI> rhi) {
	if (!enableVR) {
		LOG(Log::INFO, vrChan, "VR not enabled. Aborting loadVR().");
		return VRDisabled;
	}
	vr::EVRInitError error = vr::VRInitError_None;
	OwningPtr<vr::IVRSystem, true, VRShutdown> vr = vr::VR_Init(&error, vr::VRApplication_Scene);

	if (error != vr::VRInitError_None) {
		LOG(Log::WARN, vrChan, "Could not init VR");
		return InitFailed;
	}

	vr::IVRRenderModels* renderModels = (vr::IVRRenderModels*)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &error);
	if (!renderModels) {
		LOG(Log::WARN, vrChan, "Could not get render model interface");
		return RenderModelInterfaceFalied;
	}

	VRModule* vrModule = new VRModule{ vr.getNonNull() };

	std::string driver = vrModule->getTrackedDeviceString(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String);
	std::string serialNumber = vrModule->getTrackedDeviceString(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String);

	vr::IVRCompositor* compositor = vr::VRCompositor();
	if (!compositor) {
		LOG(Log::WARN, vrChan, "Could not initialise compositor");
		return InitCompositorFailed;
	}

	for (u32 device = 0; device < vr::k_unMaxTrackedDeviceCount; ++device) {
		if (vrModule->vr->IsTrackedDeviceConnected(device))
			vrModule->setupTrackedDevice(rhi, device);
	}

	return {std::move(vrModule)};
}

void VRModule::setupTrackedDevice(RefPtr<RHI> rhi, vr::TrackedDeviceIndex_t device) {
	if (device >= vr::k_unMaxTrackedDeviceCount)
		return;

	std::string renderModelName = getTrackedDeviceString(device, vr::Prop_RenderModelName_String);

	if (RefPtr<Mesh, true> renderMesh = Mesh::meshManager.get(sID(renderModelName.c_str()))) {
		deviceMeshes[device] = renderMesh;
	} else {
		vr::EVRRenderModelError error;
		vr::RenderModel_t* renderModel;
		while (true) {
			error = vr::VRRenderModels()->LoadRenderModel_Async(renderModelName.c_str(), &renderModel);
			if (error != vr::VRRenderModelError_Loading)
				break;
		}

		struct VRVert {
			v3f pos;
			v3f normal;
			v2f uv;
			static VertSemantic getSemantic(u8 index) {
				switch (index) {
				case 0: return {VertType::POSITION, 0, RHICommon::R32G32B32};
				case 1: return {VertType::NORMAL, 0, RHICommon::R32G32B32};
				case 2: return {VertType::TEXCOORD, 0, RHICommon::R32G32};
				default: return {VertType::NONE};
				}
			}
		};
		RefPtr<Mesh, true> mesh = Mesh::createMesh<VRVert, u16>(rhi, sID(renderModelName.c_str()),
			(const VRVert*)renderModel->rVertexData, renderModel->unVertexCount,
			renderModel->rIndexData, renderModel->unTriangleCount*3);
		if (mesh)
			deviceMeshes[device] = mesh;
		else
			LOG(Log::FATAL, vrChan, "Could not load mesh for device %d", device);
	}
}

void VRModule::teardownTrackedDevice(RefPtr<RHI> rhi, vr::TrackedDeviceIndex_t device) {
	deviceMeshes[device] = nullptr;
}

void VRModule::tick(RefPtr<RHI> rhi) {
	processEvents(rhi);
}

void VRModule::processEvents(RefPtr<RHI> rhi) {
	vr::VREvent_t event;
	while (vr->PollNextEvent(&event, sizeof(event))) {
		switch (event.eventType) {
		case vr::VREvent_TrackedDeviceActivated:
			LOG(Log::INFO, vrChan, "Tracked device %d activated", event.trackedDeviceIndex);
			setupTrackedDevice(rhi, event.trackedDeviceIndex);
			break;
		case vr::VREvent_TrackedDeviceDeactivated:
			LOG(Log::INFO, vrChan, "Tracked device %d deactivated", event.trackedDeviceIndex);
			teardownTrackedDevice(rhi, event.trackedDeviceIndex);
			break;
		case vr::VREvent_TrackedDeviceUpdated:
			LOG(Log::INFO, vrChan, "Tracked device %d updated", event.trackedDeviceIndex);
			teardownTrackedDevice(rhi, event.trackedDeviceIndex);
			setupTrackedDevice(rhi, event.trackedDeviceIndex);
			break;
		}
	}
}

void VRModule::render(RefPtr<Renderer> renderer) {
	for (u32 device = 0; device < vr::k_unMaxTrackedDeviceCount; ++device) {
		RefPtr<Mesh, true> mesh = deviceMeshes[device];
		if (!mesh)
			continue;

		RefPtr<Material> mat = materialManager.get(sID("SolidColourMat")).getNonNull();

		RefPtr<Scene> scene = application->getScene();
		renderer->renderMesh(mesh.getNonNull(), {}, scene, mat, 1);
	}
}

m44 VRModule::getProjectionMatrix(vr::Hmd_Eye eye, float nearClip, float farClip) {
	vr::HmdMatrix44_t mat = vr->GetProjectionMatrix(vr::Eye_Left, nearClip, farClip);
	return {
		mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
		mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
		mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
		mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3],
	};
}

}
