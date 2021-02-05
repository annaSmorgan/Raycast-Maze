//includes
#include <stdafx.h>
#include "GameState.h"
#include "GameStateManager.h"
//constructor clears stacks of the states
GameStateManager::GameStateManager()
{
	m_pStates.clear();
	m_pStatesToFree.clear();
}
//deconstructor that iterates through stacks deleting and setting to null pointer
GameStateManager::~GameStateManager()
{//iterate from start to end of stack
	for (auto iter = m_pStatesToFree.begin(); iter != m_pStatesToFree.end(); ++iter)
	{
		GameState* pState = (*iter); // current state = postion in loop
		delete pState; // delete current state
		pState = nullptr; // set to null pointer
	}
	m_pStatesToFree.clear(); // clears states to be free stack
	//iterate from start to end of stack
	for (auto iter = m_pStates.begin(); iter != m_pStates.end(); ++iter)
	{
		GameState* pState = (*iter);// current state = postion in loop
		delete pState;// delete current state
		pState = nullptr; // set to null pointer
	}
	m_pStates.clear();//clears state stack
}

GameStateManager* GameStateManager::m_instance = nullptr; // set pointer instance to null pointer
//create instance of class
GameStateManager* GameStateManager::CreateInstance() 
{
	if (m_instance == nullptr) // if null pointer create new instance
	{
		m_instance = new GameStateManager();
	}
	return m_instance;
}
//destroys instance of class
void GameStateManager::DestroyInstance()
{
	if (m_instance != nullptr)//if not null pointer delete instance and set to null pointer
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

void GameStateManager::Update(float a_fDT)
{
	if (m_pStatesToFree.size() > 0) // if theres more than 0 states to free 
	{
		//looks for states to free from start to end in stack
		for (auto iter = m_pStatesToFree.begin(); iter != m_pStatesToFree.end(); ++iter)
		{
			GameState* pState = (*iter); //current state is the state in the current loop postion
			delete pState; //delete state
			pState = nullptr; // set to null pointer
		}
		m_pStatesToFree.clear(); // clear stack
	}
	//iterate through states from start to end
	for (auto iter = m_pStates.rbegin(); iter != m_pStates.rend(); ++iter)
	{
		//bail out if modified stack
		u32 popStates = (u32)(m_pStatesToFree.size()); // create an int the size of stack
		//calls member function pointer
		void(GameState:: * func)(float) = (*iter)->Process;
		((*iter)->*func)(a_fDT);
		if (m_pStatesToFree.size() != popStates) // if stack not equal to correct size
		{
			break;
		}
	}
}

void GameStateManager::Draw()
{//iterate through states stack from start to end
	for (auto iter = m_pStates.begin(); iter != m_pStates.end(); ++iter)
	{
		if ((*iter)->isActive()) // if current state is active
		{
			(*iter)->Draw(); //draw this state
		}
	}
}

bool GameStateManager::RemoveState(GameState* a_pState)
{//iterate through states stack from start to end
	for (auto iter = m_pStates.begin(); iter != m_pStates.end(); ++iter)
	{
		GameState* pState = (*iter);//current state is the state in the current loop postion
		if (pState == a_pState)//if state = called state
		{
			m_pStatesToFree.push_back(pState);//push to state to free stack
			m_pStates.erase(iter); //erase postion in loop
			return true;//then was removed
		}
	}
	return false;//otherwise wasnt removed
}

bool GameStateManager::PopToState(GameState* a_pState)
{
	//counting how many states need to be popped
	int i = 0;
	bool stateFound = false;
	//loop through states stack in reverse
	for (std::vector<GameState*>::reverse_iterator iter = m_pStates.rbegin(); iter != m_pStates.rend(); ++iter, ++i)
	{
		if ((*iter) == a_pState) //if current postion state is called state
		{
			stateFound = true; //state was found
			break;
		}
	}

	if (stateFound)
	{
		//counts down while popping states until j = 0
		for (int j = i; j > 0; --j)
		{
			//push last state to states to clear next frame
			GameState* pLastState = m_pStates.back(); //pointer to last state
			m_pStatesToFree.push_back(pLastState);//push to state to free stack
			m_pStates.pop_back();//pop state
		}
	}
	return stateFound;
}

void GameStateManager::PushState(GameState* a_pState)
{
	if (a_pState)//if state called
	{
		m_pStates.push_back(a_pState); //push to states list
	}
}

void GameStateManager::PopState()
{
	if (m_pStates.size() > 0)//if state stack is above 0
	{
		GameState* pLastState = m_pStates.back(); //create pointer to last state
		m_pStatesToFree.push_back(pLastState); //push to states to free

		m_pStates.pop_back(); //pop state
	}
}

GameState* GameStateManager::StateExists(const char* a_pName)
{//loop from begining to end of state stack
	for (auto iter = m_pStates.begin(); iter != m_pStates.end(); ++iter)
	{
		GameState* pState = (*iter); //state = current loop postion state 
		const char* pName = pState->GetStateName(); //set the name of state to pName
		if (pName != nullptr && strcmp(pName, a_pName) == 0) //if not null and if both strings are equal
		{
			return pState;
		}
	}

	return nullptr;
}
