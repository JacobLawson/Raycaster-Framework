#include <stdafx.h>
#include "rc_GameState.h"
#pragma once
#ifndef __SPLASH_STATE__
#define __SPLASH_STATE__

//forward Declarations
class TextureManager;
class Texture;
class Console;

class SplashState : public GameState
{
public:
	//Constructor / Destructor
	SplashState();
	virtual ~SplashState();

	//define the virtual parent and implement them in the surce file
	virtual void Initialise(float a_fDT);
	virtual void Update(float a_fDT);
	virtual void Leave(float a_fDT);

	//Set pointers
	void SetTextureManager(TextureManager* a_texMan) { m_texMan = a_texMan; }
	void SetConsole(Console* a_console) { p_console = a_console; }

	//Draw the splash screen
	virtual void Draw();

private:
	//Pointers
	void* m_backgroundImageData;
	TextureManager* m_texMan;
	Console* p_console;
	Texture* texture;
	//Width and Height of Splash screen image
	u32 m_bgWidth;
	u32 m_bgHeight;
};
#endif // !__SPLASH_STATE__

