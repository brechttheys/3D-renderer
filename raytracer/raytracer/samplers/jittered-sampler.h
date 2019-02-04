#pragma once

#include "samplers/sampler.h"


namespace raytracer
{
	namespace samplers
	{
		Sampler jittered(int rows, int cols);
	}
}
