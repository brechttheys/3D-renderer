#include "samplers/jittered-sampler.h"
#include "easylogging++.h"
#include <stdlib.h> 
#include <random>

using namespace std;
using namespace math;
using namespace raytracer;


namespace
{
	class JitteredSampler : public raytracer::samplers::_private_::SamplerImplementation
	{
	public:
		//constructor
		JitteredSampler(int rows, int cols) : m_rows(rows), m_cols(cols) {};

		std::vector<Point2D> sample(const math::Rectangle2D& rectangle) const
		{
#ifdef SHOWOUTPUT 
			std::cout << "In jittered sampler\n";
#endif
			std::vector<Point2D> points;					//vector containing the random points to be returned by the sampler
			std::random_device rd;							//used to obtain a seed for the random engine number
			std::mt19937 gen(rd());							//standard mersenne algorithm
			std::uniform_real_distribution<> dis(0.0, 1.0); //select random values between 0.0 and 1.0

			for (int i = 0; i < m_rows; i++) {
				for (int j = 0; j < m_cols; j++) {
					Point2D randompoint = Point2D((i + dis(gen)) / m_rows, (j + dis(gen)) / m_cols); //get random point within subrectangle
#ifdef SHOWOUTPUT 
					std::cout << "randompoint: " << randompoint << " \n";
#endif
					points.push_back(rectangle.from_relative(randompoint));
				}
			}
			return points;
		}
	private:
		int m_rows;
		int m_cols;
	};
}

Sampler raytracer::samplers::jittered(int rows, int cols)
{
	return Sampler(std::make_shared<JitteredSampler>(rows, cols));
}
