#include <stdafx.h>
#include "rc_ImageLoader.h"
#include "rc_PCXLoader.h"
#include "rc_PPMLoader.h"

void* ImageLoader::LoadFromFile(const char* a_filename, u32 a_imageType, u32& a_width, u32& a_height, u8& a_bpp, void*& a_paletteData)
{
	//get an fstream to read in the file data
	std::fstream file;
	file.open(a_filename, std::ios_base::in | std::ios_base::binary);
	//test to see if the file has opended correctly
	if (file.is_open())
	{
		//success file has been opened. now verify the files contents
		file.ignore(std::numeric_limits<std::streamsize>::max());	//attempt to read highest number of bytes from file
		std::streamsize fileSize = file.gcount();					//gcount will have reached EOF marker letting us know total number of bytes
		file.clear();												//clear EOF marker from being read
		file.seekg(0, std::ios_base::beg);							//seek back to beginning of file
		if (fileSize == 0)											//if the file has no data close the file and return early a nullptr
		{
			file.close();
			return nullptr;
		}

		//set pointer to the image data when loaded so that it can refrence many imageData types
		void* imageData = nullptr;
		//switch statement to choose which custom image loader to call
		switch (a_imageType)
		{
		case(IM_BITMAP):{	//to do
			break;
		}
					   //Load PCX image
		case(IM_PCX): {
			imageData = PCXLoader::LoadFromFile(&file, a_width, a_height, a_bpp, a_paletteData);
			if (a_bpp != 32)
			{
				imageData = PCXLoader::ConvertTo32bpp(imageData, a_paletteData, a_width, a_height, a_bpp);
			}
			break;
		}
					//Load PPM Image
		case(IM_PPM): {
			imageData = PPMLoader::LoadFromFile(&file);
		}
		default:
			break;
		}
		file.close();
		return imageData;
	}
	return nullptr;
}
