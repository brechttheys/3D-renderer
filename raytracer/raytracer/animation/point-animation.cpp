#include "animation/point-animation.h"

using namespace animation;

Animation<math::Point3D> animation::point_animation(const math::Interval<math::Point3D>& animation_interval, animation::Duration duration)
{
	// Create an animation for 0 to 1
	auto double_animation = basic(0, 1, duration);

	// Creates a local function which takes a TimeStamp and returns a T.
	// The local function requires access to variables in the surrounding scope, in this case, double_animation and animation_interval.
	// These variables must be listed between [].
	std::function<math::Point3D(TimeStamp)> lambda = [double_animation, animation_interval, duration](TimeStamp now) -> math::Point3D {
		// Get the current value for t.
		// t == 0 at the beginning of the animation and grows linearly to 1.
		double t = double_animation(now);
		math::Point3D result = math::Point3D(animation_interval.lower);

		math::Vector3D differenceVector = (animation_interval.upper - animation_interval.lower);
		// Sanity check
		assert(math::interval(0.0, 1.0).contains(t));
		result += differenceVector * t;
		// Map [0, 1] to target interval. Say animation_interval equals [3, 9],
		// then 0 -> 3, 0.5 -> 6, 1 -> 9, etc.
		return result;
	};

	// Turns the function into an Animation object
	return make_animation(math::from_lambda(lambda), duration);
}

Animation<math::Point3D> animation::point_animation(const math::Point3D& from, const math::Point3D& to, animation::Duration duration)
{
	return point_animation(math::interval(from, to), duration);
}