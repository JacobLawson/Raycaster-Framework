#include <stdafx.h>
#include "rc_GameState.h"
#include "rc_GameStateManager.h"

GameStateManager::GameStateManager()
{
	//free up states on construction (should be only game state manager in game)
	m_pStates.clear();
	m_pStatesToFree.clear();
}

GameStateManager::~GameStateManager()
{
	//Remove all states from the the game state manager and frees them
	for (auto iter = m_pStatesToFree.begin(); iter != m_pStatesToFree.end(); ++iter)
	{
		GameState* pState = (*iter);
		delete pState;
		pState = nullptr;
	}
	m_pStatesToFree.clear();

	for (auto iter = m_pStates.begin(); iter != m_pStates.end(); ++iter)
	{
		GameState* pState = (*iter);
		delete pState;
		pState = nullptr;
	}
	m_pStates.clear();
}

//draw all active states
void GameStateManager::Draw()
{
	for (auto iter = m_pStates.begin(); iter != m_pStates.end(); ++iter)
	{
		//check if the state in the vector is active and if so Draw that state - don't want to draw inactive states
		if ((*iter)->isActive())
		{
			(*iter)->Draw();
		}
	}
}

void GameStateManager::Update(float a_fDT)
{
	if (m_pStatesToFree.size() > 0)
	{
		//free states that are ready to be freed
		for (auto iter = m_pStatesToFree.begin(); iter != m_pStatesToFree.end(); ++iter)
		{
			GameState* pState = (*iter);
			delete pState;
			pState = nullptr;
		}
		m_pStatesToFree.clear();
	}
	//itterate through the states currently in the stack and call theit process function
	for (auto iter = m_pStates.rbegin(); iter != m_pStates.rend(); ++iter)
	{
		//if we modify the states stack during an update we invalidate the iterator
		//best to bail out and start again next frame
		u32 PopStates = (u32)(m_pStatesToFree.size());
		//this is one of the neater ways of calling a member function pointer in C++
		void (GameState:: * func)(float) = (*iter)->Process;
		((*iter)->*func)(a_fDT);
		if (m_pStatesToFree.size() != PopStates)
		{
			break;
		}
	}
}

//Check to see if the state requested exists by itterating state stack
GameState* GameStateManager::StateExists(const char* a_pStateName)
{
	for (auto iter = m_pStates.begin(); iter != m_pStates.end(); ++iter)
	{
		GameState* pState = (*iter);
		const char* pName = pState->GetStateName();
		if (pName != nullptr && strcmp(pName, a_pStateName) == 0)
		{
			return pState;
		}
	}
	return nullptr;
}

//Push the state back
void GameStateManager::PushState(GameState* a_pState)
{
	if (a_pState)
	{
		m_pStates.push_back(a_pState);
	}
}

//pop the top off of the stack
void GameStateManager::PopState()
{
	if (m_pStates.size() > 0)
	{
		GameState* pLastState = m_pStates.back();
		m_pStatesToFree.push_back(pLastState);

		m_pStates.pop_back();
	}
}

//Pop states until requested state is top of stack
bool GameStateManager::PopToState(GameState* a_pState)
{
	//Loop through the states and count how many need to be popped
	int i = 0;
	bool stateFound = false;
	for (std::vector<GameState*>::reverse_iterator iter = m_pStates.rbegin(); iter != m_pStates.rend(); ++iter, ++i)
	{
		if ((*iter) == a_pState)
		{
			stateFound = true;
			break;
		}
	}

	if (stateFound)
	{
		//count down from then rnf and pop states off until j = 0
		for (int j = i; j > 0; --j)
		{	//get the last state and push it into the states to clear next frame
			GameState* pLastState = m_pStates.back();
			m_pStatesToFree.push_back(pLastState);
			m_pStates.pop_back();
		}
	}
	return stateFound;
}

//remove state and push it onto states to free stack to be freed next update
bool GameStateManager::RemoveState(GameState* a_pState)
{
	for (auto iter = m_pStates.begin(); iter != m_pStates.end(); ++iter)
	{
		GameState* pState = (*iter);
		if (pState == a_pState)
		{
			m_pStatesToFree.push_back(pState);
			m_pStates.erase(iter);
			return true;
		}
	}
	return false;
}