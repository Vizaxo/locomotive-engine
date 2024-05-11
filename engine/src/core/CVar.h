#pragma once

#define CVAR_NAME(name) CVar_##name
#define DEF_CVAR(name, desc, type, init) static inline CVar<type> CVAR_NAME(name) = { TOSTRING2(name), desc, init };
#define CVAR(name) CVAR_NAME(name).val

template <typename T>
struct CVar {
	const char* name;
	const char* desc;
	T val;
};

DEF_CVAR(test_cvar, "test", float, 3.5)
