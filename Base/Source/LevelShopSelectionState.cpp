#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "LevelShopSelectionState.h"
#include "LevelSelectState.h"
#include "ShopState.h"
#include "menustate.h"

CLevelShopSelectionState CLevelShopSelectionState::theLevelShopSelectionState;

void CLevelShopSelectionState::Init()
{
	theScene = new SceneManagerLevel2DforScreen(800, 600, LevelShopSelectionscreen);
	theScene->Init();
	Select = 1;
	theScene->SetSelection(Select);
}

void CLevelShopSelectionState::Init(const int width, const int height)
{
	theScene = new SceneManagerLevel2DforScreen(width, height, LevelShopSelectionscreen);
	theScene->Init();
	Select = 1;
	theScene->SetSelection(Select);
}

void CLevelShopSelectionState::Cleanup()
{
	theScene->Exit();
	delete theScene;
	theScene = NULL;
}

void CLevelShopSelectionState::Pause()
{

}

void CLevelShopSelectionState::Resume(bool m_resume)
{

}

void CLevelShopSelectionState::HandleEvents(CGameStateManager* theGSM)
{}

void CLevelShopSelectionState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{}

void CLevelShopSelectionState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
	const int button_Left, const int button_Middle, const int button_Right)
{}

void CLevelShopSelectionState::Update(CGameStateManager* theGSM)
{
}

void CLevelShopSelectionState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	theScene->Update(m_dElapsedTime);
	if (theScene->ReturnScreenTransition() == false)
	{
		if (Application::IsKeyPressed(VK_DOWN))
		{
			if (Select < 2) // Max. Number of Options
			{
				Select++;	// Move the cursor down
				Sleep(150);
				cout << Select << endl;
			}
		}
		else if (Application::IsKeyPressed(VK_UP))
		{
			if (Select > 1) // Selection is not the first one.
			{
				Select--;
				Sleep(150);
				cout << Select << endl;
			}
		}
		if (Application::IsKeyPressed(VK_RETURN))
		{
			theScene->SetScreenTransition(true);
			theScene->SetChangeScreen(true);
		}

		if (Application::IsKeyPressed(VK_BACK))
		{
			Select = -1;
			theScene->SetScreenTransition(true);
			theScene->SetChangeScreen(true);
		}
	}
	if (theScene->ReturnChangeScreen() && theScene->ReturnScreenTransition() == false)
	{
		switch (Select)
		{
		case -1:
		{
				   theGSM->ChangeState(CMenuState::Instance());
				   break;
		}
		case 1:
		{
				  theGSM->ChangeState(CLevelSelectState::Instance());
				  break;
		}
		case 2:
		{
				  theGSM->ChangeState(CShopState::Instance());
				  break;
		}
		}
	}
	else
	{
		switch (Select)
		{
		case 1:
		{
				  theScene->SetSelection(Select);
				  break;
		}
		case 2:
		{
				  theScene->SetSelection(Select);
				  break;
		}
		}
	}
}

void CLevelShopSelectionState::Draw(CGameStateManager* theGSM)
{
	theScene->Render();
}
