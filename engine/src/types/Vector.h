#pragma once

#include <types/Types.h>

#define IMPLEMENT_ARRAY_OPERATORS const T& operator[](int x) const { return v[x]; }\
	T& operator[](int x) { return v[x]; }

#define IMPLEMENT_3D_TO_2D_SWIZZLES \
	Vector<2,T> xy() { return {x, y}; } \
	Vector<2,T> yx() { return {y, x}; } \
	Vector<2,T> xz() { return {x, z}; } \
	Vector<2,T> zx() { return {z, x}; } \
	Vector<2,T> yz() { return {y, z}; } \
	Vector<2,T> zy() { return {z, y}; }

#define IMPLEMENT_4D_TO_3D_SWIZZLES \
	Vector<3,T> xyz() { return {x, y, z}; } \

template <int n, typename T>
struct Vector {
	T v[n];
	IMPLEMENT_ARRAY_OPERATORS
};

template <typename T>
struct Vector<2,T> {
	union {
		T v[2];
		struct {
			T x;
			T y;
		};
	};
	IMPLEMENT_ARRAY_OPERATORS
};

template <typename T>
struct Vector<3,T> {
	union {
		T v[3];
		struct {
			T x;
			T y;
			T z;
		};
		struct {
			T pitch;
			T yaw;
			T roll;
		};
	};
	IMPLEMENT_3D_TO_2D_SWIZZLES;
	IMPLEMENT_ARRAY_OPERATORS
};

template <typename T>
struct Vector<4,T> {
	union {
		T v[4];
		struct {
			T x;
			T y;
			T z;
			T w;
		};
		Vector<3,T> v3;
	};
	IMPLEMENT_3D_TO_2D_SWIZZLES;
	IMPLEMENT_4D_TO_3D_SWIZZLES;
	IMPLEMENT_ARRAY_OPERATORS
};

template <int n, typename T>
T dot(Vector<n, T> lhs, Vector<n, T> rhs) {
	T sum = 0;
	for (int i = 0; i < n; ++i) {
		sum += lhs[i] * rhs[i];
	}
	return sum;
}

template <typename T>
Vector<3, T> cross(Vector<3, T> a, Vector<3, T> b) {
	return {
		a.y*b.z - a.z*b.y,
		a.z*b.x - a.x*b.z,
		a.x*b.y - a.y*b.x,
	};
}


template <int n, typename T> inline T lenSquared(Vector<n, T> a) {
	T sum = 0;
	for (int i = 0; i < n; ++i) {
		sum += a[i]*a[i];
	}
	return sum;
}

template <int n, typename T> Vector<n, T> replicate(T x) {
	Vector<n, T> ret;
	for (int i = 0; i < n; ++i)
		ret[i] = x;
	return ret;
}

template <int n, typename T> inline Vector<n, T> normalize(Vector<n, T> v) {
	Vector<n, T> ret;
	float lenSq = lenSquared(v);
	float len = std::sqrt(lenSq);
	for (int i = 0; i < n; ++i) {
		ret[i] = v[i]/len;
	}
	return ret;
}

#define COMPONENTWISE_UNARY(f) do {\
	Vector<n, T> ret;\
	for (int i = 0; i < n; ++i) {\
		ret[i] = f(in[i]);\
	}\
	return ret;\
} while(0);

#define COMPONENTWISE_UNARY_OP(op) do {\
	Vector<n, T> ret;\
	for (int i = 0; i < n; ++i) {\
		ret[i] = op in[i];\
	}\
	return ret;\
} while(0);

#define COMPONENTWISE_BINARY(f) do {\
	Vector<n, T> ret;\
	for (int i = 0; i < n; ++i) {\
		ret[i] = f(a[i], b[i]);\
	}\
	return ret;\
} while(0);

#define COMPONENTWISE_BINARY_OP(op) do {\
	Vector<n, T> ret;\
	for (int i = 0; i < n; ++i) {\
		ret[i] = a[i] op b[i];\
	}\
	return ret;\
} while(0);

