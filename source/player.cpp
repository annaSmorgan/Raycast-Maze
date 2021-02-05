//includes
#include <stdafx.h>
#include "inputManager.h"
#include "player.h"
//constructor that initalises the players starting positon and direction
Player::Player() : m_dirX(1.0f), m_dirY(0.0f), m_posX(2.0f), m_posY(2.0f)
{
	//FOV calc and getting near plane
	float nearPlaneDist = 1.0f;
	m_nearPlaneLength = tanf(HALF_FOV * (float)(PI / 180.0f)) * nearPlaneDist;
}
//deconstructor
Player::~Player()
{
}

void Player::Update(float a_fDT)
{
	//process player movement 
	if (inputManager::GetInstance()->GetKeyDown('W')) //checks for W being pressed
	{
		m_posX += 2.f * m_dirX * a_fDT; //move position in X direction by 2 * delta time
		m_posY += 2.f * m_dirY * a_fDT;//move position in Y direction by 2 * delta time
	}
	if (inputManager::GetInstance()->GetKeyDown('S')) //checks for S being pressed
	{
		m_posX -= 2.f * m_dirX * a_fDT;//move position in X direction by -2 * delta time
		m_posY -= 2.f * m_dirY * a_fDT;//move position in Y direction by -2 * delta time
	}
	//player rotation
	if (inputManager::GetInstance()->GetKeyDown('A')) //checks for A being pressed
	{
		//camera direction and camera plane must be rotated 
		float playerRotSpeed = -1.5f * a_fDT;
		float oldDirX = m_dirX;
		m_dirX = oldDirX * cosf(playerRotSpeed) - m_dirY * sin(playerRotSpeed);
		m_dirY = oldDirX * sin(playerRotSpeed) + m_dirY * cos(playerRotSpeed);
	}
	if (inputManager::GetInstance()->GetKeyDown('D')) //checks for D being pressed
	{
		//camera direction and camera plane must be rotated 
		float playerRotSpeed = 1.5f * a_fDT;
		float oldDirX = m_dirX;
		m_dirX = oldDirX * cosf(playerRotSpeed) - m_dirY * sin(playerRotSpeed);
		m_dirY = oldDirX * sin(playerRotSpeed) + m_dirY * cos(playerRotSpeed);
	}
}
//setting position of player X and Y
void Player::SetPosition(float a_x, float a_y)
{
	m_posX = a_x; m_posY = a_y;
}
//getting position of player X and Y
void Player::GetPosition(float& a_x, float& a_y) const
{
	a_x = m_posX; a_y = m_posY;
}
//setting direction of player X and Y
void Player::SetRotation(float a_degree)
{
	float radians = a_degree * (float)(PI / 180.0f);
	m_dirX = 1.0f * cosf(radians);
	m_dirY = 1.0f * sin(radians);
}
//getting direction of player X and Y
void Player::GetRotation(float& a_x, float& a_y) const
{
	a_x = m_dirX; a_y = m_dirY;
}

