#pragma once

#include "quaternion.h"
#include "math/angle.h"
#include <cmath>

using namespace math;
using namespace animation;

//default constructor
Quaternion::Quaternion() { w = 1.0;	x = 0.0; y = 0.0; z = 0.0; };
// valued based constructor
Quaternion::Quaternion(const double w, const double x, const double y, const double z) : w(w), x(x), y(y), z(z) {};

//getters
double  Quaternion::getx() { return x; }
double  Quaternion::gety() { return y; }
double  Quaternion::getz() { return z; }

//conjugates the quaternion
void Quaternion::conjugate() {
	x = -x;
	y = -y;
	z = -z;
	return;
}

//creates a quaternion from axis (theta in radians, axis in x,y,z double coordinates)
void Quaternion::from_axis(double theta, double rx, double ry, double rz) {
	w = cos(theta / 2);
	x = rx * sin(theta / 2);
	y = ry * sin(theta / 2);
	z = rz * sin(theta / 2);
	return;
}

//creates a quaternion from axis (theta in radians, axis in Vector3D coordinates)
//calls the previous method
void Quaternion::from_axis(math::Angle& Angle, const math::Vector3D&  axis) {
	return from_axis(Angle.radians(), axis.x(), axis.y(), axis.z());
}

//operators

//Quaternion + Quaternion
Quaternion Quaternion::operator +(const Quaternion& q) {
	return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
}
//Quaternion - Quaternion
Quaternion Quaternion::operator -(const Quaternion& q) {
	return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
}
//Quaternion * Quaternion
Quaternion Quaternion::operator *(const Quaternion& q) {
	double w_new = w * q.w - (x*q.x + y * q.y + z * q.z);
	double x_new = w * q.x + q.w*x + y * q.z - z * q.y;
	double y_new = w * q.y + q.w*y + z * q.x - x * q.z;
	double z_new = w * q.z + q.w*z + x * q.y - y * q.x;
	w = w_new; x = x_new; y = y_new; z = z_new;
	return Quaternion(w, x, y, z);
}
//Quaternion * double
Quaternion Quaternion::operator *(double d) {
	return Quaternion(d*w, d*x, d*y, d*z);
}
//Quaternion / double
Quaternion Quaternion::operator /(double d) {
	return Quaternion(w / d, x / d, y / d, z / d);
}
//Quaternion += Quaternion
Quaternion Quaternion::operator +=(const Quaternion& q) {
	return Quaternion(w += q.w, x += q.x, y += q.y, z += q.z);
}
//Quaternion -= Quaternion
Quaternion Quaternion::operator -=(const Quaternion& q) {
	return Quaternion(w -= q.w, x -= q.x, y -= q.y, z -= q.z);
}
//Quaternion *= double
Quaternion Quaternion::operator *=(double d) {
	return Quaternion(w *= d, x *= d, y *= d, z *= d);
}
//Quaternion /= double
Quaternion Quaternion::operator /=(double d) {
	return Quaternion(w /= d, x /= d, y /= d, z /= d);
}

