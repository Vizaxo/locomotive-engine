#pragma once

#include "core/Log.h"

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
	RefPtr(RefPtr<T, Nullable>&& other) = default;
	RefPtr(const RefPtr<T, Nullable>& other) = default;
	RefPtr<T, Nullable>& operator=(const RefPtr<T, Nullable>& other) = default;

	RefPtr<T, false> getNonNull() { CHECKNULL; return {this->obj}; }
	RefPtr<T, true> getNullable() { CHECKNULL; return {this->obj}; }

	T*& getRaw() { CHECKNULL; return {this->obj}; }
};

template <typename T>
struct DefaultDelete {
	void operator()(void** mem) { delete *mem; }
};

template<typename T, bool Nullable = false, class Deleter = DefaultDelete<T>>
struct OwningPtr : Ptr<T, Nullable> {
	OwningPtr(T*&& ptr) { this->obj = ptr; CHECKNULL; } // Take ownership of rvalue reference from e.g. malloc() or new
	OwningPtr(OwningPtr<T, Nullable, Deleter>&& other) {
		this->obj = other.obj;
		CHECKNULL;
		other.obj = nullptr;
	}
	~OwningPtr() { Deleter d; d((void**)&this->obj); }

	OwningPtr<T, Nullable, Deleter>& operator=(OwningPtr<T, Nullable, Deleter>&& other) {
		CHECKNULL;
		Deleter()((void**)&this->obj);
		this->obj = other.obj;
		CHECKNULL;
		other.obj = nullptr;
		return *this;
	}

	OwningPtr<T, false, Deleter> getNonNull()
	{
		CHECKNULL;
		OwningPtr<T, false, Deleter> ret = std::move(this->obj);
		this->obj = nullptr;
		return std::move(ret);
	}

	OwningPtr<T, true, Deleter> getNullable()
	{
		CHECKNULL;
		OwningPtr<T, true, Deleter> ret = {std::move(this->obj)};
		this->obj = nullptr;
		return std::move(ret);
	}
	RefPtr<T, Nullable> getRef() { CHECKNULL; return {this->obj}; }
	T*& getRaw() { CHECKNULL; return {this->obj}; }
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

struct ArrayDelete {
	void operator()(void** mem) { delete[] *mem; }
};

struct ReleaseCOM {
	void operator()(void** obj) { if (*obj) (*(IUnknown**)obj)->Release(); obj=nullptr; }
};

struct Break {
	void operator()(void** obj) {
		//if (*obj) { (*(IUnknown**)obj)->Release(); }
		if (*obj) {
			LOG(Log::Level::INFO, Log::g_logTmp, "Releasing COM object at %p", *obj);
		}
		obj = nullptr;
	}
};
