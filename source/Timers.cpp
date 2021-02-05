//includes
#include <stdafx.h>
#include "Timers.h"
//setting up variables
static std::chrono::time_point<std::chrono::system_clock> s_prevTime;
static float s_totalTime;
static float s_deltaTime;

void Timer::resetTimer()
{
	//setting all timers back to 0
	s_prevTime = std::chrono::system_clock::now(); //previous to current as thats its start time
	s_totalTime = 0.0f;
	s_deltaTime = 0.0f;
}

float Timer::tickTimer()
{
	auto currentTime = std::chrono::system_clock::now();
	std::chrono::duration<float> tstep = currentTime - s_prevTime; //step is the difference between current and previous
	s_deltaTime = tstep.count();//count the steps to get difference between frames
	s_totalTime += s_deltaTime; //total time increased by delta 
	s_prevTime = currentTime; //previous is now current

	return s_deltaTime;//return difference between current and previous frame
}
//gets the current delta time 
float Timer::getDeltaTime()
{
	return s_deltaTime; 
}
//gets the current total time
float Timer::getTotalTime()
{
	return s_totalTime;
}
