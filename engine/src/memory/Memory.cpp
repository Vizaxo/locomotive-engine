#include "PCH.h"

#include "core/Log.h"
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

Log::Channel g_memoryChannel = {"memory"};
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

	if (debugPrintAllocations) {
		LOG(Log::INFO, g_memoryChannel, "Allocated   %lld bytes at %p", n, addr);
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

	if (debugPrintDeallocations) {
		LOG(Log::INFO, g_memoryChannel, "Deallocated %lld bytes at %p", n, p);
	}
	if (debugPrintDeallocationStacks) {
		PAL::printBacktrace(12, 2);
	}

#endif
	free(p);
}