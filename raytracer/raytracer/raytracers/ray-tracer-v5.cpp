#include "ray-tracer-v5.h"

using namespace math;
using namespace raytracer;
using namespace imaging;

TraceResult raytracer::raytracers::_private_::RayTracerV5::trace(const Scene& scene, const Ray& ray) const
{
	//weight indicates how much the result of the call will count in the final result
	//smaller than 1%, immediately return 
	double weight = 1.0;
	return trace(scene, ray, weight);
}

TraceResult raytracer::raytracers::_private_::RayTracerV5::trace(const Scene & scene, const math::Ray& ray, double  weight) const
{
	if (weight < 0.1) {
		return TraceResult::no_hit(ray);
	}

	Color output_color = colors::black();

	if (weight > 0.01) {
		Hit hit;
		// Check if there has been a hit
		// Fill in TraceResult object with information about the trace
		if (scene.root->find_first_positive_hit(ray, &hit))
		{
			unsigned groupid = hit.group_id;
			double t = hit.t;
			MaterialProperties hitmaterial = hit.material->at(hit.local_position);
			//Compute mat.ambient (RayTracerV1) of hit material
			output_color += compute_ambient(hitmaterial);
			//Process lights
			output_color += process_lights(scene, hitmaterial, hit, ray);
			//Compute reflections
			output_color += compute_reflection(scene, hit, ray, hitmaterial, weight);
			return TraceResult(output_color, groupid, ray, t);
		}
	}
	return TraceResult::no_hit(ray);
}

imaging::Color raytracer::raytracers::_private_::RayTracerV5::compute_reflection(const Scene & scene, const Hit& hit, const math::Ray & ray, const MaterialProperties & properties, double  weight) const
{
	double reflectiveproperty = properties.reflectivity;
	//check if surface is reflective
	if (0 < reflectiveproperty && reflectiveproperty <= 1) {
		// Pretend that eye is at point of reflection now, trace the reflected ray, see  where it hits, and determine that color
		Vector3D normalizedhit = hit.normal.normalized();
		Vector3D direction = ray.direction - 2 * normalizedhit.dot(ray.direction) * normalizedhit;
		//Point3D reflectionpoint = hit.position;
		Ray refray(hit.position + direction * 0.00000001, direction);
		return reflectiveproperty * trace(scene, refray, weight*reflectiveproperty).color;
	}
	//If not reflective -> return black
	return colors::black();
}




raytracer::RayTracer raytracer::raytracers::v5()
{
	return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV5>());
}
