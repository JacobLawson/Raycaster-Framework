#pragma once
#include <stdafx.h>
#include "rc_GameState.h"
#ifndef __GAMEPLAY_STATE_H__
#define __GAMEPLAY_STATE_H__

//forward declarations
class Console;
class Player;
class Level;
class TextureManager;

class GamePlayState : public GameState
{
public:
	GamePlayState();
	virtual ~GamePlayState();

	//define the virtual parent and implement them in the surce file
	virtual void Initialise(float a_fDT);
	virtual void Update(float a_fDT);
	virtual void Leave(float a_fDT);

	void SetTextureManager(TextureManager* a_texMan) { m_texMan = a_texMan; }
	void SetConsole(Console* a_console) { p_console = a_console; }
	virtual void Draw();

private:
	//pointers
	Console* p_console;
	Level* m_pLevel;
	Player* m_pPlayer;
	TextureManager* m_texMan;

	//width and height
	u32 w = 0, h = 0;
};
#endif // !__GAMEPLAY_STATE_H__

