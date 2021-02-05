//includes
#include <stdafx.h>
#include "GameState.h"
#include "GameStateManager.h"
//constructor, initialsing variables
GameState::GameState() : m_Name(nullptr), m_fTimeInState(0.0f), m_isActive(false)
{
	//set the process pointer function to point at initialise
	Process = &GameState::Initialise;
}
//deconstructor that deletes name and sets it to null pointer
GameState::~GameState()
{
	if (m_Name != nullptr)
	{
		delete[] m_Name;
		m_Name = nullptr;
	}
}

void GameState::Draw() //empty draw function
{
}
//setting the states name
void GameState::SetStateName(const char* a_name)
{
	if (m_Name == nullptr)
	{
		u64 l = (u64)strlen(a_name);
		//l+1 is null terminator
		m_Name = new char[l + 1];
		strcpy(m_Name, a_name);
	}
}
//get the name of state
const char* GameState::GetStateName()
{
	return m_Name;
}
//setting the manager
void GameState::SetManager(GameStateManager* a_pManager)
{
	m_pManager = a_pManager;
}
