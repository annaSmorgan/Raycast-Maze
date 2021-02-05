#pragma once
#ifndef _GAME_STATE_MANAGER_H
#define _GAME_STATE_MANAGER_H
//includes
#include <stdafx.h>
//included class
class GameState;

class GameStateManager
{
public:
	//Singleton accessor, retrives instance of class
	static GameStateManager* GetInstance() { return m_instance; }
	//Singleton creation, called to created an instance of class and return it
	static GameStateManager* CreateInstance();
	//destroy instance is used to remove instance of class
	static void DestroyInstance();

	//functions
	void Update(float a_fDT);
	void Draw();
	bool RemoveState(GameState* a_pState);
	bool PopToState(GameState* a_pState);
	void PushState(GameState* a_pState);
	void PopState();
	//pointer function
	GameState* StateExists(const char* a_pName);

private:
	//static instance of class
	static GameStateManager* m_instance;
	//constructor and destructor are private so cannot be called outside of instance
	GameStateManager();
	~GameStateManager();
	//vectors 
	//currently active/existing states
	std::vector<GameState*> m_pStates;
	//waiting to be freed states
	std::vector<GameState*> m_pStatesToFree;

};

#endif // !_GAME_STATE_MANAGER


