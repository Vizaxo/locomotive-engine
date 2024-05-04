#pragma once

#include <string>
#include <unordered_map>
#include <optional>

#include "types/StringID.h" 
#include "types/Pointers.h" 

template<typename T>
struct ResourceManager {
	std::unordered_map<StringId, OwningPtr<T>> resources;

	RefPtr<T, true> get(StringId name);
	RefPtr<T, true> registerResource(StringId key, OwningPtr<T> res);
};

template<typename T>
RefPtr<T, true> ResourceManager<T>::get(StringId key) {
	if (auto it = resources.find(key); it != resources.end())
		return it->second.getRef().getNullable();
	else
		return nullRef<T>;
}

template<typename T>
RefPtr<T, true> ResourceManager<T>::registerResource(StringId key, OwningPtr<T> res) {
	if (get(key))
		return nullRef<T>;
	resources.emplace(key, std::move(res));
	return get(key);
}
