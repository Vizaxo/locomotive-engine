#include "PCH.h"

#include "OpenVRModule.h"

void vrTest() {
	vr::EVRInitError error = vr::VRInitError_None;
	vr::IVRSystem* vr = vr::VR_Init(&error, vr::VRApplication_Scene);
}