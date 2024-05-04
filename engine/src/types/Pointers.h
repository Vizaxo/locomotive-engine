#pragma once

template<typename T>
struct RefPtr {
	T* obj;
	T& operator*() { return *obj; }
	operator bool() const { return obj != nullptr; }
};

template<typename T>
struct OwningPtr {
	T* obj;
	RefPtr<T> getRef() { return {obj}; }
	T& operator*() { return *obj; }
	operator bool() const { return obj != nullptr; }
};

template<typename T>
OwningPtr<T> takeOwnership(T* p) {
	return {p};
}

template<typename T>
RefPtr<T> nullRef = {nullptr};

template<typename T>
OwningPtr<T> nullOwned = {nullptr};
