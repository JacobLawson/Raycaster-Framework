#include <stdafx.h>
#include "rc_SplashState.h"
#include "rc_GameStateManager.h"
#include "rc_ImageLoader.h"
#include "renderer.h"
#include "rc_GameplayState.h"
#include "rc_TextureManager.h"
#include "rc_Texture.h"

SplashState::SplashState() : GameState(), m_backgroundImageData(nullptr), m_bgWidth(0), m_bgHeight(0), m_texMan(nullptr), texture(nullptr), p_console(nullptr)
{
}

SplashState::~SplashState()
{
}

void SplashState::Initialise(float a_fDT)
{
	UNREFERENCED_PARAMETER(a_fDT);

	//make sure their is a texture manager to safely load textures
	if (m_texMan != nullptr)
	{
		//Load the splash screen texture into the texture manager
		m_texMan->LoadTexture("../resources/images/TitleJL.pcx", ImageType::IM_PCX);
	}
	m_isActive = true;
	m_fTimeInState = 0.f;
	Process = &GameState::Update;
}

void SplashState::Update(float a_fDT)
{
	UNREFERENCED_PARAMETER(a_fDT);

	//Show the splash screen for 5 seconds before entering leave state
	m_fTimeInState += a_fDT;
	if (m_fTimeInState > 5.f)
	{
		m_fTimeInState = 0.f;
		Process = &GameState::Leave;
	}
}

void SplashState::Draw()
{
	Renderer* renderer = Renderer::GetInstance();

	if (m_texMan != nullptr)
	{
		//Draw Texture to render buffer
		texture = m_texMan->GetTexture("../resources/images/TitleJL.pcx");
		u32 w, h;
		texture->GetDimensions(w, h);
		renderer->FillRenderBuffer(0, 0, w, h, texture->GetPixelData());
		renderer->Draw();
	}
}

void SplashState::Leave(float a_fDT)
{
	UNREFERENCED_PARAMETER(a_fDT);
	//unload the splash screen texture from the texture manager
	if (m_texMan != nullptr)
	{
		m_texMan->ReleaseTexture(texture);
	}
	//Setup gameplay state before moving to it
	GamePlayState* gpState = new GamePlayState();
	gpState->SetStateName("GamePlayState");
	gpState->SetConsole(p_console);
	gpState->SetManager(GetManager());
	gpState->SetTextureManager(m_texMan);
	//switch to gameplay state
	GetManager()->PushState(gpState);
	//inform the GameStateManager to remove this stare as it is no longer required
	GetManager()->RemoveState(this);
}


