//includes
#include <stdafx.h>
#include "GameState.h"
#include "GameStateManager.h"
#include "rcImageLoader.h"
#include "renderer.h"
#include "player.h"
#include "GamePlayState.h"
#include "Level.h"
//constructor, initialsing variables and sets process to the initalise function
GamePlayState::GamePlayState() : GameState(), m_pLevel(nullptr), m_pPlayer(nullptr)
{
	Process = &GameState::Initialise;
}
//deconstructor
GamePlayState::~GamePlayState()
{
}

void GamePlayState::Initialise(float a_fDT)
{
	UNREFERENCED_PARAMETER(a_fDT);
	m_pLevel = new Level(); //instance of level class
	if (m_pLevel->LoadLevel("../resources/level/testMap.map")) //if map loaded
	{
		m_pPlayer = new Player(); // instance of player class

		m_isActive = true; // state is active 
		Process = &GameState::Update; //move the process to update function
	}
}

void GamePlayState::Update(float a_fDT)
{
	UNREFERENCED_PARAMETER(a_fDT);
	//process player movement
	m_pPlayer->Update(a_fDT);
}

void GamePlayState::Leave(float a_fDT)
{
	UNREFERENCED_PARAMETER(a_fDT);
	//inform game state manager to remove
	GetManager()->RemoveState(this);
}

void GamePlayState::Draw()
{
	//get the renderer pointer
	renderer* Renderer = renderer::GetInstance();
	//draw background
	unsigned int ceillingColour = 0xFFFFFFFF; //white
	unsigned int floorColour = 0xFFFFFFFF; //white
	Renderer->FillRenderBuffer(0, 0, 640, 320, ceillingColour); //sending colour to renderer buffer
	Renderer->FillRenderBuffer(0, 320, 640, 320, floorColour);//sending colour to renderer buffer
	//draw level
	m_pLevel->Draw(m_pPlayer);
}
