#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "Winstate.h"
#include "menustate.h"

CWinState CWinState::theWinState;

void CWinState::Init()
{
	theScene = new SceneManagerLevel2DforScreen(800, 600, Winscreen);
	theScene->Init();
}

void CWinState::Init(const int width, const int height)
{
	theScene = new SceneManagerLevel2DforScreen(width, height, Winscreen);
	theScene->Init();
}

void CWinState::Cleanup()
{
	theScene->Exit();
	delete theScene;
	theScene = NULL;
}

void CWinState::Pause()
{

}

void CWinState::Resume(bool m_resume)
{

}

void CWinState::HandleEvents(CGameStateManager* theGSM)
{}

void CWinState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{}

void CWinState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
	const int button_Left, const int button_Middle, const int button_Right)
{}

void CWinState::Update(CGameStateManager* theGSM)
{
}

void CWinState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
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

void CWinState::Draw(CGameStateManager* theGSM)
{
	theScene->Render();
}
