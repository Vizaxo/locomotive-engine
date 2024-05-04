#include "PCH.h"
#include "ResourceManager.h"

template<typename T>
std::optional<T> ResourceManager<T>::get(StringId key) {
	if (auto search = resources.find(key); search != resources.end())
		return make_optional(search);
	else
		return std::empty;
}

template<typename T>
bool ResourceManager<T>::registerResource(StringId key, OwningPtr<T> res) {
	if (get(key).has_value()) return false;
	resources.emplace({key, res});
}