#pragma once
#include "animation/animation.h"
#include "math/interval.h"

namespace animation
{

	Animation<math::Point3D> lissajous_animation
	(
		const int& x_ampl, const int& y_ampl, const int& z_ampl, 
		const int& x_freq, const int& freq, const int& z_freq, 
		math::Angle& x_phase, math::Angle& y_phase, math::Angle& z_phase, animation::Duration duration
	);
}
