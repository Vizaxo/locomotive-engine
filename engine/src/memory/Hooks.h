#pragma once

#include <map>

#define ORIGINAL(fn) ((decltype(&fn))Hooks::originalFunctions[fn])

namespace Hooks {

extern std::map<void*, void*> originalFunctions;

void hookFunction(void* original, void* hook);
}
