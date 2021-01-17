#include <stdafx.h>
#include "rc_InputManager.h"
#include "rc_GamePlayState.h"
#include "rc_Player.h"
#include "rc_Level.h"
#include "Console.h"

Player::Player() : m_dirX(1.f), m_dirY(0.f), m_posX(1.f), m_posY(3.f), p_currentLevel(nullptr), p_console(nullptr), exitX(0), exitY(0)
{
	float nearPlaneDistance = 1.f;
	m_nearPlaneLength = tanf(HALF_FOV * (float)(PI / 180.f)) * nearPlaneDistance;
}

Player::~Player()
{
}

void Player::SetPosition(float a_x, float a_y)
{
	m_posX = a_x; m_posY = a_y;
}

void Player::GetPosition(float& a_x, float& a_y) const
{
	a_x = m_posX; a_y = m_posY;
}

void Player::SetRotation(float a_degrees)
{
	float radians = a_degrees * (float)(PI / 180.f);
	m_dirX = 1.f * cosf(radians);
	m_dirY = 1.f * sinf(radians);
}

void Player::GetRotation(float& a_x, float& a_y) const
{
	a_x = m_dirX; a_y = m_dirY;
}

void Player::Update(float a_fDT)
{
	//process Player Movement
	if (InputManager::GetInstance()->GetKeyDown('W'))
	{
		m_posX += 2.f * m_dirX * a_fDT;
		m_posY += 2.f * m_dirY * a_fDT;
	}
	if (InputManager::GetInstance()->GetKeyDown('S'))
	{
		m_posX -= 2.f * m_dirX * a_fDT;
		m_posY -= 2.f * m_dirY * a_fDT;
	}
	if (InputManager::GetInstance()->GetKeyDown('A'))
	{
		//rotate player left
		float playerRotSpeed = -0.4f * a_fDT;
		float oldDirX = m_dirX;
		m_dirX = oldDirX * cosf(playerRotSpeed) - m_dirY * sin(playerRotSpeed);
		m_dirY = oldDirX * sin(playerRotSpeed) + m_dirY * cos(playerRotSpeed);
	}
	if (InputManager::GetInstance()->GetKeyDown('D'))
	{
		//rotate player right
		float playerRotSpeed = 0.4f * a_fDT;
		float oldDirX = m_dirX;
		m_dirX = oldDirX * cosf(playerRotSpeed) - m_dirY * sin(playerRotSpeed);
		m_dirY = oldDirX * sin(playerRotSpeed) + m_dirY * cos(playerRotSpeed);
	}

	//Level exit and loading new Level
	//Calculate the bottom right map tile that isn't a wall
	//exit can be any x or y coordinate however for this example I have set it to always the bottom left empty tile of the map loaded
	p_currentLevel->GetSize(exitX, exitY);
	exitX = exitX - 1;
	exitY = exitY - 1;
	//Round player position so that the so long as the player is within the tile space they will trigger the level change
	u32 roundedPosX = u32(ceil(m_posX));
	u32 roundedPosY = u32(ceil(m_posY));

	if (roundedPosX  == exitX && roundedPosY == exitY)
	{	//If within the exit tile load the new level and reset the player position
		p_currentLevel->LoadLevel("../resources/levels/TestMap.map");
		SetPosition(1.f, 3.f);
		p_currentLevel->draw(this);
	}

	//Writing to the Console
	////Buffer to hold position values
	wchar_t textBuffer[16];
	//get the position of the player
	unsigned int posX = unsigned int(m_posX);
	unsigned int posY = unsigned int(m_posY);
	//Write X and Y coords to console
	swprintf_s(textBuffer, 13, L"Player X:%3u", posX > 999 ? 999 : posX);
	p_console->WriteToLocation(textBuffer, 13, 0, 1);
	swprintf_s(textBuffer, 13, L"Player Y:%3u", posY > 999 ? 999 : posY);
	p_console->WriteToLocation(textBuffer, 13, 0, 2);
}
