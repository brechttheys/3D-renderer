#include "samplers/nrooks-sampler.h"
#include "easylogging++.h"
#include "math/rasterizer.h"
#include <random>
#include <stdlib.h> 
#include <time.h> 
#include <algorithm>

using namespace std;
using namespace math;
using namespace raytracer;


namespace
{
	class NRooksSampler : public raytracer::samplers::_private_::SamplerImplementation
	{
	public:
		NRooksSampler(int n) {
			n_count = n;
		}

		void show_list(char c, const std::vector<int>& list) const {
			std::cout << "list: " << c << ": ";
			for (int i = 0; i < list.size(); i++) {
				std::cout << list[i] << " ";
			}
			std::cout << "\n";
		}

		std::vector<Point2D> sample(const math::Rectangle2D& rectangle) const
		{
#ifdef SHOWOUTPUT 
			std::cout << "in n-rooks Sample tester: n-rooks value: " << n_count << " creating: " << n_count * n_count << " subrectangles \n";
#endif
			std::vector<Point2D> points;		// The list of points that will be returned
			std::random_device rd;				// Will be used to obtain a seed for the random number engine
			std::mt19937 gen(rd());				// Standard mersenne_twister_engine seeded with rd()
			std::uniform_real_distribution<> dis(0.0, 1.0);	//select random real values between 0.0 and 1.0

															//Creating two lists and populating them with values according to parameter n_count that is provided
			std::vector<int> listI;
			std::vector<int> listJ;
			for (int a = 0; a < n_count; ++a) {
				listI.push_back(a);
				listJ.push_back(a);
			}
#ifdef SHOWOUTPUT 
			show_list('I', listI);
			show_list('J', listJ);
#endif
			//Shuffeling both lists a random number of times
			std::srand(std::time(0)); //standard C randomizer, returns integers between 0 and RAND_MAX
			std::random_shuffle(listI.begin(), listI.end());
			std::random_shuffle(listJ.begin(), listJ.end());
#ifdef SHOWOUTPUT 
			std::cout << "After shuffle\n";
			show_list('I', listI);
			show_list('J', listJ);
#endif
			int x; //contains the selected element (x_axis) from the listI
			int y; //contains the selected element (y_axis) from the listJ
			for (int i = 0; i < n_count; ++i) {
				x = listI.back();

				//popping last item of the list
				listI.pop_back();

				y = listJ.back();
				//popping last item of thelist
				listJ.pop_back();
#ifdef SHOWOUTPUT 
				std::cout << "After back and pop_back\n";
				show_list('I', listI);
				show_list('J', listJ);
				std::cout << "X,Y selected: " << x << " " << y << "\n";
#endif
				//get a random point within the subrectangle where x and y are the origin of the subrectangle
				Point2D randompoint = Point2D((x + dis(gen)) / n_count, (y + dis(gen)) / n_count); //get random point within subrectangle, as for jittered
#ifdef SHOWOUTPUT 
				std::cout << "Random point selected: " << randompoint << " \n";
#endif

				//Push the random point to the vector
				points.push_back(rectangle.from_relative(randompoint));
			}
			return points;

		}
	private:
		int n_count;
	};
}

Sampler raytracer::samplers::nrooks(int n)
{
	return Sampler(std::make_shared<NRooksSampler>(n));
}
