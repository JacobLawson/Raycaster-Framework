#include <stdafx.h>
#include "rc_Texture.h"

#pragma once
#ifndef __LEVEL_H__
#define __LEVEL_H__

//Forward Declarations
class Player;
class Texture;
class TextureManager;

class Level
{
public:
	//Constructor / Destructor
	Level();
	~Level();

	//Getters for size of map and value of a map position
	void GetSize(u32& a_w, u32& a_h);
	u8 GetGridValue(u32 a_x, u32 a_y);

	//Load a level for gameplay to take place in
	bool LoadLevel(const char* a_filename);

	//Draw the level - Raycasting
	void draw(const Player* a_player);

	//set the texture manager
	void SetTextureManager(TextureManager* a_texMan) { m_texMan = a_texMan; }
private:
	//Pointers
	u8* m_map;
	Texture* m_texture;
	TextureManager* m_texMan;
	//variables
	u32 m_width;
	u32 m_height;
	u32 m_texWidth = 128;
	u32 m_texHeight = 128;


};
#endif // !__LEVEL_H__

