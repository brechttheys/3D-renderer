#include "raytracers/ray-tracer-v3.h"

using namespace imaging;
using namespace math;
using namespace raytracer;

Color raytracer::raytracers::_private_::RayTracerV3::process_light_ray(const Scene & scene, const MaterialProperties& properties, const Hit& hit, const math::Ray& ray, const LightRay& lightRay) const
{
	//Base class
	Color result = RayTracerV2::process_light_ray(scene, properties, hit, ray, lightRay);

	//call comput_spec & add to result
	result += compute_specular(properties, hit, ray, lightRay);
	return result;
}

Color raytracer::raytracers::_private_::RayTracerV3::compute_specular(const MaterialProperties& properties, const Hit& hit, const Ray& ray, const LightRay& lightRay) const
{
	//getting color to check if black
	double specularExponent = properties.specular_exponent;
	Color specularColor = properties.specular;

	//If its black, stop and return black
	if (specularColor == colors::black() || specularExponent == 0)
		return colors::black();

	//getting data 
	Color lightRayColor = lightRay.color;
	Point3D lightSourcePoint = lightRay.ray.origin;
	Point3D hitPoint = hit.position;
	Vector3D normalVector = hit.normal;
	Point3D eyePoint = ray.origin;

	//direction of light
	Vector3D direction = (hitPoint - lightSourcePoint).normalized();

	//direction of reflection
	Vector3D reflectedDirection = direction.reflect_by(normalVector);

	//direction pov & calc angle
	Vector3D vect = (eyePoint - hitPoint).normalized();
	double cosA = vect.dot(reflectedDirection);

	//If angle < 0, return black, else calc the color
	if (cosA > 0)
		return lightRayColor * specularColor * pow(cosA, specularExponent);
	else
		return colors::black();
}

raytracer::RayTracer raytracer::raytracers::v3()
{
	return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV3>());
}
