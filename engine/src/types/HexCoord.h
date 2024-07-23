#pragma once

#include "Types.h"
#include "types/Vector.h"

// Axial hex coordinate system
// Maintains constraint q + r + s = 0
class HexCoord
{
public:
	i32 q;
	i32 r;
	
	void addS(int delta) { q+=delta; r-=delta; }
	int s() const { return -q-r; }
	static const float root3over2;;
	v2 cartesian() const { return {root3over2*2*(float)q + (float)r*root3over2, 1.5f * (float)r}; }
	u32 hash() const { return q ^ r; } //TODO: decent hashing function

	bool operator==(const HexCoord& other) const { return other.q == q && other.r == r; }
	bool operator!=(const HexCoord& other) const { return !(other == *this); }
};

const float HexCoord::root3over2 = std::sqrt(3.0f)/2.0f;
