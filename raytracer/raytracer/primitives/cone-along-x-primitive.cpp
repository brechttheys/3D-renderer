#include "primitives/cone-along-x-primitive.h"
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
	class ConeAlongXImplementation : public raytracer::primitives::_private_::PrimitiveImplementation {

	public:

		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override {

			Point3D org = ray.origin;
			Vector3D dir = ray.direction;

			double a = (dir.y()*dir.y() + dir.z()*dir.z() - dir.x()*dir.x());
			double b = 2 * (org.y()*dir.y() + org.z()*dir.z() - org.x()*dir.x());
			double c = (org.y()*org.y() + org.z()*org.z() - org.x()*org.x());


		/*	double a = (dir.y()*dir.y() + dir.x()*dir.x() - dir.z()*dir.z());
			double b = 2 * (org.y()*dir.y() + org.x()*dir.x() - org.z()*dir.z());
			double c = (org.y()*org.y() + org.x()*org.x() - org.z()*org.z());*/

			QuadraticEquation quadr(a, b, c);


			if (quadr.has_solutions()) {
				// See cylinder
				double sol1 = quadr.x1();
				double sol2 = quadr.x2();

				sort(sol1, sol2);
				assert(sol1 <= sol2);

				std::vector<std::shared_ptr<Hit>> hits;
				auto hit = std::make_shared<Hit>();
				auto hit2 = std::make_shared<Hit>();

				initialize_hit(hit.get(), ray, sol1);
				initialize_hit(hit2.get(), ray, sol2);

				hits.push_back(hit);
				hits.push_back(hit2);

				return hits;
			}
			else
				return std::vector<std::shared_ptr<Hit>>();

		}

		bool find_first_positive_hit(const Ray& ray, Hit* hit) const override {
			assert(hit != nullptr);

			Point3D O_ = ray.origin;
			Vector3D D_ = ray.direction;
			double a = (D_.y()*D_.y() + D_.z()*D_.z() - D_.x()*D_.x());
			double b = 2 * (O_.y()*D_.y() + O_.z()*D_.z() - O_.x()*D_.x());
			double c = (O_.y()*O_.y() + O_.z()*O_.z() - O_.x()*O_.x());
			QuadraticEquation qeq(a, b, c);

			if (qeq.has_solutions()) {
				double t1 = qeq.x1();
				double t2 = qeq.x2();
				double t;

				// Smallest t value, closest intersection
				if (smallest_greater_than_zero(t1, t2, &t)) {

					assert(t < hit->t);
					initialize_hit(hit, ray, t);
					return true;
				}
			}
			return false;
		}
		math::Box bounding_box() const override {
			// [-1, 1] [-1, 1] [-1, 1] box.
			auto range = interval(-1.0, 1.0);

			return Box(range, range, range);
		}

	private:

		void initialize_hit(Hit* hit, const Ray& ray, double t) const {
			// Update Hit object, identical to cylinder
			hit->t = t;
			hit->position = ray.at(t);
			hit->local_position.xyz = hit->position;
			hit->local_position.uv = compute_uv_from_xyz(hit->position);
			hit->normal = compute_normal_at(ray, hit->position);

		}

		Point2D compute_uv_from_xyz(const Point3D& p) const {
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

		Vector3D compute_normal_at(const Ray& ray, const Point3D& position) const {

			// Compute normal on sphere at position. Always points outwards
			Vector3D outward_normal = Vector3D(-position.x(), position.y(), position.z()).normalized();

			// Make normal points towards the ray's origin
			Vector3D normal = ray.direction.dot(outward_normal) < 0 ? outward_normal : -outward_normal;

			return normal;
		}
	};
}

Primitive raytracer::primitives::cone_along_x() {
	return Primitive(std::make_shared<ConeAlongXImplementation>());
}
