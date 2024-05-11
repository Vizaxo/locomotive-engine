#include "PCH.h"

#include "core/Log.h"
#include "core/CVar.h"
#include "Utils.h"
#include "platform/Platform.h"
#include "EngineMain.h"
#include "Constants.h"

#if DEBUG_LOG_ALLOCATIONS
DEF_CVAR_(debug_print_allocation_stacks, bool, false)
DEF_CVAR_(debug_print_allocations, bool, true)
DEF_CVAR_(debug_print_deallocation_stacks, bool, false)
DEF_CVAR_(debug_print_deallocations, bool, false)

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

#if DEBUG_LOG_ALLOCATIONS
	AllocationInfo& allocInfo = allocInfos[Engine::engineState];
	allocInfo.numAllocations++;
	allocInfo.totalAllocated += n;
	allocInfo.currentUsage += n;

	if (CVAR(debug_print_allocations)) {
		LOG(Log::INFO, g_memoryChannel, "Allocated   %lld bytes at %p", n, addr);
	}
	if (CVAR(debug_print_allocation_stacks)) {
		PAL::printBacktrace(12, 2, g_memoryChannel);
	}

#endif

	return addr;
}

void operator delete(void* p, size_t n) {
#if DEBUG_LOG_ALLOCATIONS
	AllocationInfo& allocInfo = allocInfos[Engine::engineState];
	allocInfo.numDeallocations++;
	allocInfo.totalDeallocated += n;
	allocInfo.currentUsage -= n;

	if (CVAR(debug_print_deallocations)) {
		LOG(Log::INFO, g_memoryChannel, "Deallocated %lld bytes at %p", n, p);
	}
	if (CVAR(debug_print_deallocation_stacks)) {
		PAL::printBacktrace(12, 2, g_memoryChannel);
	}

#endif
	free(p);
}