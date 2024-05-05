#include "PCH.h"

#include "Utils.h"
#include "platform/Platform.h"
#include "EngineMain.h"

#if _DEBUG
static bool debugPrintAllocationStacks = false;
static bool debugPrintAllocations = true;
static bool debugPrintDeallocationStacks = false;
static bool debugPrintDeallocations = true;

struct AllocationInfo {
	int numAllocations, numDeallocations;
	size_t totalAllocated, totalDeallocated, currentUsage;
};

AllocationInfo allocInfos[Engine::NUM_ENGINE_STATES];
#endif

void* operator new(size_t n) {
	// Reimplement default operator new behaviour
	void* addr;
	while (true) {
		addr = malloc(n);
		if (!addr) {
			std::new_handler h = std::get_new_handler();
			if (h) {
				h();
				continue;
			} else {
				std::terminate();
			}
		}

		break;
	}

#if _DEBUG
	AllocationInfo& allocInfo = allocInfos[Engine::engineState];
	allocInfo.numAllocations++;
	allocInfo.totalAllocated += n;
	allocInfo.currentUsage += n;
	char buf[50];

	if (debugPrintAllocations) {
		snprintf(buf, 50, "Allocated   %lld bytes at %p\n", n, addr);
		DEBUG_PRINT(buf);
	}
	if (debugPrintAllocationStacks) {
		PAL::printBacktrace(12, 2);
	}

#endif

	return addr;
}

void operator delete(void* p, size_t n) {
#if _DEBUG
	AllocationInfo& allocInfo = allocInfos[Engine::engineState];
	allocInfo.numDeallocations++;
	allocInfo.totalDeallocated += n;
	allocInfo.currentUsage -= n;
	char buf[50];

	if (debugPrintDeallocations) {
		snprintf(buf, 50, "Deallocated %lld bytes at %p\n", n, p);
		DEBUG_PRINT(buf);
	}
	if (debugPrintDeallocationStacks) {
		PAL::printBacktrace(12, 2);
	}

#endif
	free(p);
}