#pragma once

#include <string>
#include <map>
#include <optional>

#include "types/StringID.h" 
#include "types/Pointers.h" 

template<typename T>
class ResourceManager {
	std::map<StringId, OwningPtr<T>> resources;

	std::optional<T> get(StringId name);
	bool registerResource(StringId name, OwningPtr<T> resource);
};
