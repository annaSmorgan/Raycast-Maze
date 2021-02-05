#pragma once
#ifndef _GAMEPLAY_STATE_H
#define _GAMEPLAY_STATE_H
//includes
#include <stdafx.h>
#include "GameState.h"
//included classes
class Player;
class Level;

class GamePlayState : public GameState
{
public:
	GamePlayState(); //constructor
	virtual ~GamePlayState(); //virtual deconstructor
	//define the virtual parent methods, to be implemented in source file
	virtual void Initialise(float a_fDT);
	virtual void Update(float a_fDT);
	virtual void Leave(float a_fDT);
	//draw functions
	void DrawBackground();
	virtual void Draw();
private:
	//class pointers
	Level* m_pLevel;
	Player* m_pPlayer;
};

#endif // !_GAMEPLAY_STATE_H
