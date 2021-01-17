#include <stdafx.h>
#include "rc_GameState.h"
#include "rc_GameStateManager.h"

GameState::GameState() : m_Name(nullptr), m_fTimeInState(0.f), m_isActive(false), m_pManager(nullptr)
{
	//set the current process function pointer to point to initialise
	Process = &GameState::Initialise;
}

GameState::~GameState()
{
	//destroy state
	if (m_Name != nullptr)
	{
		delete[] m_Name;
		m_Name = nullptr;
	}
}

//set the name of the state
void GameState::SetStateName(const char* a_pStateName)
{
	if (m_Name == nullptr)
	{
		u64 l = (u64)strlen(a_pStateName);
		//allocate l+1 as null terminator is required
		m_Name = new char[l + 1];
		//set name of state from the name passed
		strcpy(m_Name, a_pStateName);
	}
}

//Get the Name of the State
const char* GameState::GetStateName()
{
	return m_Name;
}

//set the game state manager
void GameState::SetManager(GameStateManager* a_pManager)
{
	m_pManager = a_pManager;
}

//Empty draw function (overided by child states)
void GameState::Draw()
{
}