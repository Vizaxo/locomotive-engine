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

template <typename T> inline Vector2<T> operator+(Vector2<T> a, Vector2<T> b) { return {a.x+b.x, a.y+b.y}; }
template <typename T> inline Vector2<T> operator-(Vector2<T> a, Vector2<T> b) { return {a.x-b.x, a.y-b.y}; }

template <typename T> inline Vector2<T> operator*(Vector2<T> a, T b) { return {a.x*b, a.y*b}; }
template <typename T> inline Vector2<T> operator/(Vector2<T> a, T b) { return {a.x/b, a.y/b}; }

typedef Vector2<float> v2f;
typedef Vector2<double> v2d;
typedef Vector2<int> v2i;

typedef v2f v2;



template <typename T>
struct Vector3 {
	T x;
	T y;
	T z;
};

template <>
struct Vector3<float> {
	float x;
	float y;
	float z;
};

template <typename T> inline T lenSquared(Vector3<T> a) {
	return a.x*a.x + a.y*a.y + a.z*a.z;
}

template <typename T> inline Vector3<T> normalize(Vector3<T> a) {
	float lenSq = lenSquared(a);
	float len = std::sqrt(lenSq);
	return {a.x / len, a.y / len, a.z / len};
}

template <typename T> inline Vector3<T> floor(Vector3<T> in) { return {std::floor(in.x), std::floor(in.y), std::floor(in.z)}; }
template <typename T> inline Vector3<T> ceil(Vector3<T> in) { return {std::ceil(in.x), std::ceil(in.y), std::ceil(in.z)}; }
template <typename T> inline T dot(Vector3<T> a, Vector3<T> b) { return a.x*b.x+a.y*b.y+a.z*b.z; }

template <typename T> inline Vector3<T> operator+(Vector3<T> a, Vector3<T> b) { return {a.x+b.x, a.y+b.y, a.z+b.z}; }
template <typename T> inline Vector3<T> operator-(Vector3<T> a, Vector3<T> b) { return {a.x-b.x, a.y-b.y, a.z-b.z}; }

template <typename T> inline Vector3<T> operator*(Vector3<T> a, T b) { return {a.x*b, a.y*b, a.z*b}; }
template <typename T> inline Vector3<T> operator/(Vector3<T> a, T b) { return {a.x/b, a.y/b, a.z/b}; }

typedef Vector3<float> v3f;
typedef Vector3<double> v3d;
typedef Vector3<int> v3i;

typedef v3f v3;
