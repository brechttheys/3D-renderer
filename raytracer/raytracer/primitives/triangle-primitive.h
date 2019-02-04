#pragma once

#include "primitives/primitive.h"


namespace raytracer
{
	namespace primitives
	{
		Primitive triangle(const math::Point3D& p1, const math::Point3D& p2, const math::Point3D& p3);
	}
}
