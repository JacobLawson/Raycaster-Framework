#include <stdafx.h>
#include "rc_Level.h"
#include "Renderer.h"
#include "rc_Player.h"
#include "rc_Texture.h"
#include "rc_TextureManager.h"
#include "rc_ImageLoader.h"
#include "rc_PCXLoader.h"

Level::Level() : m_width(0), m_height(0), m_map(nullptr),m_texMan(nullptr), m_texture(nullptr)
{

}

Level::~Level()
{
	//free map pointer
	if (m_map != nullptr)
	{
		delete[] m_map;
		m_map = nullptr;
	}
}

void Level::GetSize(u32& a_w, u32& a_h)
{
	a_w = m_width;
	a_h = m_height;
}

//Get the value of a specific map tile
u8 Level::GetGridValue(u32 a_x, u32 a_y)
{
	u32 index = a_x + (a_y * m_width);
	if (index < (m_width * m_height))
	{
		return m_map[index];
	}
	return 255; //indicates map out of bounds error
}

bool Level::LoadLevel(const char* a_filename)
{
	//Load Textures used for drawing the map
	m_texMan->LoadTexture("../resources/images/WallJL.PCX", ImageType::IM_PCX);
	m_texMan->LoadTexture("../resources/images/StoneJL.PCX", ImageType::IM_PCX);
	m_texMan->LoadTexture("../resources/images/OvergrownJL.PCX", ImageType::IM_PCX);
	m_texMan->LoadTexture("../resources/images/GoalJL.PCX", ImageType::IM_PCX);

	//If Level is not null then delete it to free up memory
	if (m_map != nullptr)
	{	
		delete[] m_map;
		m_map = nullptr;
	}

	//open the Level map from file
	std::fstream levelFile;
	levelFile.open(a_filename, std::ios_base::in | std::ios_base::binary);
	//test to see if the file has opened correctly
	if (levelFile.is_open())
	{
		levelFile.ignore(std::numeric_limits<std::streamsize>::max());
		std::streamsize fileSize = levelFile.gcount();
		levelFile.clear();
		levelFile.seekg(0, std::ios_base::beg);
		if (fileSize == 0)
		{
			levelFile.close();
			return false;
		}
		//read in the width and Height of the level map
		levelFile >> m_width >> m_height;
		levelFile.ignore(1); //ignore end of line marker
		//allocate memory to hold all map data
		m_map = new u8[m_width * m_height];
		u32 index = 0;
		//read each line of the file to get one row worth of map data
		//use getline to read in each line of the file
		for (std::string currentMapLine; std::getline(levelFile, currentMapLine);)
		{
			//convert string from getline into stringstram and use comma separator to break into chuncks and store values in map
			std::istringstream ss{ currentMapLine };
			int val = 0;
			while (ss >> val)
			{	//whilst there is still data to read
				//store int value temporarily then static cast to u8 otherwise we read vaues as char
				m_map[index] = (u8)(val);
				++index;
				if ((ss >> std::ws).peek() == ',') // std::ws ignores the white space and commas
				{
					ss.ignore();
				}
			}
		}
		//close the file
		levelFile.close();
		return true;
	}
	return false;
}

