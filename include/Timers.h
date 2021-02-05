#pragma once
#ifndef _TIMERS_H
#define _TIMERS_H
//includes
#include <stdafx.h>

class Timer
{
public: 
	//utilities for timing
	static void resetTimer();
	static float tickTimer();
	//return time and return time since last
	static float getDeltaTime();
	static float getTotalTime();
};

#endif // !_TIMERS_H

