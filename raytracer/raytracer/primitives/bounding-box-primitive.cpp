#include "primitives/bounding-box-primitive.h"
#include <assert.h>

using namespace raytracer;
using namespace raytracer::primitives;


namespace
{
	class BoundingBoxAccelerator : public raytracer::primitives::_private_::PrimitiveImplementation
	{
	public:
		BoundingBoxAccelerator(Primitive child)
			: m_child(child), box(child->bounding_box())
		{
			assert(child);
			assert(bounding_box);
		}

		std::vector<std::shared_ptr<Hit>> find_all_hits(const math::Ray& ray) const override
		{
			if (box.is_hit_by(ray))
				return m_child->find_all_hits(ray);
			return std::vector<std::shared_ptr<Hit>>();
		}

		math::Box bounding_box() const override
		{
			return box;
		}

	private:
		math::Box box;
		Primitive m_child;
	};
}

Primitive raytracer::primitives::bounding_box_accelerator(Primitive child)
{
	return Primitive(std::make_shared<BoundingBoxAccelerator>(child));
}
