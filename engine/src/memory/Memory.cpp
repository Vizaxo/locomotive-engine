#include "PCH.h"

#include "Utils.h"
#include "platform/Platform.h"

#if _DEBUG
static bool debugPrintAllocationStacks = false;
static bool debugPrintAllocations = true;
#endif

void* operator new(size_t n) {
	void* addr = malloc(n);

#if _DEBUG
	char buf[50];

	if (debugPrintAllocationStacks) {
		snprintf(buf, 50, "Allocated %lld bytes\n", n);
		PAL::printBacktrace(12, 2);
		DEBUG_PRINT(buf);
	}

	if (debugPrintAllocations) {
		snprintf(buf, 50, "Allocated %lld bytes at %p\n", n, addr);
		DEBUG_PRINT(buf);
	}
#endif

	return addr;
}