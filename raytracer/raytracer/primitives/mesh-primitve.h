#pragma once

#include "primitives/primitive.h"

namespace raytracer
{
	namespace primitives
	{
		Primitive mesh(const std::string&);
		//	Primitive optimize_mesh(std::vector<Primitive>);
		Primitive implementMesh(std::vector<Primitive>&);

	}
}