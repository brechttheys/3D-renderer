#include "my_demo.h"
#include "demos/demo.h"
#include "util/lazy.h"
#include "easylogging++.h"

using namespace raytracer;
using namespace animation;
using namespace math;
using namespace imaging;
using namespace demos;


namespace
{
	class MeshDemo : public Demo
	{
	public:
		// C++ allows you to copy a superclass's constructor
		using Demo::Demo;

	protected:
		/// <summary>
		/// Creates the root. This method will be called for several values of <paramref name="now" />,
		/// thus creating an animation.
		/// </summary>
		raytracer::Primitive create_root(TimeStamp now) override
		{
			// Local imports. Allows us to write sphere() instead of raytracer::primitives::sphere()
			using namespace raytracer::primitives;
			using namespace raytracer::materials;

			// Define material properties
			MaterialProperties material_properties(
				colors::white(),      // ambient lighting, not supported yet
				colors::red(),        // diffuse lighting, not supported yet
				colors::black(),      // specular highlights, not supported yet
				0                     // specular exponent, not supported yet
			);

			// Create a uniform material: all parts of the primitive will be made out of the same material
			auto material = uniform(material_properties);

			//----TASK 1 -----
			//MOVING SPHERE FROM RIGHT TO LEFT

			// Create an animation of a double going from -5.0 to 5.0 in 1 second
			// It is important to write 5.0 and not 5, otherwise it will create an animation of ints instead of doubles
			//auto x_position = animation::animate(5.0, -5.0, 1_s);

			//auto test = animation::ease(x_position, functions::easing::linear());

			// Create a sphere. It has radius 1 and is centered at (0, 0, 0)
			//Primitive primitive = sphere();

			// Move the sphere. x_position(now) = asks the animation what x at this point in time
			//primitive = translate(Vector3D(x_position(now), 0, 0), primitive);

			//Assign a material to the sphere
			//primitive = decorate(material, primitive);

			//return primitive;

			//----TASK 2 -----
			//MOVING SPHERE FROM TOP TO BOTTOM

			// Create an animation of a double going from -5.0 to 5.0 in 1 second
			// It is important to write 5.0 and not 5, otherwise it will create an animation of ints instead of doubles
			//auto y_position = animation::animate(5.0, -5.0, 1_s);

			//auto test = animation::ease(x_position, functions::easing::linear());

			// Create a sphere. It has radius 1 and is centered at (0, 0, 0)
			//Primitive primitive = sphere();

			// Move the sphere. x_position(now) = asks the animation what x at this point in time
			//As x stays the same, the only variable is y
			//primitive = translate(Vector3D(0, y_position(now), 0), primitive);

			//Assign a material to the sphere
			//primitive = decorate(material, primitive);

			//return primitive;

			//----TASK 3 -----
			//TWO SPHERES MOVING FROM LEFT TO RIGHT AND VICE VERSA

			// Create an animation of a double going from -5.0 to 5.0 in 1 second
			// It is important to write 5.0 and not 5, otherwise it will create an animation of ints instead of doubles
			//auto x_position = animation::animate(5.0, -5.0, 1_s);

			//auto test = animation::ease(x_position, functions::easing::linear());

			// Creating two spheres. both with radius 1 and centered at (0, 0, 0)
			//std::vector<Primitive> primitives = { sphere(), sphere() };

			// Move the sphere. x_position(now) = asks the animation what x at this point in time

			//primitives.at(0) = translate(Vector3D(-x_position(now), 1, 0), primitives.at(0));
			//primitives.at(1) = translate(Vector3D(x_position(now), -1, 0), primitives.at(1));

			//for (int i = 0; i < primitives.size(); i++) {
			//	primitives.at(i) = decorate(material, primitives.at(i));
			//}

			//Primitive root_union = make_union(primitives);

			//return root_union;

			//----TASK 4 -----
			//MAKING THE SPHERE LOOK BIGGER IN TWO WAYS

			// Create an animation of a double going from -5.0 to 5.0 in 1 second
			// It is important to write 5.0 and not 5, otherwise it will create an animation of ints instead of doubles
			auto x_position = animation::animate(-5.0, 5.0, 1_s);

			auto test = animation::ease(x_position, functions::easing::linear());

			// Create a sphere. It has radius 1 and is centered at (0, 0, 0)
			Primitive primitive = sphere();

			// Move the sphere. x_position(now) = asks the animation what x at this point in time

			//WAY 1
			primitive = translate(Vector3D(x_position(now),0,4), primitive);

			//WAY 2
			//Change the distance between camera and viewing plane when creating the camera


			//Assign a material to the sphere
			primitive = decorate(material, primitive);

			return primitive;
		
		}

		/// <summary>
		/// Creates light sources.
		/// </summary>
		std::vector<raytracer::LightSource> create_light_sources(TimeStamp now) override
		{
			// Local import
			using namespace raytracer::lights;

			std::vector<LightSource> light_sources;

			// No lights

			return light_sources;
		}

		/// <summary>
		/// Creates camera.
		/// </summary>
		raytracer::Camera create_camera(TimeStamp) override
		{
			return raytracer::cameras::perspective(
				Point3D(0, 0, 10),         // position of eye
				Point3D(0, 0, 0),          // point the camera looks at
				Vector3D(0, 1, 0),         // up-vector: indicates camera is "standing up" (also use to rotate camera)
				1,                         // distance between eye and viewing plane (higher = closer)
				1                          // aspect ratio
			);
		}

	private:
		Primitive m_mesh;
	};
}

void demos::my_demo(std::shared_ptr<raytracer::pipeline::Consumer<std::shared_ptr<imaging::Bitmap>>> output)
{
	MeshDemo(500, 1_s, 30, 1).render(output);
}
