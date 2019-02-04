#include "pipeline/ppm-consumer.h"
#include "imaging/ppm-format.h"

using namespace raytracer::pipeline;
using namespace imaging;


namespace
{
	class Ppm : public Consumer<std::shared_ptr<imaging::Bitmap>>
	{
	public:
		Ppm()
			: m_ppm() { }

		void consume(std::shared_ptr<imaging::Bitmap> bitmap) override
		{
			m_ppm.write_text_ppm(*bitmap, std::cout);
		}

	private:
		PPM m_ppm;
	};
}

std::shared_ptr<Consumer<std::shared_ptr<imaging::Bitmap>>> raytracer::pipeline::ppm()
{
	return std::make_shared<Ppm>();
}