#pragma once
#include "animation/animation.h"
#include "math/interval.h"

namespace animation
{
	Animation<math::Point3D> circular(const math::Point3D& point, //original position where to start
		const math::Point3D& center,			//point around which animation resolves, rotates around (0,0,0) in this implementation
		const math::Vector3D& rotation_axis,	//normal on the plane in which the circular motion takes place
		const math::Angle& start,				//start Angle
		const math::Angle& end,					//end Angle
		Duration duration);						//how long the duration takes

	math::Point3D rotation(const math::Angle& angle, const math::Point3D& point, const math::Point3D& center,
		const math::Vector3D& rotation_axis);

}