#include "PCH.h"

#include "RHICommon.h"
#include "core/Assert.h"

u32 RHICommon::stride(RHICommon::PixelFormat pf) {
	switch (pf) {
	case R8G8B8A8:		return 4;
	case R32:			return 4;
	case R32G32:		return 8;
	case R32G32B32:		return 12;
	case R32G32B32A32:	return 16;
	default: ASSERT(false, "No stride found for pixel format %d", pf);
	}
}
