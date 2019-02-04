#include "samplers/stratified-sampler.h"
#include "easylogging++.h"
#include "math\rasterizer.h"
#include <stdlib.h> 
#include <algorithm>

using namespace std;
using namespace math;
using namespace raytracer;


namespace
{
	class StratifiedSampler : public raytracer::samplers::_private_::SamplerImplementation
	{
	public:
		//constructor
		StratifiedSampler(int rows, int cols) : m_rows(rows), m_cols(cols) {};

		std::vector<Point2D> sample(const math::Rectangle2D& rectangle) const
		{
#ifdef SHOWOUTPUT 
			std::cout << "In stratified sampler\n";
#endif
			std::vector<Point2D> points;					//vector containing the points to be returned by the sampler
			for (int i = 0; i < m_rows; i++) {
				for (int j = 0; j < m_cols; j++) {
					Point2D center = Point2D((i + 0.5) / m_rows, (j + 0.5) / m_cols); //select center in each sub retangle
					points.push_back(rectangle.from_relative(center));				//add the selected center to the list
#ifdef SHOWOUTPUT
					std::cout << "center: " << center << " \n";
#endif
				}
			}
			return points;
		}
	private:
		int m_rows;
		int m_cols;
	};

}

Sampler raytracer::samplers::stratified(int N, int M)
{
	return Sampler(std::make_shared<StratifiedSampler>(N, M));
}
