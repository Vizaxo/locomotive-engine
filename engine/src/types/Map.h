#pragma once

#include "utils/Hashing.h"
#include "Array.h"
#include "Product.h"
#include "Pointers.h"

template <typename K, typename V, typename H=DefaultHash<K>>
struct HashMap {
	struct Entry {
		K key;
		V value;
	};

	struct EntryInternal {
		Entry e;
		bool active;
	};

	EntryInternal* data = nullptr;
	u32 n = 0;
	u32 m = 0;

	float loadFactor() const { return (float)n/(float)m; }
	static constexpr float loadFactorMax = 0.7f;

	RefPtr<Entry> insert(K k, V v);
	RefPtr<Entry, true> get(const K&& k);
	RefPtr<Entry, true> get(const K& k);
	void resizeIfNeeded();

	RefPtr<Entry> insertNoResize(EntryInternal&& e);
};

template <typename K, typename V, typename H>
void HashMap<K, V, H>::resizeIfNeeded() {
	float loadFactor = ((float)n+1)/(float)m;

	if (!data || loadFactor > loadFactorMax) {
		u32 oldM = m;
		m = oldM == 0 ? 4 : oldM*2;
		EntryInternal* newData = (EntryInternal*)calloc(m, sizeof(EntryInternal));
		n = 0;
		ASSERT(newData, "Calloc failed");
		EntryInternal* oldData = data;
		data = newData;

		for (int i = 0; i < oldM; ++i) {
			EntryInternal& e = oldData[i];
			if (e.active) {
				insertNoResize(std::move(e));
			}
		}

		free(oldData); //All objects moved-from. Calling destructors *should* be a waste of time--is this a vaild assumption?
	}
}

template <typename K, typename V, typename H>
RefPtr<typename HashMap<K, V, H>::Entry, true> HashMap<K, V, H>::get(const K&& k) {
	return get(k);
}

template <typename K, typename V, typename H>
RefPtr<typename HashMap<K, V, H>::Entry, true> HashMap<K, V, H>::get(const K& k) {
	if (m == 0) return nullptr;
	u32 hash = H{}(k);

	u32 index = hash % m;
	while (data[index].active && data[index].e.key != k) {
		++index;
		index %= m;
	}

	if (!data[index].active)
		return nullptr;

	return &data[index].e;
}

template <typename K, typename V, typename H>
RefPtr<typename HashMap<K, V, H>::Entry> HashMap<K, V, H>::insertNoResize(EntryInternal&& e) {
	u32 hash = H{}(e.e.key);

	ASSERT(n<m, "Trying to insert into a hashmap with insufficient space");
	u32 insertIndex = hash % m;
	ASSERT(m != 0, "");
	while (data[insertIndex].active) {
		++insertIndex;
		insertIndex %= m;
	}
	data[insertIndex] = std::move(e);
	n++;
	return &data[insertIndex].e;
}

template <typename K, typename V, typename H>
RefPtr<typename HashMap<K, V, H>::Entry> HashMap<K, V, H>::insert(K k, V v) {
	resizeIfNeeded();
	return insertNoResize({{std::move(k), std::move(v)}, true});
}
