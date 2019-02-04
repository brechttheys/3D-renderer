//Quaternions
#pragma once

#include "math/function.h"
#include "math/point.h"
#include "math/interval.h"
#include "math/functions/easing-functions.h"
#include "math/angle.h"
#include "animation/duration.h"
#include "animation/time-stamp.h"
#include "easylogging++.h"
#include <assert.h>
#include "math/vector.h"
#include "math/point.h"

namespace animation
{
	class Quaternion
	{
	public:
		// default constructor
		Quaternion();
		// valued based constructor
		Quaternion(const double w, const double x, const double y, const double z);
		//gettters
		double getx();
		double gety();
		double getz();

		// conjugates the quaternion
		void conjugate();

		// creates a quaternion from axis (theta in radians, axis in x,y,z double coordinates)
		void from_axis(double theta, double x, double y, double z);

		//creates a quaternion from axis (theta in radians, axis in Vector3D coordinates)
		//calls the previous method
		void from_axis(math::Angle& Angle, const math::Vector3D&  axis);

		//operators
		Quaternion operator +(const Quaternion& q); //Quaternion + Quaternion		
		Quaternion operator -(const Quaternion& q); //Quaternion - Quaternion
		Quaternion operator *(const Quaternion& q); //Quaternion * Quaternion
		Quaternion operator *(double d); // Quaternion * double
		Quaternion operator /(double d); //Quaternion / double 
		Quaternion operator +=(const Quaternion& q); //Quaternion += Quaternion
		Quaternion operator -=(const Quaternion& q); //Quaternion -= Quaternion
		Quaternion operator *=(double d); //Quaternion *= double
		Quaternion operator /=(double d); //Quaternion /= double

	private:
		double w, x, y, z;
	};
}
