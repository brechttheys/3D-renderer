#pragma once

#include "raytracers/ray-tracer.h"
#include "raytracers/ray-tracer-v3.h"


namespace raytracer
{
	namespace raytracers
	{
		namespace _private_
		{
			class RayTracerV4 : public RayTracerV3
			{
			public:
			protected:
				imaging::Color process_light_ray(const Scene&, const MaterialProperties&, const Hit&, const math::Ray&, const LightRay&) const override;
			};
		}

		/// <summary>
		/// Creates fourth simplest ray tracer.
		/// </summary>
		RayTracer v4();
	}
}