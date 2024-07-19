#pragma once

#include "utils/Hashing.h"
#include "Array.h"
#include "Product.h"
#include "Pointers.h"

template <typename K, typename V, typename H=DefaultHash<K>>
struct HashMap {
	using Entry = Product<K, V>;

	struct EntryInternal {
		Entry e;
		bool active;
	};

	Entry* data;
	u32 n = 0;
	u32 m = 0;

	float loadFactor() const { return (float)n/(float)m; }
	static constexpr float loadFactorMax = 0.7f;

	RefPtr<Entry> insert(K& k, V& v);
	RefPtr<Entry, true> get(K& k);
	void resizeIfNeeded();

	RefPtr<Product<K, V>> insertNoResize(Entry& e);
};

template <typename K, typename V, typename H>
void HashMap<K, V, H>::resizeIfNeeded() {
	float loadFactor = ((float)n+1)/(float)m;
	if (loadFactor > loadFactorMax) {
		u32 newM = m*2;
		Entry* newData = calloc(sizeof(Entry) * newM);

		for (int i = 0; i < m; ++i) {
			Entry& e = data[i];
			if (e.active) {
				insertNoResize(std::move(e));
			}
		}

		m = newM;
		free(data); //All objects moved-from. Calling destructors *should* be a waste of time--is this a vaild assumption?
		data = newData;
	}
}

template <typename K, typename V, typename H>
RefPtr<Product<K, V>, true> HashMap<K, V, H>::get(K& k) {
	u32 hash = H{}(k);

	u32 index = hash;
	while (data[index].k != k) {
		++index;
		index %= m;
	}

	return &data[index];
}

template <typename K, typename V, typename H>
RefPtr<Product<K, V>> HashMap<K, V, H>::insertNoResize(Entry& e) {
	u32 hash = H{}(e.key);

	u32 insertIndex = hash;
	while (data[insertIndex].active) {
		++insertIndex;
		insertIndex %= m;
	}
	data[insertIndex] = std::move(e);
	return &data[insertIndex].e;
}

template <typename K, typename V, typename H>
RefPtr<Product<K, V>> HashMap<K, V, H>::insert(K& k, V& v) {
	resizeIfNeeded();
	return insertNoResize(Entry{std::move(k), std::move(v), true});
}
