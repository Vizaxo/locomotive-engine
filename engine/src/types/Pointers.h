#pragma once

// Owning and non-owning pointer types.
// Nullable pointers can be nullptr, non-nullable (default) pointers can't. Null errors are only checked in debug builds.

#if _DEBUG
#define NULLABLE_PTR_CHECKS_ENABLED 1
#endif

#if NULLABLE_PTR_CHECKS_ENABLED
#define CHECKNULL do { if (!Nullable && this->obj == nullptr) DebugBreak(); } while(0);
#else
#define CHECKNULL
#endif

template <typename T, bool Nullable = false>
struct Ptr {
	T* obj;
	T& operator*() { CHECKNULL; return *obj; }
	T* operator->() { CHECKNULL; return obj; }

	operator bool() const {
		CHECKNULL;
		if (Nullable)
			return obj != nullptr;
		else
			return true;
	}
protected:
	Ptr() = default;
};

template<typename T, bool Nullable = false>
struct RefPtr : Ptr<T, Nullable> {
	//friend class OwningPtr<T>;
	RefPtr(T* ptr) { this->obj = ptr; CHECKNULL; } // Get reference to raw pointer
	RefPtr<T, false> getNonNull() { CHECKNULL; return {this->obj}; }
	RefPtr<T, true> getNullable() { CHECKNULL; return {this->obj}; }
};

template<typename T, bool Nullable = false, class Deleter = std::default_delete<T>>
struct OwningPtr : Ptr<T, Nullable> {
	RefPtr<T, Nullable> getRef() { CHECKNULL; return {this->obj}; }
	OwningPtr(T*&& ptr) { this->obj = ptr; CHECKNULL; } // Take ownership of rvalue reference from e.g. malloc() or new
	OwningPtr(OwningPtr<T, Nullable, Deleter>&& other) {
		this->obj = other.obj;
		CHECKNULL;
		other.obj = nullptr;
	}
	OwningPtr<T, false, Deleter> getNonNull() { CHECKNULL; return {this->obj}; }
	OwningPtr<T, true, Deleter> getNullable() { CHECKNULL; return {this->obj}; }
	~OwningPtr() { Deleter()(this->obj); }
};

template<typename T, bool Nullable = false>
OwningPtr<T, Nullable> takeOwnership(T* p) {
#if NULLABLE_PTR_CHECKS_ENABLED
	if (!Nullable && p == nullptr) DebugBreak();
#endif
	return std::move(p);
}

template<typename T> RefPtr<T, true> nullRef = {nullptr};
template<typename T> OwningPtr<T, true> nullOwned = {nullptr};
