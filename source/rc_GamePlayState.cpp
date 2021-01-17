#include <stdafx.h>
#include "rc_GamePlayState.h"
#include "rc_GameStateManager.h"
#include "rc_ImageLoader.h"
#include "Renderer.h"
#include "rc_Player.h"
#include "rc_level.h"

GamePlayState::GamePlayState() : GameState(), m_pLevel(nullptr), m_pPlayer(nullptr), m_texMan(nullptr), p_console(nullptr)
{
}

GamePlayState::~GamePlayState()
{
}

void GamePlayState::Initialise(float a_fDT)
{
	UNREFERENCED_PARAMETER(a_fDT);

	//Loand and intitialise the first level
	m_pLevel = new Level();
	//pass the texture manager into the new Level
	m_pLevel->SetTextureManager(m_texMan);
	//Load the level from directory
	if (m_pLevel->LoadLevel("../resources/levels/BigMap.map"))
	{
		//Initialise the player
		m_pPlayer = new Player;
		m_pPlayer->SetConsole(p_console);
		m_pPlayer->SetLevel(m_pLevel);
		m_isActive = true;
		//exit initialisation and start update the game
		Process = &GameState::Update;
	}
}

void GamePlayState::Update(float a_fDT)
{
	UNREFERENCED_PARAMETER(a_fDT);

	//Process player movement
	m_pPlayer->Update(a_fDT);
}

void GamePlayState::Draw()
{
	//Get Renderer pointer
	Renderer* renderer = Renderer::GetInstance();

	//draw Background (walls and ceiling
	renderer->GetWindowSize(w, h);
	unsigned int ceilingColour = 0x00505050;
	unsigned int floorColour = 0x00B2B2B2;
	renderer->FillRenderBuffer(0, 0, w, h>>1, ceilingColour);
	renderer->FillRenderBuffer(0, h>>1, w, h>>1, floorColour);

	//draw level
	m_pLevel->draw(m_pPlayer);
	renderer->Draw();
}

void GamePlayState::Leave(float a_fDT)
{
	UNREFERENCED_PARAMETER(a_fDT);

	//if we are leaving the srate inform the GameStateManager to remove us
	GetManager()->RemoveState(this);
}