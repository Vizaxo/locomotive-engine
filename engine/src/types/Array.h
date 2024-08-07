#pragma once

#include "Product.h"
#include "Types.h"

struct Allocator {
	struct AllocResult {
		void* mem;
		u32 size;
	};
	// Returns pointer to the new memory, and number of elements it has size for
	AllocResult allocate(size_t stride, u32 elementsRequested) {}
	void postAlloc() {}
	void freeMem() {}
protected:
	Allocator() {}
	Allocator(size_t stride);
};

template<int initialSize>
struct DoublingAllocator : public Allocator {
	Allocator::AllocResult allocate(size_t stride, u32 elementsRequested) {
		if (elementsRequested == 0) {
			prevSize = initialSize;
		} else {
			while (prevSize < elementsRequested) {
				prevSize = prevSize == 0 ? 1 : prevSize * 2;
			}
		}

		void* newData = calloc(stride, prevSize);
		cur = newData;
		Allocator::AllocResult r = { newData, prevSize };
		return r;
	}

	void postAlloc() {
		free(prev);
		prev = cur;
	}

	void freeMem() {
		free(prev);
	}

	DoublingAllocator(size_t stride) {
		allocate(stride, initialSize);
		postAlloc();
	}

private:
	void* prev = nullptr;
	void* cur = nullptr;
	bool requiresFree = false;
	size_t prevSize = 0;
};

typedef DoublingAllocator<0> DefaultAllocator;


template <typename T, typename Alloc = DefaultAllocator>
struct Array {
	u32 add(T& elem) {
		allocIfNecessary(used + 1);

		data[used] = elem;
		u32 ret = used;
		used++;
		return ret;
	}

	u32 add(T&& elem) {
		allocIfNecessary(used + 1);

		data[used] = std::move(elem);
		u32 ret = used;
		used++;
		return ret;
	}

	void insertAt(u32 index, const T& elem, const T& nullElem) {
		allocIfNecessary(index+1);
		data[index] = elem;
		if (index > used) {
			for (int i = used; i < index; i++) {
				data[i] = nullElem;
			}
		}
		used = max(used, index+1);
	}

	T& pop() {
		return data[--used];
	}

	u32 num() {
		return used;
	}

	bool isEmpty() {
		return num() == 0;
	}

	T& operator[] (u32 index) {
		return data[index];
	}

	~Array() {
		for (int i = 0; i < used; ++i) {
			data[i].~T();
		}
		alloc.freeMem();
	}


	T* data = nullptr;
private:
	void allocIfNecessary(u32 desired) {
		if (desired <= size)
			return;

		Allocator::AllocResult res = alloc.allocate(sizeof(T), desired);

		size = res.size;
		if (res.mem != data)
			memcpy(res.mem, data, sizeof(T)*used);
		data = (T*)res.mem;

		alloc.postAlloc();
	}

	Alloc alloc = Alloc(sizeof(T));
	u32 size = 0;
	u32 used = 0;
};
