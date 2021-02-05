#pragma once
#ifndef  _PLAYER_
#define _PLAYER_
//includes
#include <stdafx.h>

class Player
{
public:
	Player(); //constructor
	~Player(); //deconstructor
	//update function
	void Update(float a_fDT);
	//getter and setter function
	void SetPosition(float a_x, float a_y);
	void GetPosition(float& a_x, float& a_y) const;
	void SetRotation(float a_degree);
	void GetRotation(float& a_x, float& a_y) const;
	float GetNearPlaneLength() const { return m_nearPlaneLength; }

private:
	//variables
	float m_posX;
	float m_posY;
	float m_dirX;
	float m_dirY;
	float m_nearPlaneLength;
};

#endif //! _PLAYER_