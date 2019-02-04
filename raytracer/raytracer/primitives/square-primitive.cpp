#include "primitives/square-primitive.h"
#include "math/quadratic_equation.h"
#include "util/misc.h"
#include "math/interval.h"

using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;

namespace {
	class CoordinateSquareImplementation : public raytracer::primitives::_private_::PrimitiveImplementation {
		//See: plane primitive

	protected:
		const Vector3D m_normal;
		const Box bounding_box;

		CoordinateSquareImplementation(const Vector3D& normal, const Box bounding_box)
			: m_normal(normal), bounding_box(bounding_box) {
			assert(normal.is_unit());
			assert(bounding_box);
		}

		virtual void initialize_hit(Hit* hit, const Ray& ray, double t) const = 0;

	public:
		bool find_first_positive_hit(const Ray& ray, Hit* hit) const override
		{
			assert(hit != nullptr);

			Vector3D direction = ray.direction;
			double d = direction.dot(m_normal);

			if (d != 0 && bounding_box.is_hit_by(ray)) {
				Point3D origin = ray.origin;
				Point3D P = Point3D(0, 0, 0);
				double newt = ((P - origin).dot(m_normal) / d);
				if (newt > 0 && newt < hit->t) {
					initialize_hit(hit, ray, newt);
					return true;
				}
			}
			return false;
		}

		std::vector<std::shared_ptr<Hit>> find_all_hits(const math::Ray& ray) const override
		{
			std::vector<std::shared_ptr<Hit>> hits;

			// Compute denominator
			double denom = ray.direction.dot(m_normal);

			// If denominator == 0, there is no intersection (ray runs parallel to plane)
			if (denom != approx(0.0) && bounding_box.is_hit_by(ray))
			{
				// Compute numerator
				double numer = -((ray.origin - Point3D(0, 0, 0)).dot(m_normal));

				// Compute t
				double t = numer / denom;

				// Create hit object
				auto hit = std::make_shared<Hit>();

				// shared_ptr<T>::get() returns the T* inside the shared pointer
				initialize_hit(hit.get(), ray, t);

				// Put hit in list
				hits.push_back(hit);
			}

			return hits;
		}
	};

	class SquareXZImplementation : public CoordinateSquareImplementation {
	public:
		SquareXZImplementation()
			: CoordinateSquareImplementation(Vector3D(0, 1, 0), bounding_box()) { }

		math::Box bounding_box() const override {
			return Box(interval(-1.0, 1.0), interval(-0.01, 0.01), interval(-1.0, 1.0));
		}

	protected:
		void initialize_hit(Hit* hit, const Ray& ray, double t) const override {
			hit->t = t; //Fill in hit
			hit->position = ray.at(hit->t);
			hit->local_position.xyz = hit->position;
			hit->local_position.uv = Point2D(hit->position.x(), hit->position.z());
			hit->normal = ray.origin.y() > 0 ? m_normal : -m_normal;
		}
	};


	class SquareXYImplementation : public CoordinateSquareImplementation {
	public:
		SquareXYImplementation()
			: CoordinateSquareImplementation(Vector3D(0, 0, 1), bounding_box()) { }

		math::Box bounding_box() const override {
			return Box(interval(-1.0, 1.0), interval(-1.0, 1.0), interval(-0.01, 0.01));
		}

	protected:
		void initialize_hit(Hit* hit, const Ray& ray, double t) const override {
			hit->t = t; //Fill in hit
			hit->position = ray.at(hit->t);
			hit->local_position.xyz = hit->position;
			hit->local_position.uv = Point2D(hit->position.x(), hit->position.z());
			hit->normal = ray.origin.y() > 0 ? m_normal : -m_normal;
		}
	};
}

Primitive raytracer::primitives::xz_square()
{
	return Primitive(std::make_shared<SquareXZImplementation>());
}

Primitive raytracer::primitives::xy_square()
{
	return Primitive(std::make_shared<SquareXYImplementation>());
}
