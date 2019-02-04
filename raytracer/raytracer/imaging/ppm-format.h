#pragma once

#include "imaging/bitmap.h"
#include <string>
#include <ostream>
#include <fstream>

namespace imaging
{
	/// <summary>
	/// Class used to write PPM files.
	/// </summary>
	class PPM
	{
	public:
		/// <summary>
		/// Constructor.
		/// </summary>
		/// <param name="path">Path of PPM file</param>
		PPM();

		/// <summary>
		/// Destructor.
		/// </summary>
		~PPM();

		/// <summary>
		/// Writes a new frame to file.
		/// </summary>
		void write_text_ppm(const Bitmap& bitmap, std::ostream& out);

	private:
		unsigned m_frame_index;
	};
}