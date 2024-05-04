#pragma once

template <typename T>
struct Ptr {
	T* obj;
	T& operator*() { return *obj; }
	T* operator->() { return obj; }
	operator bool() const { return obj != nullptr; }
protected:
	Ptr() = default;
};

template<typename T>
struct RefPtr : Ptr<T> {
	//friend class OwningPtr<T>;
};

template<typename T>
struct OwningPtr : Ptr<T> {
	RefPtr<T> getRef() { return {this->obj}; }
	OwningPtr(T*&& ptr) { this->obj = ptr; } // Take ownership of rvalue reference from e.g. malloc() or new
};

template<typename T>
OwningPtr<T> takeOwnership(T* p) {
	return std::move(p);
}

template<typename T> RefPtr<T> nullRef = {nullptr};
template<typename T> OwningPtr<T> nullOwned = {nullptr};
