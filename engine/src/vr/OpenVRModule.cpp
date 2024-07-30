#include "PCH.h"

#include "OpenVRModule.h"

#include "core/Log.h"
#include "types/Either.h"
#include "types/Pointers.h"
#include "types/Types.h"

namespace VR {

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

enum InitError : u8 {InitFailed, RenderModelInterfaceFalied, InitCompositorFailed};
Either<OwningPtr<VRModule>, InitError> loadVR(RefPtr<RHI> rhi) {
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
		RefPtr<Mesh, true> mesh = Mesh::createMesh<vr::RenderModel_Vertex_t, u16>(rhi, sID(renderModelName.c_str()),
			renderModel->rVertexData, renderModel->unVertexCount,
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

		static const u32 VR_RENDER_MESH_ELEMENT_COUNT = 3;
		D3D11_INPUT_ELEMENT_DESC descs[VR_RENDER_MESH_ELEMENT_COUNT];
		descs[0].SemanticName = "POSITION";
		descs[0].SemanticIndex = 0;
		descs[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		descs[0].InputSlot = 0;
		descs[0].AlignedByteOffset = 0;
		descs[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		descs[0].InstanceDataStepRate = 0;

		descs[1].SemanticName = "NORMAL";
		descs[1].SemanticIndex = 0;
		descs[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		descs[1].InputSlot = 0;
		descs[1].AlignedByteOffset = sizeof(v3f);
		descs[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		descs[1].InstanceDataStepRate = 0;

		descs[2].SemanticName = "TEXCOORD";
		descs[2].SemanticIndex = 0;
		descs[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		descs[2].InputSlot = 0;
		descs[2].AlignedByteOffset = sizeof(v3f)*2;
		descs[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		descs[2].InstanceDataStepRate = 0;

		RHI::InputLayout inputLayout = renderer->rhi->createInputLayout(descs, VR_RENDER_MESH_ELEMENT_COUNT, &mat->vertexShader);
		renderer->renderMesh(mesh.getNonNull(), mat, &inputLayout, 1);
	}
}

}
