#include <stdafx.h>
#pragma once
#ifndef __RC_PLAYER_H__
#define __RC_PLAYER_H__

//Forward Declarations
class Console;
class Level;

class Player
{
public:
	//Constructor / Destructor
	Player();
	~Player();

	//Update the player object
	void Update(float a_fDT);
	
	//Getters and Setters
	//Pass Console to write player Data to console
	void SetConsole(Console* a_console) { p_console = a_console; }	

	//Position
	void SetPosition(float a_x, float a_y);
	void GetPosition(float& a_x, float& a_y) const;
	//Rotation
	void SetRotation(float a_degrees);
	void GetRotation(float& a_x, float& a_y) const;
	//Get Camera plane for raycast
	float GetNearPlaneLength() const { return m_nearPlaneLength; }
	//Exit Check
	void SetExit(u32 a_x, u32 a_y) { exitX = a_x; exitY = a_y; }
	void GetExit(u32 a_x, u32 a_y) { a_x = exitX; a_y = exitY; }
	//Pass Current Level so that level can be told to load the next map
	void SetLevel(Level* a_currentLevel) { p_currentLevel = a_currentLevel; }


private:
	//pointers
	Console* p_console;
	Level* p_currentLevel;

	//Positional variables
	float m_posX;
	float m_posY;
	float m_dirX;
	float m_dirY;

	float m_nearPlaneLength;

	//Level Exit Position
	u32 exitX;
	u32 exitY;
};

#endif // !__RC_PLAYER_H__
