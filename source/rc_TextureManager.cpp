#include <stdafx.h>
#include "rc_TextureManager.h"
#include "rc_Texture.h"

TextureManager::TextureManager() : m_pTextureMap() 
{
}

TextureManager::~TextureManager()
{
	m_pTextureMap.clear();
}

Texture* TextureManager::LoadTexture(const char* a_fileName, u32 a_format)
{
	Texture* pTexture = nullptr;
	if (a_fileName != nullptr)
	{
		std::map<std::string, TextureRef>::iterator dictionaryIter =
			m_pTextureMap.find(a_fileName);
		if (dictionaryIter != m_pTextureMap.end())
		{ //if iterator is not at end then texture was located
			TextureRef& texRef = (TextureRef&)(dictionaryIter->second);
			//get reference to texture ref struct
			pTexture = texRef.pTexture;
			++texRef.refCount;
		}
		else
		{
			//better load the texture in as it's not in the texture map yet
			pTexture = new Texture();
			if (pTexture->Load(a_fileName, a_format))
			{
				//Load successful place into texRef structure and insert into dictionary
				TextureRef texRef = { pTexture, 1 };
				m_pTextureMap[a_fileName] = texRef;
			}
			else
			{
				//Load failed free memory
				delete pTexture; 
				pTexture = nullptr;
			}
		}
	}
	return pTexture;
}

//Remove a texture from memory
//this will either unload the texture or simply decrement
//it's reference count until this reaches 0 and then unload
void TextureManager::ReleaseTexture(Texture* a_texturePointer)
{
	for (auto dictionaryIter = m_pTextureMap.begin();
		dictionaryIter != m_pTextureMap.end(); ++dictionaryIter)
	{
		TextureRef& texRef = (TextureRef&)(dictionaryIter->second);
		if (a_texturePointer == texRef.pTexture) //found the texture to remove
		{
			//pre decrement will be evaluated before call to <=
			//-- safety code could be seperated into two lines
			if (--texRef.refCount == 0)
			{
				delete texRef.pTexture;
				texRef.pTexture = nullptr;
				m_pTextureMap.erase(dictionaryIter);
				break;//escape the loop to return
			}
		}
	}

}

//check if texture exists within the Texture map
bool TextureManager::TextureExists(const char* a_fileName)
{
	auto dictionaryIter = m_pTextureMap.find(a_fileName);
	if (dictionaryIter != m_pTextureMap.end())
	{ //if iterator is not at end then texture was located
		return true;
	}
	return false;
}

//returns the texture in the texture map
Texture* TextureManager::GetTexture(const char* a_fileName)
{
	//Get blank Texture to be filled if texture found
	Texture* pTexture = nullptr;
	auto dictionaryIter = m_pTextureMap.find(a_fileName);
	if (dictionaryIter != m_pTextureMap.end())
	{ //if iterator is not at end then texture was located
		TextureRef& texRef = (TextureRef&)(dictionaryIter->second);
		//get reference to texture ref struct
		texRef.refCount++;
		//set blank texture with found texture pointer
		pTexture = texRef.pTexture;
	}
	return pTexture;
}