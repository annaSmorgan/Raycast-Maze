#pragma once
#ifndef  _SPLASH_STATE_H
#define _SPLASH_STATE_H
//includes
#include <stdafx.h>
#include "GameState.h"

class SplashState : public GameState //inherits from game state class
{
public:
	//constructors and deconstructors
	SplashState();
	virtual ~SplashState();
	//virtual parent method
	virtual void Initialise(float a_fDT);
	virtual void Update(float a_fDT);
	virtual void Leave(float a_fDT);
	virtual void Draw();

private:
	//variables
	void* m_backgroundImageData;
	u32 m_bgWidth;
	u32 m_bgHeight;

};

#endif // ! _SPLASH_STATE_H

