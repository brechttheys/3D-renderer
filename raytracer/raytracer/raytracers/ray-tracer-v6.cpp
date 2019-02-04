#include "ray-tracer-v6.h"

using namespace imaging;
using namespace math;
using namespace raytracer;

TraceResult raytracer::raytracers::_private_::RayTracerV6::trace(const Scene & scene, const math::Ray & ray, double  weight) const
{
	Color output_color = colors::black();

	if (weight > 0.01) {
		Hit hit;

		if (scene.root->find_first_positive_hit(ray, &hit))
		{
			MaterialProperties properties = hit.material->at(hit.local_position);
			output_color += compute_ambient(properties);
			output_color += process_lights(scene, properties, hit, ray);
			output_color += compute_reflection(scene, hit, ray, properties, weight); //Reflectie berekenen uit Raytracer v5
			output_color += compute_refraction(scene, properties, ray, hit, weight); //Hier gaan we de refractie in het object berekenen
			unsigned groupid = hit.group_id;
			double t = hit.t;
			return TraceResult(output_color, groupid, ray, t);
		}
	}
	return TraceResult::no_hit(ray);
}

imaging::Color raytracer::raytracers::_private_::RayTracerV6::compute_refraction(const Scene &scene, const MaterialProperties &props, const math::Ray &ray, const Hit &hit, double weight) const {
	if (props.transparency > 0) {
		Ray refray = compute_ray(scene, ray, hit, 1.0, props.refractive_index);
		/*if (refray == ray) {
		return colors::black();
		}*/

		Hit out;
		if (scene.root->find_first_positive_hit(refray, &out))
		{
			Ray outray = compute_ray(scene, refray, out, props.refractive_index, 1.0);
			return trace(scene, outray, weight * props.transparency).color * props.transparency;
		}
	}
	return colors::black();
}

math::Ray raytracer::raytracers::_private_::RayTracerV6::compute_ray(const Scene &scene, const math::Ray &ray, const Hit &hit, double double1, double double2) const
{
	// Calculate vector for incoming ray
	Vector3D incoming = (hit.position - ray.origin).normalized();
	// See where that ray leaves the shape
	Vector3D out_x = (double1 / double2) * (incoming - (incoming.dot(hit.normal)) * hit.normal);
	if ((1 - out_x.norm_sqr()) < 0) {
		return ray;
	}
	Vector3D out_y = (0 - sqrt(1 - out_x.norm_sqr())) * hit.normal;
	Vector3D dir = out_x + out_y;
	Point3D out = ray.at(hit.t) + (0.00001 * dir);

	Ray refray = Ray(out, dir);

	return refray;
}


raytracer::RayTracer raytracer::raytracers::v6()
{
	return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV6>());
}

