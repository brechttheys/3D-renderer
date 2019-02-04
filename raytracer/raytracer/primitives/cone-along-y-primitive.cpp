#include "primitives/cone-along-y-primitive.h"
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
	class ConeAlongYImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
	{
	public:
		bool find_first_positive_hit(const Ray& ray, Hit* hit) const override
		{
			assert(hit != nullptr);

			Point3D O_ = ray.origin;
			Vector3D D_ = ray.direction;
			double a = (D_.x()*D_.x() + D_.z()*D_.z() - D_.y()*D_.y());
			double b = 2 * (O_.x()*D_.x() + O_.z()*D_.z() - O_.y()*D_.y());
			double c = (O_.x()*O_.x() + O_.z()*O_.z() - O_.y()*O_.y());
			QuadraticEquation qeq(a, b, c);

			if (qeq.has_solutions())
			{
				double t1 = qeq.x1();
				double t2 = qeq.x2();
				double t;

				// Find smallest positive t-value 
				if (smallest_greater_than_zero(t1, t2, &t))
				{
					// Check that our new t is better than the pre-existing t
					if (t < hit->t)
					{
						initialize_hit(hit, ray, t);

						return true;
					}
				}
			}

			return false;
		}

		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override
		{

			Point3D O_ = ray.origin;
			Vector3D D_ = ray.direction;
			double a = (D_.x()*D_.x() + D_.z()*D_.z() - D_.y()*D_.y());
			double b = 2 * (O_.x()*D_.x() + O_.z()*D_.z() - O_.y()*D_.y());
			double c = (O_.x()*O_.x() + O_.z()*O_.z() - O_.y()*O_.y());


			QuadraticEquation qeq(a, b, c);

			// If the ray hits the sphere, the quadratic equation will have solutions
			if (qeq.has_solutions())
			{
				// Quadratic equation has solutions: there are two intersections
				double t1 = qeq.x1();
				double t2 = qeq.x2();

				// We want t1 &lt; t2, swap them if necessary
				sort(t1, t2);

				// Sanity check (only performed in debug builds)
				assert(t1 <= t2);

				// Allocate vector on stack
				std::vector<std::shared_ptr<Hit>> hits;

				// Allocate two Hit objects on heap and store address in shared pointers
				auto hit1 = std::make_shared<Hit>();
				auto hit2 = std::make_shared<Hit>();

				// Initialize both hits
				initialize_hit(hit1.get(), ray, t1);
				initialize_hit(hit2.get(), ray, t2);

				// Put hits in vector
				hits.push_back(hit1);
				hits.push_back(hit2);

				// Return hit list
				return hits;
			}
			else
			{
				// No intersections to be found
				return std::vector<std::shared_ptr<Hit>>();
			}
		}

		math::Box bounding_box() const override
		{
			// Create a [-1, 1] x [-1, 1] x [-1, 1] box.
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
			hit->normal = compute_normal_at(ray, hit->position);

		}

		Vector3D compute_normal_at(const Ray& ray, const Point3D& position) const
		{

			// Compute normal on sphere at position. Always points outwards
			Vector3D outward_normal = Vector3D(position.x(), -position.y(), position.z()).normalized();

			// Make normal points towards the ray's origin
			Vector3D normal = ray.direction.dot(outward_normal) < 0 ? outward_normal : -outward_normal;

			return normal;
		}
	};
}

Primitive raytracer::primitives::cone_along_y()
{
	return Primitive(std::make_shared<ConeAlongYImplementation>());
}
