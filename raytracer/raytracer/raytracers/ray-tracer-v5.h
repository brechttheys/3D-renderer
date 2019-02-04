#pragma once
#include "raytracers/ray-tracer.h"
#include "raytracers/ray-tracer-v4.h"


namespace raytracer
{
	namespace raytracers
	{
		namespace _private_
		{
			class RayTracerV5 : public RayTracerV4
			{
			public:
				TraceResult trace(const Scene&, const math::Ray&) const override;
				virtual TraceResult trace(const Scene&, const math::Ray&, double weight) const;
			protected:
				imaging::Color compute_reflection(const Scene&, const Hit& hit, const math::Ray&, const MaterialProperties&, double weight) const;
			};
		}

		RayTracer v5();
	}
}