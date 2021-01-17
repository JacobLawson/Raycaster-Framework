#pragma once
#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include <stdafx.h>

//forward declaration for a texture class
class Texture;

class TextureManager
{
public:
	//Constructor / Destructor
	TextureManager();
	~TextureManager();

	//Function to test if a texture exists in memory
	bool TextureExists(const char* a_fileName);
	//Load a texture from a file
	Texture* LoadTexture(const char* a_fileName, u32 a_format);
	Texture* GetTexture(const char* a_fileName);

	//Release texture from the texture map
	void ReleaseTexture(Texture* a_texturePointer);

private:
	//a small structure to reference count a texture
	//reference count indicates how many pointers are
	//currently pointing to this texture
	typedef struct TextureRef
	{
		Texture* pTexture;
		unsigned int refCount;
	}TextureRef;
	std::map<std::string, TextureRef> m_pTextureMap;
};

#endif