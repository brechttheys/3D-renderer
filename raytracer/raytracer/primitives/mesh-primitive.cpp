#include "primitives/mesh-primitve.h"
#include "primitives/triangle-primitive.h"
#include "primitives/union-primitive.h"
#include <fstream>
#include <sstream>
#include "logging.h"
#include "primitives/bounding-box-primitive.h"
#include "easylogging++.h"

using namespace raytracer;
using namespace raytracer::primitives;
using namespace std;
using namespace math;

std::vector<Primitive> read_mesh(const std::string& path)
{
	std::vector<Point3D> points;
	std::vector<Primitive> triangles;
	char seps[] = "\n";
	ifstream infile(path);
	if (!infile.is_open()) {
		LOG(INFO) << "Could not open file";
	}
	string line;
	while (std::getline(infile, line)) {
		std::istringstream iss(line);
		string a;
		iss >> a;
		if (a == "v") {
			double x, y, z;
			iss >> x >> y >> z;
			Point3D p = Point3D(x, y, z);
			points.push_back(p);
		}
		// Als het een face is, vector opvullen met face-vertex indexes, en dan tellen hoeveel hoeken het vlak heeft
		if (a == "f") {
			std::vector<double> faces;
			double d;
			while (iss >> d) {
				faces.push_back(d);
			}
			int count = faces.size();
			if (count == 3) {
				Point3D p1 = points[faces[0] - 1];
				Point3D p2 = points[faces[1] - 1];
				Point3D p3 = points[faces[2] - 1];
				Primitive t = triangle(p1, p2, p3);
				triangles.push_back(t);
			}
			else if (count == 4) {
				triangles.push_back(triangle(points[faces[0] - 1], points[faces[1] - 1], points[faces[2] - 1]));
				triangles.push_back(triangle(points[faces[2] - 1], points[faces[3] - 1], points[faces[0] - 1]));
			}
			else if (count == 5) {
				triangles.push_back(triangle(points[faces[0] - 1], points[faces[1] - 1], points[faces[2] - 1]));
				triangles.push_back(triangle(points[faces[2] - 1], points[faces[3] - 1], points[faces[0] - 1]));
				triangles.push_back(triangle(points[faces[3] - 1], points[faces[4] - 1], points[faces[0] - 1]));
			}
			else if (count == 6) {
				triangles.push_back(triangle(points[faces[0] - 1], points[faces[1] - 1], points[faces[5] - 1]));
				triangles.push_back(triangle(points[faces[1] - 1], points[faces[2] - 1], points[faces[3] - 1]));
				triangles.push_back(triangle(points[faces[3] - 1], points[faces[4] - 1], points[faces[5] - 1]));
				triangles.push_back(triangle(points[faces[1] - 1], points[faces[3] - 1], points[faces[5] - 1]));
			}
			else {

			}
		}
	}
	if (!triangles.size() > 0) {
		LOG(INFO) << "Something went wrong reading the file";
	}
	infile.close();
	return triangles;
}
Primitive raytracer::primitives::mesh(const std::string& path)
{
	Primitive mesh = implementMesh(read_mesh(path));
	return mesh;
}

namespace
{
	class MeshImplementation :public raytracer::primitives::_private_::PrimitiveImplementation
	{
	public:

		MeshImplementation(std::vector<Primitive>& prims, int amount) : triangles(prims), iteration(amount), box(Box::empty())
		{
			generate_boxes(prims);
		}


