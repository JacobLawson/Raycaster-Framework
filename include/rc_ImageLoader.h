#include <stdafx.h>
#pragma once

#ifndef __IMAGE_LOADER_H__
#define __IMAGE_LOADER_H__

//set enum of different types of images formats capable of being loaded 
typedef enum ImageType
{
	IM_BITMAP,
	IM_PCX,
	IM_PPM,

	IM_MAX
}ImageType;

class ImageLoader
{
public:
	ImageLoader() {};	//default constructor - no real functionality
	~ImageLoader() {};	//default destructor - no additional functionality

	//A Static function to load an image from a provided file
	// oth the returned image pixel data and the pallate data will need to be freed by the caller with delete[] operator

	/*!
		\param a_filename the file name and path of the image to load
		\param a_width a reference that will be used to store the width of the image in pixels
		\param a_height a reference that will be used to store the height of the image in pixels
		\param a_bpp a reference to store the number of bits per pixel for the image
		\param a_paletteData a referece pointer to the palette data that may be loaded with the image, 
			for not palleted images pass in nullptr
		\return returns a voud pointer to the image pixel data that has been extracted from the file
	*/
	static void* LoadFromFile(const char* a_filename, u32 a_imageType, u32& a_width, u32& a_height, u8& a_bpp, void*& a_paletteData);
};

#endif