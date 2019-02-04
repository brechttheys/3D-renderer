#include "primitives/triangle-primitive.h"
#include "math/interval.h"
#include "easylogging++.h"

using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;

namespace
{
	class TriangleImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
	{
	public:
		Point3D p1, p2, p3;
		Vector3D normal;

		TriangleImplementation(const math::Point3D &p1_in, const math::Point3D &p2_in, const math::Point3D &p3_in) {
			p1 = p1_in; p2 = p2_in; p3 = p3_in;
			normal = (p2 - p1).cross(p3 - p1).normalized();
		}

		Point3D triangle_get_p1() { return p1; }
		Point3D triangle_get_p2() { return p2; }
		Point3D triangle_get_p3() { return p3; }


		bool find_first_positive_hit(const Ray& ray, Hit* hit) const override
		{
			auto hits = find_all_hits(ray);
			if (hits.size() > 0)
			{
				auto found = hits.back();
				if (found->t > 0 && found->t < hit->t)
				{
					*hit = *found;
					return true;
				}
			}
			return false;
		}

		std::vector<std::shared_ptr<Hit>> find_all_hits(const Ray& ray) const override
		{
			// Allocate vector of hits
			std::vector<std::shared_ptr<Hit>> hits;

			// implement the defintion of a triangle
			auto t = (p1 - ray.origin).dot(normal) / ray.direction.dot(normal);
			auto h = ray.origin + ray.direction * t;
			if ((p2 - p1).cross(h - p1).dot(normal) < 0) { return hits; }
			else if ((p3 - p2).cross(h - p2).dot(normal) < 0) { return hits; }
			else if ((p1 - p3).cross(h - p3).dot(normal) < 0) { return hits; }

			// Allocate hit objects
			auto hit = std::make_shared<Hit>();
			initialize_hit(hit.get(), ray, t);
			hits.push_back(hit);
			// Return hit list
			return hits;
		}

		math::Box bounding_box() const override
		{
			auto x_interval = getInterval(p1.x(), p2.x(), p3.x());
			auto y_interval = getInterval(p1.y(), p2.y(), p3.y());
			auto z_interval = getInterval(p1.z(), p2.z(), p3.z());
			return Box(x_interval, y_interval, z_interval);
		}

	private:

		math::Interval<double> getInterval(double a, double b, double c) const {
			double min_value = std::min(a, b); min_value = std::min(min_value, c);
			double max_value = std::max(a, b); max_value = std::max(max_value, c);
			return interval(min_value, max_value);
		}

		void initialize_hit(Hit* hit, const Ray& ray, double t) const
		{
			// Update Hit object
			hit->t = t;
			hit->position = ray.at(t);
			hit->local_position.xyz = hit->position;
			hit->local_position.uv = Point2D(0, 0);
			hit->normal = normal;
		}
	};
}

Primitive raytracer::primitives::triangle(const Point3D &x, const Point3D &y, const Point3D &z)
{
	return Primitive(std::make_shared<TriangleImplementation>(x, y, z));
}