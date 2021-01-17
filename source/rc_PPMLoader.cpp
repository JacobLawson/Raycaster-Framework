
#include <stdafx.h>
#include "rc_PCXLoader.h"
#include "..\include\rc_PPMLoader.h"

void* PPMLoader::LoadFromFile(std::fstream* a_stream)
{
	u16 id;
	PPM image;
	std::cout << "file Loaded" << std::endl;
	//read in first two bytes of file

	u8 imageData[512];
	memset(imageData, 0, 512);
	std::fstream* file = a_stream;	//get pointer to file stream
	//file->getline(imageData, 512);
	memcpy(&image.id, imageData, 2);
	memcpy(&id, imageData, 2);
	if (id != 13904)
	{
		std::cout << imageData << std::endl;
	}
	memset(imageData, 0, 512);
	//filegetline(imageData, 512);
	if (imageData[0] == '#')
	{

	}
	*file >> image.width >> image.height;
	*file >> image.maxColours;
	file->ignore(1);

	//create a PPM image object to store pixel data
	unsigned int pixelSize;
	if (image.maxColours > 255)
	{
		image.pixelData = new PPMPixel<unsigned short>[image.width * image.height];
		pixelSize = image.width * image.height * sizeof(PPMPixel<unsigned short>);
	}
	else
	{
		image.pixelData = new PPMPixel<unsigned char>[image.width * image.height];
		pixelSize = image.width * image.height * sizeof(PPMPixel<unsigned char>);
	}
	file->read((char*)image.pixelData, pixelSize);

	return image.pixelData;
}
