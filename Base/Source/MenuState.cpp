#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "highscorestate.h"
#include "optionstate.h"
//#include "playstate.h"
#include "LevelShopSelectionState.h"
#include "gamestate.h"
#include "menustate.h"
#include "instructionstate.h"

CMenuState CMenuState::theMenuState;

void CMenuState::Init()
{
	theScene = new SceneManagerLevel2DforScreen(800, 600, Menuscreen);
	theScene->Init();
	Select = 1;
	theScene->SetSelection(Select);
}

void CMenuState::Init(const int width, const int height)
{
	theScene = new SceneManagerLevel2DforScreen(width, height, Menuscreen);
	theScene->Init();
	Select = 1;
	theScene->SetSelection(Select);
}

void CMenuState::Cleanup()
{
	theScene->Exit();
	delete theScene;
	theScene = NULL;
}

void CMenuState::Pause()
{
}

void CMenuState::Resume(bool m_resume)
{
}

void CMenuState::HandleEvents(CGameStateManager* theGSM)
{
}

void CMenuState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
}

void CMenuState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
	const int button_Left, const int button_Middle, const int button_Right)
{
}

void CMenuState::Update(CGameStateManager* theGSM)
{
	
}

void CMenuState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	theScene->Update(m_dElapsedTime);
	if (theScene->ReturnScreenTransition() == false)
	{
		if (Application::IsKeyPressed(VK_DOWN))
		{
			if (Select < 5) // Max. Number of Options
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
	}
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
	case 3:
	{
			  theScene->SetSelection(Select);
			  break;
	}
	case 4:
	{
			  theScene->SetSelection(Select);
			  break;

	}
	case 5:
	{
			  theScene->SetSelection(Select);
			  break;
	}
	}



	if (Application::IsKeyPressed(VK_RETURN))
	{
		if (Select != 5)
		{
			theScene->SetScreenTransition(true);
			theScene->SetChangeScreen(true);
		}
		if (Select == 5)
		{
			theGSM->Quit();
		}
	}

	if (theScene->ReturnChangeScreen() && theScene->ReturnScreenTransition() == false)
	{
		switch (Select)
		{
		case 1:
		{
				  theGSM->ChangeState(CLevelShopSelectionState::Instance());
				  break;
		}
		case 2:
		{
				  theGSM->ChangeState(CInstructionState::Instance());
				  break;
		}
		case 3:
		{
				  theGSM->ChangeState(CHighscoreState::Instance());
				  break;
		}
		case 4:
		{
				  theGSM->ChangeState(COptionState::Instance());
				  break;
		}
		}
	}
}

void CMenuState::Draw(CGameStateManager* theGSM)
{
	theScene->Render();
}
