#pragma once
#include "cameras/camera.h"
#include "samplers/sampler.h"


namespace raytracer
{
	namespace cameras
	{
		Camera depth_of_field_perspective(const math::Point3D& eye, const math::Point3D& look_at, const math::Vector3D& up, double distance, double aspect_ratio, double eye_size, const Sampler eye_sampler);
	}
}
