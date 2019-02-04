#include "raytracers/ray-tracer-v2.h"

using namespace imaging;
using namespace math;
using namespace raytracer;


TraceResult raytracer::raytracers::_private_::RayTracerV2::trace(const Scene& scene, const Ray& ray) const
{
	Hit hit;

	// Ask the scene for the first positive hit, i.e. the closest hit in front of the eye
	// If there's a hit, find_first_positive_hit returns true and updates the hit object with information about the hit
	if (scene.root->find_first_positive_hit(ray, &hit))
	{
		// There's been a hit
		// Fill in TraceResult object with information about the trace

		//Start met "lege" ray, 0 photons
		Color result = colors::black();

		//Haal MaterialProperties van de hit positie op
		MaterialProperties properties = hit.material->at(hit.local_position);

		//Bereken ambient lightning en voeg deze toe bij result
		result += compute_ambient(properties);

		//Process lights zal nu over alle lichten ittereren en het resultaat toevoegen aan result
		result += process_lights(scene, properties, hit, ray);

		// The hit object contains the group id, just copy it (group ids are important for edge detection)
		unsigned group_id = hit.group_id;

		// The t-value indicates where the ray/scene intersection took place.
		// You can use ray.at(t) to find the xyz-coordinates in space.
		double t = hit.t;

		// Group all this data into a TraceResult object.
		return TraceResult(result, group_id, ray, t);
	}
	else
	{
		// The ray missed all objects in the scene
		// Return a TraceResult object representing "no hit found"
		// which is basically the same as returning black
		return TraceResult::no_hit(ray);
	}
}

//geeft som van alle lichten terug
Color raytracer::raytracers::_private_::RayTracerV2::process_lights(const Scene & scene, const MaterialProperties& properties, const Hit & hit, const Ray & ray) const
{
	Color result = colors::black();
	for (LightSource current : scene.light_sources) {
		result += process_light_source(scene, properties, hit, ray, current);
	}
	return result;
}

//Verwerkt Lightray = ray met kleur
Color raytracer::raytracers::_private_::RayTracerV2::process_light_source(const Scene & scene, const MaterialProperties& properties, const Hit & hit, const Ray & ray, LightSource lightSource) const
{
	Color result = colors::black();
	for (LightRay current : lightSource->lightrays_to(hit.position)) result += process_light_ray(scene, properties, hit, ray, current);
	return result;
}

//Verwerk een LightRay
Color raytracer::raytracers::_private_::RayTracerV2::process_light_ray(const Scene & scene, const MaterialProperties& properties, const Hit & hit, const Ray & ray, const LightRay & lightRay) const
{
	Color result = colors::black();
	result += compute_diffuse(properties, hit, ray, lightRay);
	return result;
}

//berekent diffusie, normal vector is niet nodig om aan te maken doordat dit hetzelfde is als .normalized()
Color raytracer::raytracers::_private_::RayTracerV2::compute_diffuse(const MaterialProperties& properties, const Hit & hit, const Ray & ray, const LightRay & lightRay) const
{
	Color lightColor = lightRay.color; //de kleur van de lichtstraal
	Point3D lightSource = lightRay.ray.origin; //punt vanwaar de lichtstraal vertrekt
	Point3D hitPoint = hit.position; //punt waarvan we de kleur moeten berekenen
	Vector3D normalVector = hit.normal;//normaalvector van het geraakte punt
	double result = ((lightSource - hitPoint).normalized()).dot(normalVector);
	if (result <= 0) return colors::black();
	else {
		return lightColor * result*properties.diffuse;
	}
}

raytracer::RayTracer raytracer::raytracers::v2()
{
	return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV2>());
}

