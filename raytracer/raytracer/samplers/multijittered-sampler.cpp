#include "samplers/multijittered-sampler.h"
#include "easylogging++.h"
#include "math\rasterizer.h"
#include <random>
#include <stdlib.h> 
#include <time.h> 
#include <algorithm>
#include <iomanip>
using namespace std;
using namespace math;
using namespace raytracer;


namespace
{
	class MultiJitteredSampler : public raytracer::samplers::_private_::SamplerImplementation
	{
	public:
		MultiJitteredSampler(int n) {
			n_count = n;
		}

		void showMatrix(char c, double **matrix, int size) const {
			std::cout << "Matrix: " << c << "\n";
			for (int i = 0; i < size; i++) {
				std::cout << "row:" << std::setw(2) << i << " ";
				for (int j = 0; j < size; j++) {
					std::cout << matrix[i][j] << " ";
				}
				std::cout << "\n";
			}
			return;
		}

		void showXYMatrix(char c, double **matrix_x, double **matrix_y, int size) const {
			std::cout << "\n";
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					std::cout << "(" << i << "," << j << ")= " << "(" << matrix_x[i][j] << "," << matrix_y[i][j] << ") ";
				}
				std::cout << "\n";
			}
			return;
		}

		std::vector<Point2D> sample(const math::Rectangle2D& rectangle) const
		{
#ifdef SHOWOUTPUT 
			std::cout << "in multi-jittered Sample tester: " << n_count << "\n";
#endif
			std::vector<Point2D> points;					// The list of points that will be returned
			std::random_device rd;							// Will be used to obtain a seed for the random number engine
			std::mt19937 gen(rd());							// Standard mersenne_twister_engine seeded with rd()
			std::uniform_real_distribution<> dis(0.0, 1.0);	//select random values between 0.0 and 1.0
			std::srand(std::time(0)); 						//standard C randomizer, returns integers between 0 and RAND_MAX

															//allocates the sampler x and y values matrices (2 matrices will be created)
															//another option would be to work with 1 matrix with as cells a Point2D(x,y)
															//  but this solution is more difficult to implement for the shuffling and swaps 

			double **samples_x;								//dimensional matrix containing the x values
			samples_x = new double *[n_count];				//allocate rows for the x values matrix
			for (int i = 0; i < n_count; i++) {
				samples_x[i] = new double[n_count];			//allocate collumns for the x values matrix
			}
			double **samples_y;								//dimensional matrix containing the y values
			samples_y = new double *[n_count];				//allocate rows for the y values matrix
			for (int i = 0; i < n_count; i++) {
				samples_y[i] = new double[n_count];			//allocate collumns for the y values matrix
			}

			//reference algorithm: Kenneth Chiu, Peter Shirley, and Changyaw Wang.“Multi-jittered sampling.” 
			//In Graphics Gems IV, pp.370–374. Academic Press, May 1994
			//initialize the both x and y sampler matrices 
			for (int i = 0; i < n_count; i++) {
				for (int j = 0; j < n_count; j++) {
					samples_x[i][j] = i / n_count + (j + dis(gen) / n_count) / n_count;
					samples_y[i][j] = j / n_count + (i + dis(gen) / n_count) / n_count;
				}
			}
#ifdef SHOWOUTPUT 
			showMatrix('X', samples_x, n_count);
			showMatrix('Y', samples_y, n_count);
#endif

			//showXYMatrix('B', samples_x, samples_y, n_count);

			//Shuffle and swap x and y coordinates
			int randj;		//random value for the shuffle of the x coordinates
			int randi;		//random value for the shuffle of the y coordinates
			double temp;	//intermediate value for the swap
							//shuffle and swap the X coordinates within each column of cells
							//showMatrix('X', samples_x, n_count);
			for (int i = 0; i < n_count; i++) {
				for (int j = n_count - 1; j >= 1; j--) {
					randj = rand() % (j + 1); 			//select a rondom point between 0 and j
					temp = samples_x[i][j]; 			//temporary storage for the swap operation
					samples_x[i][j] = samples_x[i][randj];
					samples_x[i][randj] = temp;			//swap done
				}
			}
			//showMatrix('X', samples_x, n_count);
			//showXYMatrix('B', samples_x, samples_y, n_count);
			//showMatrix('X', samples_x, n_count);
			//showMatrix('Y', samples_y, n_count);
			//shuffle and swap the Y coordinates within each column of cells 
			for (int j = 0; j < n_count; j++) {
				for (int i = n_count - 1; i >= 1; i--) {
					randi = rand() % (i + 1); 			//select a rondom point between 0 and i
					temp = samples_y[i][j]; 			//temporary storage for the swap operation
					samples_y[i][j] = samples_y[randi][j];
					samples_y[randi][j] = temp;			//swap done
				}
			}
			//showMatrix('Y', samples_y, n_count);

			//push the random points on the vector
			for (int i = 0; i < n_count; i++) {
				for (int j = 0; j < n_count; j++) {
					points.push_back(rectangle.from_relative(Point2D(samples_x[i][j], samples_y[i][j])));
				}
			}
#ifdef SHOWOUTPUT 
			showXYMatrix('B', samples_x, samples_y, n_count);
#endif
			return points;

		}
	private:
		int n_count;
	};
}

Sampler raytracer::samplers::multijittered(int N)
{
	return Sampler(std::make_shared<MultiJitteredSampler>(N));
}
