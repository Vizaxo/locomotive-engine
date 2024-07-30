#pragma once

#include <types/Types.h>

#define IMPLEMENT_ARRAY_OPERATORS const T& operator[](int x) const { return v[x]; }\
	T& operator[](int x) { return v[x]; }

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

template <int n, typename T>
T dot(Vector<n, T> lhs, Vector<n, T> rhs) {
	T sum = 0;
	for (int i = 0; i < n; ++i) {
		sum += lhs[i] * rhs[i];
	}
	return sum;
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
		ret[i] /= len;
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

template <int n, typename T> static Vector<n, T> operator+(Vector<n, T> a, Vector<n, T> b) {COMPONENTWISE_BINARY_OP(+)}
template <int n, typename T> static Vector<n, T> operator-(Vector<n, T> a, Vector<n, T> b) {COMPONENTWISE_BINARY_OP(-)}
template <int n, typename T> static Vector<n, T> operator*(Vector<n, T> a, Vector<n, T> b) {COMPONENTWISE_BINARY_OP(*)}
template <int n, typename T> static Vector<n, T> operator/(Vector<n, T> a, Vector<n, T> b) {COMPONENTWISE_BINARY_OP(/)}

template <int n, typename T> static Vector<n, T> operator+(Vector<n, T> a, T b) {VEC_SCALAR_BINARY_OP(+)}
template <int n, typename T> static Vector<n, T> operator-(Vector<n, T> a, T b) {VEC_SCALAR_BINARY_OP(-)}
template <int n, typename T> static Vector<n, T> operator*(Vector<n, T> a, T b) {VEC_SCALAR_BINARY_OP(*)}
template <int n, typename T> static Vector<n, T> operator/(Vector<n, T> a, T b) {VEC_SCALAR_BINARY_OP(/)}

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


template <int m, int n, typename T>
struct Matrix {
	/* Matrix format as in Real-time Rendering book
	 * vx.x vy.x vz.x t.x
	 * vx.y vy.y vz.y t.y
	 * vx.z vy.z vz.z t.z
	 * 0    0    0    1
	 *
	 * v00  v01  v02  v03
	 * v10  v11  v12  v13
	 * v20  v21  v22  v23
	 * v30  v31  v32  v33
	 *
	 * Column-major memory layout. I.e. t.x, t.y, t.z are in positions 12, 13, 14.
	 */
	T v[n][m];

	// Column-major indexing. m[2] gets the third column as a vector. Transpose first to get rows.
	const Vector<m, T>& operator[](const int x) const { return *reinterpret_cast<const Vector<m,T>*>(&v[x]); }
	Vector<m, T>& operator[](const int x) { return *reinterpret_cast<Vector<m,T>*>(&v[x]); }
};

typedef Matrix<4,4,float> m44;
typedef Matrix<3,4,float> m34;
typedef Matrix<4,4,float> m44f;
typedef Matrix<3,4,float> m34f;
typedef Matrix<4,4,double> m44d;
typedef Matrix<3,4,double> md4d;



template <int m, int n, typename T>
Matrix<n, m, T> transpose(Matrix<m, n, T> in) {
	Matrix<n, m, T> ret;
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			ret[i][j] = in[j][i];
		}
	}
	return ret;
}

template <int m, int n, int o, typename T>
Matrix<m, o, T> operator*(const Matrix<m, n, T>& lhs, const Matrix<n, o, T>& rhs) {
	Matrix<n, m, T> lhsT = transpose(lhs);
	Matrix<o, n, T> rhsT = transpose(rhs);
	Matrix<m, o, T> ret;
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < o; ++j) {
			ret[j][i] = dot(lhsT[i], rhs[j]);
		}
	}
	return ret;
}
