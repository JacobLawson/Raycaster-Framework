#include <stdafx.h>
#include <rc_Texture.h>
#include <rc_ImageLoader.h>

//initialise texture
Texture::Texture() :
	m_filename(""), m_width(0), m_height(0), m_bpp(0),
	m_paletteSize(0), m_palette(nullptr), m_pixelData(__nullptr)
{
}

Texture::~Texture() 
{
	//If these are not null then an image was loaded and must be deleted
	if (m_palette) 
	{ 
		delete[] m_palette;
		m_palette = nullptr;
	}
	if (m_pixelData)
	{
		delete[] m_pixelData;
		m_pixelData = nullptr;
	}
}

//load image into texture as a pointer and set the properties fo the texture (width, height)
bool Texture::Load(const char* a_filename, u32 a_format)
{
	m_pixelData =
		ImageLoader::LoadFromFile(a_filename, a_format, 
			m_width, m_height, m_bpp, m_palette);
	//check if texture is valid
	if (m_pixelData)
	{
		m_filename = a_filename;
		return true;
	}
	//texture pixel data isn't valid to set to 0 and return false
	m_width = m_height = 0;
	m_bpp = 0;
	m_paletteSize = 0;
	return false;

}

//get dimensions of texture image
void Texture::GetDimensions(u32& a_Width, u32& a_Height) const
{
	a_Width = m_width;
	a_Height = m_height;
}

//get palette of texture image
u16 Texture::GetPalette(void* a_palette)
{
	a_palette = m_palette;
	return m_paletteSize;
}