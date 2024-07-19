#pragma once

#include "types/Pointers.h"
#include "renderer/SpriteSheet.h"
#include "types/HexCoord.h"
#include "types/Map.h"

template <typename K>
struct TileMap<K> {
	RefPtr<SpriteSheet> spriteSheet;

	struct Tile {
		v2f spriteIndex;
		v2f spriteSize;
	};
	HashMap<HexCoord, Tile> map;

	RefPtr<Tile> get(K k);
	RefPtr<Tile> insert(K& k, Tile& t);
};

template <typename K>
RefPtr<TileMap<K>::Tile> TileMap<K>::get(K k) {
	auto* res = map.get(std::move(k));
	if (res)
		return &res->value;
	else
		return nullptr; // Generate new
}

template <typename K>
RefPtr<TileMap<K>::Tile> TileMap<K>::insert(K& k, Tile& t) {
	return &map.insert(k, t);
}
