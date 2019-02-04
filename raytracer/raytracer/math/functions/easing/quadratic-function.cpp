#include "math/functions/easing/quadratic-function.h"
#include "math/interval.h"
#include <assert.h>

using namespace math;
using namespace math::functions;


EasingFunction math::functions::easing::quadratic_in() {
	//Traag starten, sneller naar einde

	std::function<double(double)> lambda = [](double t) {
		assert(interval(0.0, 1.0).contains(t));
		return (t*t);
	};
	return from_lambda(lambda);
}

EasingFunction math::functions::easing::quadratic_out() {
	//Traag einde, snel starten

	std::function<double(double)> lambda = [](double t) {
		assert(interval(0.0, 1.0).contains(t));
		return (2 * t) - (t*t);
	};
	return from_lambda(lambda);
}

EasingFunction math::functions::easing::quadratic_inout() {
	//Traag beginnen en einde, snel midden

	std::function<double(double)> lambda = [](double t) {
		assert(interval(0.0, 1.0).contains(t));
		if (t < 0.5) 
			return 2 * t*t;
		return -1 + 4 * t - 2 * (t*t);
	};
	return from_lambda(lambda);
}