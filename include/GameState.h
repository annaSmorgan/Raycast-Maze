#pragma once
#ifndef _GAME_STATE_H
#define _GAME_STATE_H
//includes
#include <stdafx.h>
//included class
class GameStateManager;

class GameState
{
public:
	//constructor and deconstructor
	GameState();
	virtual ~GameState();
	//virutal functions
	virtual void Initialise(float a_fDT) = 0;
	virtual void Update(float a_fDT) = 0;
	virtual void Leave(float a_fDT) = 0;
	virtual void Draw();
	//getters and setter functions
	void SetStateName(const char* a_name);
	const char* GetStateName();
	void SetManager(GameStateManager* m_pManager);
	GameStateManager* GetManager() { return m_pManager; }

	bool isActive() { return m_isActive; } //checks if active
	void (GameState::* Process)(float); //pointer function

private:
	//variables
	char* m_Name;
	//pointer
	GameStateManager* m_pManager;

protected:
	//protected variables
	float m_fTimeInState;
	bool m_isActive;

};

#endif // !_GAME_STATE_H

