#pragma once

template <typename T>
struct Vector2 {
	T x;
	T y;
};

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
