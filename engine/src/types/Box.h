#pragma once

#include "Vector.h"
#include "Types.h"

template <typename T>
struct Box2 {
	Vector<2,T> min, max;
	T width() { return max.x - min.x; }
	T height() { return max.y - min.y; }
};

typedef Box2<float> Box2f;
typedef Box2<double> Box2d;
typedef Box2<int> Box2i;
typedef Box2<uint> Box2u;
