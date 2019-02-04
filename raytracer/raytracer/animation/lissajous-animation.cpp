#include "animation/lissajous-animation.h"

using namespace animation;

namespace
{
	//pi constant
	const double pi = std::atan(1.0) * 4;
}

Animation<math::Point3D> animation::lissajous_animation(const int& x_ampl, const int& y_ampl, const int& z_ampl, const int& x_freq, const int& y_freq, const int& z_freq, math::Angle& x_phase, math::Angle& y_phase, math::Angle& z_phase, animation::Duration duration)
{
	//Creates animation for 0 to 2*pi 
	
	auto double_animation = basic(0, 2 * pi, duration);

	std::function<math::Point3D(TimeStamp)> lambda = [x_ampl, y_ampl, z_ampl, x_freq, y_freq, z_freq, x_phase, y_phase, z_phase, double_animation](TimeStamp now) -> math::Point3D {
		
		

		return math::Point3D(x_ampl * math::sin(math::Angle::radians(x_freq * double_animation(now)) + x_phase), y_ampl * math::sin(math::Angle::radians(y_freq * double_animation(now)) + y_phase), z_ampl * math::sin(math::Angle::radians(z_freq * double_animation(now)) + z_phase));


	};

	return make_animation(math::from_lambda(lambda), duration);

}