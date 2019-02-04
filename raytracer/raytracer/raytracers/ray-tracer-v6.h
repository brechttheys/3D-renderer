#pragma once

#include "ray-tracer.h"
#include "ray-tracer-v5.h"


namespace raytracer
{
	namespace raytracers
	{
		namespace _private_
		{
			class RayTracerV6 : public RayTracerV5
			{
			public:
				TraceResult trace(const Scene&, const math::Ray&, double weight) const override;
				imaging::Color compute_refraction(const Scene & scene, const MaterialProperties & properties, const math::Ray & ray, const Hit & hit, double weight) const;
				math::Ray compute_ray(const Scene & scene, const math::Ray & ray, const Hit & hit, double double1, double double2) const;
			};
		}
		RayTracer v6();
	}
}