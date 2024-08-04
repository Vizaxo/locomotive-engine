#pragma once

#include "Vector.h"

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

typedef Matrix<1,1,float> m11;
typedef Matrix<2,2,float> m22;
typedef Matrix<3,3,float> m33;
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

template <typename T>
Matrix<4,4,T> translate(Vector<3,T> t) {
	return transpose<4,4,T>(
		{ 1.0, 0.0, 0.0, t.x,
	         0.0, 1.0, 0.0, t.y,
	         0.0, 0.0, 1.0, t.z,
	         0.0, 0.0, 0.0, 1.0 });
}

template <typename T>
Matrix<4,4,T> rotX(T a) {
	return transpose<4,4,T>(
		{ 1.0, 0.0, 0.0, 0.0,
	         0.0, cos(a), -sin(a), 0.0,
	         0.0, sin(a), cos(a), 0.0,
	         0.0, 0.0, 0.0, 1.0 });
}

template <typename T>
Matrix<4,4,T> rotY(T a) {
	return transpose<4,4,T>(
		{ cos(a), 0.0, sin(a), 0.0,
	         0.0, 1.0, 0.0, 0.0,
	         -sin(a), 0.0, cos(a), 0.0,
	         0.0, 0.0, 0.0, 1.0 });
}

template <typename T>
Matrix<4,4,T> rotZ(T a) {
	return transpose<4,4,T>(
		{ cos(a), -sin(a), 0.0, 0.0,
	         sin(a), cos(a), 0.0, 0.0,
	         0.0, 0.0, 1.0, 0.0,
	         0.0, 0.0, 0.0, 1.0 });
}

template <typename T>
Matrix<4,4,T> basis(Vector<3,T> x, Vector<3,T> y, Vector<3,T> z) {
	return transpose<4,4,T>(
		{ x.x, y.x, z.x, 0.0,
	         x.y, y.y, z.y, 0.0,
	         x.z, y.z, z.z, 0.0,
	         0.0, 0.0, 0.0, 1.0 });
}

template <int n, typename T>
constexpr Matrix<n,n,T> id() {
	Matrix<n,n,T> ret;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			ret[i][j] = i == j ? 1.0 : 0.0;
	return ret;
}

static inline m11 id11 = id<1, float>();
static inline m22 id22 = id<2, float>();
static inline m33 id33 = id<3, float>();
static inline m44 id44 = id<4, float>();
