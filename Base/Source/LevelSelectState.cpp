#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "LevelShopSelectionState.h"
#include "LevelSelectState.h"
#include "menustate.h"

CLevelSelectState CLevelSelectState::theLevelSelectState;

void CLevelSelectState::Init()
{
	theScene = new SceneManagerLevel2DforScreen(800, 600, LevelSelectscreen);
	theScene->Init();
	Select = 1;
}

void CLevelSelectState::Init(const int width, const int height)
{
	theScene = new SceneManagerLevel2DforScreen(width, height, LevelSelectscreen);
	theScene->Init();
	Select = 1;
}

void CLevelSelectState::Cleanup()
{
	theScene->Exit();
	delete theScene;
	theScene = NULL;
}

void CLevelSelectState::Pause()
{

}

void CLevelSelectState::Resume(bool m_resume)
{

}

void CLevelSelectState::HandleEvents(CGameStateManager* theGSM)
{}

void CLevelSelectState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{}

void CLevelSelectState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
	const int button_Left, const int button_Middle, const int button_Right)
{}

void CLevelSelectState::Update(CGameStateManager* theGSM)
{
}

void CLevelSelectState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	theScene->Update(m_dElapsedTime);
	if (theScene->ReturnScreenTransition() == false)
	{
		if (Application::IsKeyPressed(VK_BACK))
		{
			theScene->SetScreenTransition(true);
			theScene->SetChangeScreen(true);
			Select = -1;
		}
		if (Application::IsKeyPressed(VK_RETURN))
		{
			theScene->SetScreenTransition(true);
			theScene->SetChangeScreen(true);
			Select = 1;
		}
	}
	if (theScene->ReturnChangeScreen() && theScene->ReturnScreenTransition() == false)
	{
		switch (Select)
		{
		case -1:
		{
				   theGSM->ChangeState(CLevelShopSelectionState::Instance());
				   break;
		}
		case 1:
		{
				  theGSM->ChangeState(CPlayState::Instance());
				  break;
		}
		}
	}
}

void CLevelSelectState::Draw(CGameStateManager* theGSM)
{
	theScene->Render();
}
