#include "samplers/random-sampler.h"
#include "easylogging++.h"
#include <random>

using namespace std;
using namespace math;
using namespace raytracer;


namespace
{
	class RandomSampler : public raytracer::samplers::_private_::SamplerImplementation
	{
	public:
		RandomSampler(int sample_count) {
			m_sample_count = sample_count;
		}

		std::vector<Point2D> sample(const math::Rectangle2D& rectangle) const
		{
#ifdef SHOWOUTPUT 
			std::cout << "In random sampler\n";
#endif
			std::vector<Point2D> points;					//vector containing the randompoints te be returned
			std::random_device rd;							//used to obtain a seed for the random engine number
			std::mt19937 gen(rd());							//standard mersenne algorithm
			std::uniform_real_distribution<> dis(0.0, 1.0); //select random values between 0.0 and 1.0

			for (int i = 0; i < m_sample_count; i++) { 		//select as many of random points required
				double x_dis_gen = dis(gen); 				//random point on the x-axis between 0 and 1
				double y_dis_gen = dis(gen); 				//random point on the y-axis between 0 and 1
				points.push_back(rectangle.from_relative(Point2D(x_dis_gen, y_dis_gen)));
#ifdef SHOWOUTPUT 
				std::cout << "random(x,y): " << Point2D(x_dis_gen, y_dis_gen) << " \n";
#endif
			}
			return points;
		}

	private:
		int m_sample_count;
	};
}

Sampler raytracer::samplers::random(int sample_count)
{
	return Sampler(std::make_shared<RandomSampler>(sample_count));
}