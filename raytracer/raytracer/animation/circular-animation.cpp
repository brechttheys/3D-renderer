#pragma once

#include "animation/animation.h"
#include "animation/interval-animation.h"
#include "animation/circular-animation.h"
#include "math/interval.h"
#include "animation/quaternion.h"

using namespace math;

namespace animation
{
	Animation<Point3D> circular(const math::Point3D& point,
		const Point3D& center,
		const Vector3D& rotation_axis,
		const Angle& start,
		const Angle& end,
		Duration duration) {

		auto double_animation = basic(0, 1, duration);
		Interval<Angle> angle_interval = Interval<Angle>(start, end);

		std::function<Point3D(TimeStamp)> lambda = [double_animation, angle_interval, point, center, rotation_axis](TimeStamp now) -> Point3D {
			double t = double_animation(now);				//start from a linear time span, from 0->1
			assert(math::interval(0.0, 1.0).contains(t));	//sanity check
			auto  angle = angle_interval.from_relative(t); // Map [0,1] to the target interval (angles in this case)
			auto xyz_now = rotation(angle, point, center, rotation_axis);
			return xyz_now;
		};

		// Turns the function into an Animation object
		return make_animation(math::from_lambda(lambda), duration);
	}

	math::Point3D rotation(const Angle& angle, const math::Point3D& point, const math::Point3D& center,
		const math::Vector3D& rotation_axis) {

		//Algorithm: see Quaternions -> Usage

		//1. Compute the quaternion p of the point we want to rotate
		Quaternion p = Quaternion(1.0, point.x(), point.y(), point.z());

		//2. Compute the quaternion q from an angle and a vector indication the axis
		Quaternion q; //creates default quaterion
		q.from_axis(angle.radians(), rotation_axis.x(), rotation_axis.y(), rotation_axis.z());

		//3. Multiply as follows : pq= q.p.q* (q*=conjugate of q)
		Quaternion cq = q;
		cq.conjugate();
		Quaternion pq = q * p*cq; //pq contains the Point3D(x,y,z) for the scene

		return Point3D(pq.getx(), pq.gety(), pq.getz());
	}

}


