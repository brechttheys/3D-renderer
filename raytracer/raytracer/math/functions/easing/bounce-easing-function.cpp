
#include "math/functions/easing/bounce-easing-function.h"
#include "math/interval.h"
#include <assert.h>

math::functions::EasingFunction math::functions::easing::bounce(int number, double absorption)
{
	std::function<double(double)> lambda = [number, absorption](double t) {
		const double PI = 3.141592653589793238463;
		if (absorption == 0) return 1 - (std::abs(std::cos((number*PI - (PI / 2)) *t)));
		return 1 - (std::abs((std::pow((1 / absorption), t))*std::cos((number*PI - (PI / 2))*t)));
	};
	return from_lambda(lambda);
}
