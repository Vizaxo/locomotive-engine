#pragma once

#include "types/Types.h"

namespace RHICommon {

enum PixelFormat {
	 R8G8B8A8,
	 R32,
	 R32G32,
	 R32G32B32,
	 R32G32B32A32,
};

u32 stride(PixelFormat pf);

}
