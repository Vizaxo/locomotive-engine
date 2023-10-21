#pragma once

#include "types/Vector.h"

// Axial hex coordinate system
// Maintains constraint q + r + s = 0
class HexCoord
{
public:
	int q;
	int r;
	
	void addS(int delta) { q+=delta; r-=delta; }
	int s() { return -q-r; }
	float root3over2 = (float)std::sqrt(3.0f)/2;
	v2 cartesian() { return {root3over2*2*q + r*root3over2, 1.5f * r}; }
};
