#pragma once

#include <openvr.h>

#include "renderer/Mesh.h"
#include "renderer/Renderer.h"
#include "types/Either.h"
#include "types/Pointers.h"

namespace VR {
enum InitError : uint8_t;

struct VRShutdown {
	void operator() (void** vr) {
		if (*vr) {
			vr::VR_Shutdown();
			*vr = nullptr;
		}
	}
};

struct VRModule {
	OwningPtr<vr::IVRSystem, false, VRShutdown> vr;
	RefPtr<Mesh, true> deviceMeshes[vr::k_unMaxTrackedDeviceCount] = {};

	void tick(RefPtr<RHI> rhi);
	void render(RefPtr<Renderer> renderer);
	void processEvents(RefPtr<RHI> rhi);
	void setupTrackedDevice(RefPtr<RHI> rhi, vr::TrackedDeviceIndex_t device);
	void teardownTrackedDevice(RefPtr<RHI> rhi, vr::TrackedDeviceIndex_t device);
	std::string getTrackedDeviceString(vr::TrackedDeviceIndex_t device, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError* error = nullptr);
};

Either<OwningPtr<VRModule>, InitError> loadVR(RefPtr<RHI> rhi);

}
