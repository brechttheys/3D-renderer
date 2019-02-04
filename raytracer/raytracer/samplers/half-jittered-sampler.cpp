#include "samplers/half-jittered-sampler.h"
#include "easylogging++.h"
#include <stdlib.h> 
#include <random>

using namespace std;
using namespace math;
using namespace raytracer;


namespace
{
	class HalfJitteredSampler : public raytracer::samplers::_private_::SamplerImplementation
	{
	public:
		//constructor
		HalfJitteredSampler(int rows, int cols) : m_rows(rows), m_cols(cols) {};

		std::vector<Point2D> sample(const math::Rectangle2D& rectangle) const
		{
#ifdef SHOWOUTPUT 
			std::cout << "In halfjittered sampler\n";
#endif
			std::vector<Point2D> points; 	//vector containing the points to be returned by the sampler
			std::random_device rd;  		//used to obtain a seed for the random engine number
			std::mt19937 gen(rd()); 		//standard mersenne algorithm
			std::uniform_real_distribution<> dis(0.25, 0.75);	//select random values between 0.25 and 0.75 (around center)
																//this is the only difference with the jittered sampler

			for (int i = 0; i < m_rows; i++) {
				for (int j = 0; j < m_cols; j++) {
					Point2D randompoint = Point2D((i + dis(gen)) / m_rows, (j + dis(gen)) / m_cols); //get random point around center
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

Sampler raytracer::samplers::halfjittered(int rows, int cols)
{
	return Sampler(std::make_shared<HalfJitteredSampler>(rows, cols));
}
