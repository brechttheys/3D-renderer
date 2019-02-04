#include "primitives/cylinder-along-x-primitive.h"
#include "util/misc.h"
#include "math/coordinate-systems.h"
#include "math/quadratic_equation.h"
#include "performance/performance.h"
#include "easylogging++.h"
#include <assert.h>

using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;


namespace
{
	class CylinderXImplementation : public raytracer::primitives::_private_::PrimitiveImplementation {

	public:
		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override {
			//See cylinder formules
			Point2D oStart(ray.origin.y(), ray.origin.z());
			Vector2D dRay(ray.direction.y(), ray.direction.z());

			//abc for discriminant
			double a = dRay.dot(dRay);
			double b = 2 * (oStart - Point2D(0, 0)).dot(dRay);
			double c = (oStart - Point2D(0, 0)).dot(oStart - Point2D(0, 0)) - 1;

			QuadraticEquation quadr(a, b, c);
			if (quadr.has_solutions()) {
				// If solutions: there are intersections
				double sol1 = quadr.x1();
				double sol2 = quadr.x2();

				// sort t1 < t2
				sort(sol1, sol2);

				// Sanity check 
				assert(sol1 <= sol2);

				// result
				std::vector<std::shared_ptr<Hit>> hits;

				// Two hits on heap to use for result
				auto hit = std::make_shared<Hit>();
				auto hit2 = std::make_shared<Hit>();

				// fill in both hits
				initialize_hit(hit.get(), ray, sol1);
				initialize_hit(hit2.get(), ray, sol2);

				//Put in vector
				hits.push_back(hit);
				hits.push_back(hit2);

				return hits;
			}
			else {
				// No solution, so no intersections
				return std::vector<std::shared_ptr<Hit>>();
			}
		}

		bool find_first_positive_hit(const Ray& ray, Hit* hit) const override {
			assert(hit != nullptr);

			//same as multiple hits
			Point2D oStart(ray.origin.y(), ray.origin.z());
			Vector2D dRay(ray.direction.y(), ray.direction.z());
			double a = dRay.dot(dRay);
			double b = 2 * (oStart - Point2D(0, 0)).dot(dRay);
			double c = (oStart - Point2D(0, 0)).dot(oStart - Point2D(0, 0)) - 1;

			QuadraticEquation quadr(a, b, c);

			if (quadr.has_solutions())
			{
				//See cylinder
				double sol1 = quadr.x1();
				double sol2 = quadr.x2();
				double t;

				if (smallest_greater_than_zero(sol1, sol2, &t)) {
					assert(t < hit->t);
					initialize_hit(hit, ray, t);
					return true;
				}
			}
			return false;
		}


		math::Box bounding_box() const override
		{
			// cylinder with 1x1x1: bounding box is a cube
			auto range = interval(-1.0, 1.0);

			return Box(range, range, range);
		}

	private:
		Point2D compute_uv_from_xyz(const Point3D& p) const
		{
			Cartesian3D cartesian{ p.x(), p.y(), p.z() };
			Spherical spherical = convert_coordinates<Spherical>(cartesian);

			double u = fmod(Interval<Angle>(-180_degrees, 180_degrees).to_relative(spherical.azimuth) + 0.5, 1.0);
			double v = Interval<Angle>(90_degrees, -90_degrees).to_relative(spherical.elevation);

			assert(0 <= u);
			assert(u <= 1);
			assert(0 <= v);
			assert(v <= 1);

			return Point2D(u, v);
		}
		void initialize_hit(Hit* hit, const Ray& ray, double t) const
		{
			// Update Hit object
			hit->t = t;
			hit->position = ray.at(t);
			hit->local_position.xyz = hit->position;
			hit->local_position.uv = compute_uv_from_xyz(hit->position);

			Vector3D out_norm(0, hit->position.y(), hit->position.z());
			if (ray.direction.dot(out_norm) < 0)
				hit->normal = out_norm;
			else
				hit->normal = -out_norm;

		}
	};
}

Primitive raytracer::primitives::cylinder_along_x()
{
	return Primitive(std::make_shared<CylinderXImplementation>());
}
