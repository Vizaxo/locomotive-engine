#pragma once

// TODO: make this platform- and RHI-agnostic
#include "d2d1.h"

template <typename T>
struct Vector2 {
	T x;
	T y;
};

template <>
struct Vector2<float> {
	float x;
	float y;
	operator const D2D1_POINT_2F() {
		return {x, y};
	}
};

template <typename T> inline T lenSquared(Vector2<T> a) {
	return a.x*a.x + a.y*a.y;
}

template <typename T> inline Vector2<T> normalize(Vector2<T> a) {
	float lenSq = lenSquared(a);
	float len = std::sqrt(lenSq);
	return {a.x / len, a.y / len};
}

template <typename T> inline Vector2<T> floor(Vector2<T> in) { return {std::floor(in.x), std::floor(in.y)}; }
template <typename T> inline Vector2<T> ceil(Vector2<T> in) { return {std::ceil(in.x), std::ceil(in.y)}; }
template <typename T> inline T dot(Vector2<T> a, Vector2<T> b) { return a.x*b.x+a.y*b.y; }

template <typename T> inline Vector2<T> operator+(Vector2<T> a, Vector2<T> b) { return {a.x-b.x, a.y-b.y}; }
template <typename T> inline Vector2<T> operator-(Vector2<T> a, Vector2<T> b) { return {a.x-b.x, a.y-b.y}; }

template <typename T> inline Vector2<T> operator*(Vector2<T> a, T b) { return {a.x*b, a.y*b}; }
template <typename T> inline Vector2<T> operator/(Vector2<T> a, T b) { return {a.x/b, a.y/b}; }

typedef Vector2<float> v2f;
typedef Vector2<double> v2d;
typedef Vector2<int> v2i;

typedef v2f v2;
