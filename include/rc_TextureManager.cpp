#include <stdafx.h>
#pragma once
#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

//forward declaration for a texture class
class Texture;

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	//Function to test if a texture exists in memory
	bool TextureExists(const char* a_pTextureName);
	//Load a texture from a file
	Texture* LoadTexture(const char* a_pTextureName, u32 a_format);
	Texture* GetTexture(Texture* a_texturePointer);

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