#include "PCH.h"
#include "Camera.h"

#include "core/Assert.h"
#include "types/Vector.h"

namespace Camera {

v3 Camera3D::worldRight() {
	return cross(lookDirection, worldUp);
}

m44 Camera3D::view() {
	return basis(worldRight(), worldUp, lookDirection);
}

void Camera3D::setCustomProj(m44 proj) {
	projection = proj;
	customProj = true;
}

constexpr float pi = 3.141592;
constexpr float degToRad(float deg) { return (deg / 180) * pi; }

m44 makePerspectiveProjection(float fovXrad, float aspect, float n, float f) {
	float halfWidth = n * tan(fovXrad / 2.f);
	float halfHeight = halfWidth / aspect;

	v3 camDir = v3{0.f, 0.f, -1.f};

	float l = -halfWidth;
	float r = halfWidth;
	float b = -halfHeight;
	float t = halfHeight;

	return transpose<4,4,float>( {
		2*n*(r-l), 0, -(r+l)/(r-l), 0,
		0, 2*n/(t-b), -(t+b)/(t-b), 0,
		0, 0, f/(f-n), -(f*n)/(f-n),
		0, 0, 1, 0 });
}

m44 Camera3D::viewProj(float aspect, float n, float f) {
	if (customProj) 
		return projection;
	else 
		return makePerspectiveProjection(degToRad(90), aspect, n, f) * view();
}

}
