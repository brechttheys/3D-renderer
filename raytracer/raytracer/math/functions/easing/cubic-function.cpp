#include "math/functions/easing/cubic-function.h"
#include "math/interval.h"
#include <assert.h>

using namespace math;
using namespace math::functions;


EasingFunction math::functions::easing::cubic_in()
{
	std::function<double(double)> lambda = [](double t) {
		assert(interval(0.0, 1.0).contains(t));
		return (t*t*t);
	};

	return from_lambda(lambda);
}

EasingFunction math::functions::easing::cubic_out()
{
	std::function<double(double)> lambda = [](double t) {
		assert(interval(0.0, 1.0).contains(t));
		return (3 * t) - (3 * t*t) + (t*t*t);
	};

	return from_lambda(lambda);
}

EasingFunction math::functions::easing::cubic_inout()
{
	std::function<double(double)> lambda = [](double t) {
		assert(interval(0.0, 1.0).contains(t));
		if (t < 0.5) {
			return 4 * t*t*t;
		}
		return -3 + 12 * t - 12 * (t*t) + 4 * (t*t*t);
	};

	return from_lambda(lambda);
}