		vector<shared_ptr<Hit>> find_all_hits(const Ray& ray) const override {
			std::vector<std::shared_ptr<Hit>> hits;
			// Als box door ray geraakt wordt, dan hits zoeken voor de driehoeken verbonden aan deze box.
			if (box.is_hit_by(ray)) {
				if (iteration == 1) {
					for (auto triangle : triangles) {
						Hit hit;
						auto foundhit = triangle->find_first_positive_hit(ray, &hit);
						if (foundhit) {
							hits.push_back(make_shared<Hit>(hit));
						}
					}
				}
				// Zelfde doen voor boxes die gegeneerd zijn
				std::vector<std::shared_ptr<Hit>> firstprimhits = firstprim->find_all_hits(ray);
				if (firstprimhits.size() > 0) {
					hits.insert(std::end(hits), std::begin(firstprimhits), std::end(firstprimhits));
				}
				std::vector<std::shared_ptr<Hit>> secondprimhits = secondprim->find_all_hits(ray);
				if (secondprimhits.size() > 0) {
					hits.insert(std::end(hits), std::begin(secondprimhits), std::end(secondprimhits));
				}
			}
			// Sorteer hits vervolgens zodat hits met kleinste t waarde aan begin van vector zitten
			for (size_t i = 1; i < hits.size(); ++i) {
				for (size_t j = 0; j < hits.size() - 1; ++j) {
					if (hits[j]->t > hits[i]->t) std::swap(hits[j], hits[i]);
				}
			}
			return hits;
		}
		Box bounding_box() const override
		{
			return box;
		}


	private:
		Primitive firstprim;
		Primitive secondprim;
		int iteration;
		vector<Primitive> triangles;
		Box box;

		void generate_boxes(vector<Primitive> prims) {

			// Als er geen primitives maar zijn, of de iterations gestopt zijn, return
			if (iteration == 0) {
				return;
			}
			if (prims.size() == 0) {
				iteration = 1;
				return;
			}

			Box mainbox = Box::empty();
			for (auto primitive : prims) {
				// Box initialiseren voor alles primitives
				mainbox = primitive->bounding_box().merge(mainbox);
			}

			Box box1 = Box::empty();
			Box box2 = Box::empty();
			std::vector<Primitive> first_box_triangles;
			std::vector<Primitive> second_box_triangles;
			math::Interval<double> x = mainbox.x();
			math::Interval<double> y = mainbox.y();
			math::Interval<double> z = mainbox.z();
			if (x.upper - x.lower >= z.upper - z.lower && x.upper - x.lower >= y.upper - y.lower) {
				// Als x langste kant is, verdeel in 2 boxes volgens x
				box1 = Box(interval(x.lower, x.center()), y, z);
				box2 = Box(interval(x.center(), x.lower), y, z);
				// Check voor elke driehoek of het in de box van laagste-x to center-x zit, of center-x tot hoogste-x
				for (auto triangle : prims) {
					if (triangle->bounding_box().x().lower >= x.lower && triangle->bounding_box().x().upper <= x.center()) {
						first_box_triangles.push_back(triangle);
					}
					else {
						second_box_triangles.push_back(triangle);
					}
				}
			}
			else if (y.upper - y.lower >= z.upper - z.lower && y.upper - y.lower >= x.upper - x.lower) {
				box1 = Box(x, interval(y.lower, y.center()), z);
				box2 = Box(x, interval(y.center(), y.upper), z);
				for (auto triangle : prims) {
					if (triangle->bounding_box().y().lower >= y.lower && triangle->bounding_box().y().upper <= y.center()) {
						first_box_triangles.push_back(triangle);
					}
					else {
						second_box_triangles.push_back(triangle);
					}
				}
			}
			else {
				box1 = Box(x, y, interval(z.lower, z.center()));
				box2 = Box(x, y, interval(z.center(), z.upper));
				for (auto triangle : prims) {
					if (triangle->bounding_box().z().lower >= z.lower && triangle->bounding_box().z().upper <= z.center()) {
						first_box_triangles.push_back(triangle);
					}
					else {
						second_box_triangles.push_back(triangle);
					}
				}
			}
			box = mainbox;

			// Recursief, iteration - 1 doen
			firstprim = Primitive(make_shared<MeshImplementation>(first_box_triangles, iteration - 1));
			secondprim = Primitive(make_shared<MeshImplementation>(second_box_triangles, iteration - 1));
		}
	};
};
Primitive raytracer::primitives::implementMesh(vector<Primitive>& triangles)
{
	return Primitive(make_shared<MeshImplementation>(triangles, 60));
}