//the level knows how to draw itself
void Level::draw(const Player* a_player)
{
	//get the instance of the renderer to get the dimensions of the render window
	Renderer* renderer = Renderer::GetInstance();
	u32 windowWidth = 0, windowHeight = 0;
	renderer->GetWindowSize(windowWidth, windowHeight);

	//from the passed in pointer to the player get the current position and the direction of the player
	float playerPosX = 0.f, playerPosY = 0.f;
	a_player->GetPosition(playerPosX, playerPosY);
	float playerDirX = 0.f, playerDirY = 0.f;
	a_player->GetRotation(playerDirX, playerDirY);
	float nearPlaneLength = a_player->GetNearPlaneLength();

	//Camera plane is perpendicular to player direction
	//swap components and negate y
	//multiply by near plane length to get vector to right length
	float camPlaneX = -playerDirY * nearPlaneLength;
	float camPlaneY = playerDirX * nearPlaneLength;

	//Ray casting
	for (u32 c = 0; c < windowWidth; c++) //c represents which screen column it is examining
	{
		//get the x position as a value between -1 & 1
		//-1 = Left of center, 1 = right of center
		float cameraX = ((2.f * c) / (float)windowWidth) - 1.f;
		float rayDirX = playerDirX + (camPlaneX * cameraX);
		float rayDirY = playerDirY + (camPlaneY * cameraX);

		//Length of ray from one X/Y side to the next X/Y Side
		float deltaDistX = (rayDirX != 0.f) ? std::abs(1.f / rayDirX) : 0;
		float deltaDistY = (rayDirY != 0.f) ? std::abs(1.f / rayDirY) : 0;

		//players current Grid Position
		int mapX = (int)(playerPosX);
		int mapY = (int)(playerPosY);

		//Length of ray from current position to next x/y sidd
		float sideDistX = 0.f;
		float sideDistY = 0.f;

		float perpWallDist = 0.f;

		//direction to step x/y +-1
		int stepX = 0;
		int stepY = 0;

		int collision = 0;
		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (playerPosX - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.f - playerPosX) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (playerPosY - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.f - playerPosY) * deltaDistY;
		}

		int yIntersection = 0;
		while (collision == 0)
		{ //jump to the next map square in x or y direction
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				yIntersection = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				yIntersection = 1;
			}
			//test current map location for a collision (tile values that are non zero indicate a wall or non empty tile)
			collision = m_map[(mapX + (mapY * m_width))];
		}
		//we have collided out raycasy with a wall
		//Get perpendicular distance to wall from the player position
		perpWallDist = (!yIntersection) ?
			(mapX - playerPosX + (1 - stepX) / 2.f) / rayDirX :
			(mapY - playerPosY + (1 - stepY) / 2.f) / rayDirY;

		//calculate height of line to draw
		s32 lineHeight = (int)(windowHeight / perpWallDist);

		//calculate upper and lower points to draw
		u32 StartofDraw = -lineHeight / 2 + windowHeight / 2;
		if (StartofDraw < 0)
		{
			StartofDraw = 0;
		}
		u32 EndofDraw = lineHeight / 2 + windowHeight / 2;
		if (EndofDraw >= windowHeight)
		{
			EndofDraw = windowHeight - 1;
		}

		//Choose a wall texture to draw based on the value of the map position they raycaster has collided with
		if (collision == 1)
		{
			m_texture = m_texMan->GetTexture("../resources/images/WallJL.PCX");
		}
		else if (collision == 2)
		{
			m_texture = m_texMan->GetTexture("../resources/images/StoneJL.PCX");
		}
		else if (collision == 3)
		{
			m_texture = m_texMan->GetTexture("../resources/images/OvergrownJL.PCX");
		}
		else if (collision == 4)
		{
			m_texture = m_texMan->GetTexture("../resources/images/GoalJL.PCX");
		}

		//Get the width and height of the texture
		u32 texW = 0;
		u32 texH = 0;
		m_texture->GetDimensions(texW, texH);

		//Calculate the exact position the wall was hit
		double wallX;
		if (yIntersection == 0)
		{
			wallX = playerPosY + perpWallDist * rayDirY;
		}
		else
		{
			wallX = playerPosX + perpWallDist * rayDirX;
		}
		wallX -= floor((wallX));

		//x coordinate of the texture to draw
		int texX = int(wallX * double(texW));
		if (yIntersection == 0 && rayDirX > 0)
		{
			texX = texW - texX - 1;
		}
		if (yIntersection == 1 && rayDirY < 0)
		{
			texX = texW - texX - 1;
		}

			//How much to increase the texture coordinate per screen pixel
			//adjusting step also seems to scale texture on wall
			double step = 1.0 * texH / lineHeight;
			//The starting texture coordinate to draw
			unsigned int* rawTextureData = (unsigned int*)m_texture->GetPixelData();
			double texPos = (StartofDraw - windowHeight / 2 + lineHeight / 2) * step;
			//loop to fill textures columns of image data into render buffer 
			for (u32 y = StartofDraw; y < EndofDraw; y++)
			{
				// Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
				int texY = (int)texPos & (texH - 1);
				texPos += step;
				u32 data = rawTextureData[texW * texY + texX];
				//make the colour darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
				//this makes the sides of walls darker and improves perspective
				if (yIntersection == 1)
				{
					data = (data >> 1) & 8355711;
				}
				//fill render buffer
				renderer->FillRenderBuffer(c, y, 1, 1, data);
			}
	}
}
