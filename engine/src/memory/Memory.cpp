#include "PCH.h"

#include "Utils.h"
#include "platform/Platform.h"

#if _DEBUG
static bool debugPrintAllocationStacks = false;
#endif

void* operator new(size_t n) {
#if _DEBUG
	if (debugPrintAllocationStacks) {
		char buf[50];
		snprintf(buf, 50, "Allocated %lld bytes\n", n);
		PAL::printBacktrace(12, 2);
		DEBUG_PRINT(buf);
	}
#endif
	return malloc(n);
}