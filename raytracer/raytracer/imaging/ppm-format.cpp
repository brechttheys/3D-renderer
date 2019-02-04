#include "ppm-format.h"
#include "easylogging++.h"

using namespace imaging;

namespace
{
	struct RGBColor
	{
		uint8_t r, g, b;

		RGBColor(const Color& c)
		{
			Color clamped = c.clamped();

			r = uint8_t(clamped.r * 255);
			g = uint8_t(clamped.g * 255);
			b = uint8_t(clamped.b * 255);
		}
	};
}


imaging::PPM::PPM()
	: m_frame_index(0)
{
	// NOP
}

imaging::PPM::~PPM()
{
	uint32_t datum = 0;
}

void imaging::PPM::write_text_ppm(const Bitmap & bitmap, std::ostream & out)
{
	LOG(INFO) << "Writing PPM frame #" << m_frame_index;

	int width = bitmap.width();
	int height = bitmap.height();
	int length = 0;
	out << "P3\n";
	out << width << " " << height << " 255 \n";

	for (unsigned j = 0; j != bitmap.height(); ++j)
	{
		for (unsigned i = 0; i != bitmap.width(); ++i)
		{

			RGBColor color(bitmap[Position2D(i, j)]);
			out << int(color.r) << " " << int(color.g) << " " << int(color.b) << " ";
		}
	}
	out << "\n";

	++m_frame_index;
}