#define VEC_SCALAR_BINARY(f) do {\
	Vector<n, T> ret;\
	for (int i = 0; i < n; ++i) {\
		ret[i] = f(a[i], b);\
	}\
	return ret;\
} while(0);

#define VEC_SCALAR_BINARY_OP(op) do {\
	Vector<n, T> ret;\
	for (int i = 0; i < n; ++i) {\
		ret[i] = (a[i] op b);\
	}\
	return ret;\
} while(0);

template <int n, typename T> Vector<n, T> floor(Vector<n, T> in) {COMPONENTWISE_UNARY(std::floor)}
template <int n, typename T> Vector<n, T> ceil(Vector<n, T> in) {COMPONENTWISE_UNARY(std::ceil)}

template <int n, typename T> Vector<n, T> operator-(Vector<n, T> in) {COMPONENTWISE_UNARY_OP(-)}

template <int n, typename T> static Vector<n, T> operator+(Vector<n, T> a, Vector<n, T> b) {COMPONENTWISE_BINARY_OP(+)}
template <int n, typename T> static Vector<n, T> operator-(Vector<n, T> a, Vector<n, T> b) {COMPONENTWISE_BINARY_OP(-)}
template <int n, typename T> static Vector<n, T> operator*(Vector<n, T> a, Vector<n, T> b) {COMPONENTWISE_BINARY_OP(*)}
template <int n, typename T> static Vector<n, T> operator/(Vector<n, T> a, Vector<n, T> b) {COMPONENTWISE_BINARY_OP(/)}

template <int n, typename T> static Vector<n, T> operator+(Vector<n, T> a, T b) {VEC_SCALAR_BINARY_OP(+)}
template <int n, typename T> static Vector<n, T> operator-(Vector<n, T> a, T b) {VEC_SCALAR_BINARY_OP(-)}
template <int n, typename T> static Vector<n, T> operator*(Vector<n, T> a, T b) {VEC_SCALAR_BINARY_OP(*)}
template <int n, typename T> static Vector<n, T> operator/(Vector<n, T> a, T b) {VEC_SCALAR_BINARY_OP(/)}
template <int n, typename T> static Vector<n, T> operator+(T b, Vector<n, T> a) {VEC_SCALAR_BINARY_OP(+)}
template <int n, typename T> static Vector<n, T> operator-(T b, Vector<n, T> a) {VEC_SCALAR_BINARY_OP(-)}
template <int n, typename T> static Vector<n, T> operator*(T b, Vector<n, T> a) {VEC_SCALAR_BINARY_OP(*)}
template <int n, typename T> static Vector<n, T> operator/(T b, Vector<n, T> a) {VEC_SCALAR_BINARY_OP(/)}

template <int n, typename T> static Vector<n, T>& operator+=(Vector<n,T>& lhs, const Vector<n, T>& rhs) {
	for (int i = 0; i < n; ++i) {
		lhs[i] += rhs[i];
	}
	return lhs;
}

template <int n, typename T> bool operator==(Vector<n, T> lhs, Vector<n, T> rhs) {
	bool ret = true;
	for (int i = 0; i < n; ++i) {
		ret = ret && (lhs[i] == rhs[i]);
	}
	return ret;
}
template <int n, typename T> inline bool operator!=(Vector<n, T> lhs, Vector<n, T> rhs) { return !(lhs == rhs); }

typedef Vector<2, float> v2;
typedef Vector<3, float> v3;
typedef Vector<4, float> v4;
typedef Vector<2, float> v2f;
typedef Vector<3, float> v3f;
typedef Vector<4, float> v4f;
typedef Vector<2, double> v2d;
typedef Vector<3, double> v3d;
typedef Vector<4, double> v4d;
typedef Vector<2, i32> v2i;
typedef Vector<3, i32> v3i;
typedef Vector<4, i32> v4i;

static inline Vector<4,float> mkv4(const v3f& v, float w) { return {v.x, v.y, v.z, w}; }
static inline Vector<4,float> mkv4f(const v3f& v, float w) { return {v.x, v.y, v.z, w}; }
