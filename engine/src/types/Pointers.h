#pragma once

template<typename T>
struct RefPtr {
	T* obj;
};

template<typename T>
struct OwningPtr : RefPtr<T> {
	T* obj;
	RefPtr<T> getRef() { return {obj}; }
};

