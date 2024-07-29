#pragma once

#include <openvr.h>

#include "types/Either.h"
#include "types/Pointers.h"

namespace VR {
enum InitError : uint8_t;

struct VRModule {
	OwningPtr<vr::IVRSystem> vr;
};

Either<OwningPtr<VRModule>, InitError> loadVR();

}
