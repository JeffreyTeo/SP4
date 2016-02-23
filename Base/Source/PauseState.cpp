#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "Pausestate.h"
#include "menustate.h"

CPauseState CPauseState::thePauseState;

void CPauseState::Init()
{
	theScene = new SceneManagerLevel2DforScreen(800, 600,Pausescreen);
	theScene->Init();
	Select = 1;
	theScene->SetSelection(Select);
}

void CPauseState::Init(const int width, const int height)
{
	theScene = new SceneManagerLevel2DforScreen(width, height, Pausescreen);
	theScene->Init();
	Select = 1;
	theScene->SetSelection(Select);
}

void CPauseState::Cleanup()
{
	theScene->Exit();
	delete theScene;
	theScene = NULL;
}

void CPauseState::Pause()
{

}

void CPauseState::Resume(bool m_resume)
{

}

void CPauseState::HandleEvents(CGameStateManager* theGSM)
{}

void CPauseState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{}

void CPauseState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
	const int button_Left, const int button_Middle, const int button_Right)
{}

void CPauseState::Update(CGameStateManager* theGSM)
{
}

void CPauseState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
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
	}
	if (theScene->ReturnChangeScreen() && theScene->ReturnScreenTransition() == false)
	{
		switch (Select)
		{
		case 1:
		{
				  theGSM->PopState(false);
				  break;
		}
		case 2:
		{
				  theGSM->PopState(true);
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


	/*theScene->Update(m_dElapsedTime);
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
		theGSM->PopState(true);
	}*/
}

void CPauseState::Draw(CGameStateManager* theGSM)
{
	theScene->Render();
}
