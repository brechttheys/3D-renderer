#pragma once
#include "animation/animation.h"
#include "math/interval.h"

namespace animation
{
	Animation<math::Point3D> point_animation(const math::Point3D& from, const math::Point3D& to, animation::Duration duration);

	Animation<math::Point3D> point_animation(const math::Interval<math::Point3D>& animation_interval, animation::Duration duration);
}