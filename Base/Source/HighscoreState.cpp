#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "menustate.h"
#include "highscorestate.h"

CHighscoreState CHighscoreState::theHighscoreState;

void CHighscoreState::Init()
{
	theScene = new SceneManagerLevel2DforScreen(800, 600,Highscorescreen);
	theScene->Init();
}

void CHighscoreState::Init(const int width, const int height)
{
	theScene = new SceneManagerLevel2DforScreen(width, height, Highscorescreen);
	theScene->Init();
}

void CHighscoreState::Cleanup()
{
	theScene->Exit();
	delete theScene;
	theScene = NULL;
}

void CHighscoreState::Pause()
{
}

void CHighscoreState::Resume(bool m_resume)
{
}

void CHighscoreState::HandleEvents(CGameStateManager* theGSM)
{
}

void CHighscoreState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
}

void CHighscoreState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
	const int button_Left, const int button_Middle, const int button_Right)
{
}

void CHighscoreState::Update(CGameStateManager* theGSM)
{
}

void CHighscoreState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	theScene->Update(m_dElapsedTime);
	if (theScene->ReturnScreenTransition() == false)
	{
		if (Application::IsKeyPressed(VK_BACK))
		{
			theScene->SetScreenTransition(true);
			theScene->SetChangeScreen(true);
		}
	}
	if (theScene->ReturnChangeScreen() && theScene->ReturnScreenTransition() == false)
	{
		theGSM->ChangeState(CMenuState::Instance());
	}
}

void CHighscoreState::Draw(CGameStateManager* theGSM)
{
	theScene->Render();
}
