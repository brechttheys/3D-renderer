#include "cameras/depth-of-field-perspective-camera.h"
#include "cameras/displaceable-camera.h"
#include "cameras/perspective-camera.h"
#include "samplers/sampler.h"
#include "math/point.h"
#include <memory>



using namespace raytracer;
using namespace raytracer::cameras;
using namespace math;


namespace
{

	class DepthOfFieldPerspectiveCamera : public raytracer::cameras::_private_::DisplaceableCamera {

	public:
		DepthOfFieldPerspectiveCamera(const math::Matrix4x4 transformation, const std::vector<Camera> cameras) : DisplaceableCamera(transformation), cameras(cameras) {}

		void enumerate_untransformed_rays(const Point2D& point, std::function<void(const math::Ray&)> callback) const {
			assert(interval(0.0, 1.0).contains(point.x()));
			assert(interval(0.0, 1.0).contains(point.y()));

			for (int n = 0; n < cameras.size(); ++n) {
				enumerate_rays(point, callback);
				//?
			}
		}

	private:
		std::vector<Camera> cameras;
	};
}

Camera raytracer::cameras::depth_of_field_perspective(const math::Point3D& eye, const math::Point3D& look_at, const math::Vector3D& up, double distance, double aspect, double eye_size, const Sampler eye_sampler) {
	//Sanity check
	assert(up.is_unit);

	//Create few variables, see explenation
	Matrix4x4 transfMatrix = raytracer::cameras::_private_::create_transformation(eye, look_at, up);
	Point3D canonical_look_at = Point3D(0.0, 0.0, math::distance(eye, look_at));													
	std::vector<Camera> cameras;

	//Create eye area
	double width = eye_size * aspect;
	double height = eye_size;
	Vector2D x = Vector2D(width, 0.0);
	Vector2D y = Vector2D(0.0, height);
	Rectangle2D eye_area = Rectangle2D(Point2D(-(width/2), -(height/2)), x, y);// Of: Rectangle2D eye_area = Rectangle2D(0.0, 0.0), x, y);

	//All points looking toward the object
	std::vector<Point2D> sampledPoints = eye_sampler->sample(eye_area);

	//Fill in camers with each sampled point and previous variables
	for (int i = 0; i < sampledPoints.size(); i++) { 
		Point3D tempPoint = Point3D(sampledPoints[i].x(), sampledPoints[i].y(), 0.0);								

		cameras.push_back(raytracer::cameras::perspective(tempPoint, canonical_look_at, Vector3D(0.0, 1.0, 0.0), distance, aspect));
	}
	//Create and return the DepthOfFieldCamera
	return Camera(std::make_shared<::DepthOfFieldPerspectiveCamera>(transfMatrix, cameras));
}

