#include "PCH.h"

#include "OpenVRModule.h"

#include "core/Log.h"
#include "types/Either.h"
#include "types/Pointers.h"
#include "types/Types.h"

namespace VR {

Log::Channel vrChan = { "vr" };

std::string getTrackedDeviceString(vr::IVRSystem *vr, vr::TrackedDeviceIndex_t device, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *error = nullptr)
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

enum InitError : u8 {InitFailed, RenderModelInterfaceFalied};
Either<OwningPtr<VRModule>, InitError> loadVR() {
	vr::EVRInitError error = vr::VRInitError_None;
	vr::IVRSystem* vr = vr::VR_Init(&error, vr::VRApplication_Scene);

	if (error != vr::VRInitError_None) {
		LOG(Log::WARN, vrChan, "Could not init VR");
		return InitFailed;
	}

	vr::IVRRenderModels* renderModels = (vr::IVRRenderModels*)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &error);
	if (!renderModels) {
		LOG(Log::WARN, vrChan, "Could not get render model interface");
		return RenderModelInterfaceFalied;
	}

	std::string driver = getTrackedDeviceString(vr, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String);
	std::string serialNumber = getTrackedDeviceString(vr, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String);

	return {new VRModule{std::move(vr)}};
}

}