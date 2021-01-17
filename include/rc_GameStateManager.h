#include <stdafx.h>
#pragma once
#ifndef __GAME_STATE_MANAGER_H
#define __GAME_STATE_MANAGER_H

//forward declaration of game state class
class GameState;

class GameStateManager
{
protected:
public:
	//Constructor / Destructor
	GameStateManager();
	~GameStateManager();

	void Update(float a_FDT);
	void Draw();

	//state removal and addition to the state stack
	bool RemoveState(GameState* a_pState);	//Function to remove the state passed in from the state stack
	bool PopToState(GameState* a_pState);	//Function to pop all states from the stack until the state passed in is located
	void PushState(GameState* a_pState);	//Function to push the state passed in onto the state stack and make it the current state
	void PopState();	//Function to pop the current state from the state stack and make the next state the active state

	//check if state exists
	GameState* StateExists(const char* a_pStateName);

private:
	//A vector of states that are currently active/existing
	std::vector<GameState*> m_pStates;
	//When a state is popped from the stack it will get pushed onto the list of states to free in the next update.
	//immediately as it will possibly be getting updates still as it is freed
	std::vector<GameState*> m_pStatesToFree;
};
#endif // !__GAME_STATE_MANAGER_H


