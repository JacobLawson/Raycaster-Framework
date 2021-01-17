#include <stdafx.h>
#pragma once
#ifndef  __PPM_IMAGE_H__
#define  __PPM_IMAGE_H__



//Image Data (RGB)
template <typename T>
struct PPMPixel
{
	T R;
	T G;
	T B;
};

//Text Ascii
//Magic Number
//Possible Comment
//Width/ Height
//Max Colours
struct PPM
{
	u16 id;
	u8 comment[512];
	u32 width;
	u32 height;
	u32 maxColours;

	void* pixelData;
};

class PPMLoader
{
public:
	PPMLoader() {};
	~PPMLoader() {};

	static void* LoadFromFile(std::fstream* a_stream);
};
#endif