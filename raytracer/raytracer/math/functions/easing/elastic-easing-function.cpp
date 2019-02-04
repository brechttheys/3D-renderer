#include "math/functions/easing/elastic-easing-function.h"
#include "math/interval.h"
#include <assert.h>


math::functions::EasingFunction math::functions::easing::elastic(int numberOscillations, double energyLoss) {

	std::function<double(double)> lambda = [numberOscillations, energyLoss](double t) {
		const double PI = 3.14159265358979323846;

		if (energyLoss == 0) {
			return 1 - (std::cos((numberOscillations * PI - (PI / 2)) * t));
		}
		return 1 - ((std::pow((1 / energyLoss), t) * std::cos((numberOscillations * PI - (PI / 2)) * t)));
	};
	return from_lambda(lambda);
}
