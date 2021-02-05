//includes
#include <stdafx.h>
#include "SplashState.h"
#include "GameStateManager.h"
#include "rcImageLoader.h"
#include "renderer.h"
#include "GamePlayState.h"
//constructor initalising variables
SplashState::SplashState() : GameState(), m_backgroundImageData(nullptr), m_bgWidth(0), m_bgHeight(0)
{}
//deconstructor deleting then setting background image data to null if not null
SplashState::~SplashState()
{
	if (m_backgroundImageData)
	{
		delete[] m_backgroundImageData;
		m_backgroundImageData = nullptr;
	}
}
//initalising image and state
void SplashState::Initialise(float a_fDT)
{
	UNREFERENCED_PARAMETER(a_fDT);
	void* paletteData = nullptr; //set palette data to null
	u8 bpp; //creating bits per pixel variables
	//load background image
	m_backgroundImageData = ImageLoader::LoadFromFile("../resources/images/splashScreen.PCX",
		rc_imageType::IM_PCX, m_bgWidth, m_bgHeight, bpp, paletteData); //loading in the splash screen image
	m_isActive = true; //setting state as active
	m_fTimeInState = 0.0f; //setting the time within state to 0
	Process = &GameState::Update; //moving process to update
}

void SplashState::Update(float a_fDT)
{
	UNREFERENCED_PARAMETER(a_fDT);
	m_fTimeInState += a_fDT; //add delta time onto the time within state
	if (m_fTimeInState > 5.0f) //once time within state is more than 5 seconds
	{
		m_fTimeInState = 0.0f; 
		Process = &GameState::Leave; //move process to leave fucntion
	}
}

void SplashState::Leave(float a_fDT)
{
	UNREFERENCED_PARAMETER(a_fDT);
	//set up gameplay state
	GamePlayState* gpState = new GamePlayState();
	gpState->SetStateName("GamePlayState");
	gpState->SetManager(GetManager());
	//switching to gameplay
	GetManager()->PushState(gpState);
	//if leaving inform state manager to remove
	GetManager()->RemoveState(this);
}

void SplashState::Draw()
{
	//draw background image
	renderer* Renderer = renderer::GetInstance(); //instance of renderer
	if (m_backgroundImageData != nullptr)
	{//drawing splash screen image at pos 0, 0
		Renderer->FillRenderBuffer2(0, 0, m_bgWidth, m_bgHeight, m_backgroundImageData);
	}
}
