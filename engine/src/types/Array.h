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

		void* newData = malloc(prevSize * stride);
		cur = newData;
		Allocator::AllocResult r = { newData, prevSize };
		return r;
	}

	void postAlloc() {
		free(prev);
		prev = cur;
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
	void add(T& elem) {
		allocIfNecessary(used + 1);

		data[used] = elem;
		used++;
	}

	void add(T&& elem) {
		allocIfNecessary(used + 1);

		data[used] = std::move(elem);
		used++;
	}

	T& operator[] (int index) {
		return data[index];
	}

	~Array() {
		for (int i = 0; i < used; ++i) {
			data[used].~T();
		}
	}


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
	T* data = nullptr;
	u32 size = 0;
	u32 used = 0;
};
