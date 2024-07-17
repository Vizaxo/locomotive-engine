#include "PCH.h"

#include "Hooks.h"
#include "types/Types.h"

namespace Hooks {

void hookFunction(void* original, void* hook) {
	u8* orig = (u8*)original;
	DWORD old;
	// Make code page editable
	VirtualProtect(orig+1, 4, PAGE_READWRITE, &old);

	// Function pointers point to a jump table
	// The first byte is the opcode, the next 4 are the RIP-relative jump
	i32 jmpOffset = *(u32*)(orig+1);

	// Calculate the new offset (adding 5 because RIP-relative is relative to _after_ the
	// instruction has executed.
	// This points to the jump table entry, so if `hook` is hooked,
	// this will recursively apply to `original`.
	i32 newJmpOffset = (i32)hook - ((i32)original + 5);
	*(i32*)(orig+1) = newJmpOffset;

	VirtualProtect(orig+1, 4, PAGE_EXECUTE, &old);
}

}