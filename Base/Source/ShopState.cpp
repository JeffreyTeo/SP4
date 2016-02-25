#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "LevelShopSelectionState.h"
#include "Shopstate.h"
#include "menustate.h"

CShopState CShopState::theShopState;

void CShopState::Init()
{
	theScene = new SceneManagerLevel2DforScreen(800, 600,Shopscreen);
	theScene->Init();
	Select = 1;
	theScene->SetSelection(Select);
	pressed = false;
	boughtOne = false;
	boughtTwo = false;
	boughtThree = false;
}

void CShopState::Init(const int width, const int height)
{
	theScene = new SceneManagerLevel2DforScreen(width, height, Shopscreen);
	theScene->Init();
	Select = 1;
	theScene->SetSelection(Select);
	pressed = false;
	boughtOne = false;
	boughtTwo = false;
	boughtThree = false;
}

void CShopState::Cleanup()
{
	theScene->Exit();
	delete theScene;
	theScene = NULL;
}

void CShopState::Pause()
{

}

void CShopState::Resume(bool m_resume)
{

}

void CShopState::HandleEvents(CGameStateManager* theGSM)
{}

void CShopState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{}

void CShopState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
	const int button_Left, const int button_Middle, const int button_Right)
{}

void CShopState::Update(CGameStateManager* theGSM)
{
}

void CShopState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
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
		}

		switch (Select)
		{
			case 1:
			{
				if (Application::IsKeyPressed(VK_RETURN) && pressed == false && boughtOne == false)
				{
					theScene->SetShopSelect((Select - 1));
					pressed = true;
					boughtOne = true;
				}
				else if (!Application::IsKeyPressed(VK_RETURN) && pressed == true)
				{
					pressed = false;
				}
				break;
			}
			case 2:
			{
				if (Application::IsKeyPressed(VK_RETURN) && pressed == false && boughtTwo == false)
				{
					theScene->SetShopSelect((Select - 1));
					pressed = true;
					boughtTwo = true;
				}
				else if (!Application::IsKeyPressed(VK_RETURN) && pressed == true)
				{
					pressed = false;
				}
				break;
			}
			case 3:
			{
				if (Application::IsKeyPressed(VK_RETURN) && pressed == false && boughtThree == false)
				{
					theScene->SetShopSelect((Select - 1));
					pressed = true;
					boughtThree = true;
				}
				else if (!Application::IsKeyPressed(VK_RETURN) && pressed == true)
				{
					pressed = false;
				}
				break;
			}
		}

		if (Application::IsKeyPressed(VK_BACK))
		{
			theScene->SetScreenTransition(true);
			theScene->SetChangeScreen(true);
		}
	}
	if (theScene->ReturnChangeScreen() && theScene->ReturnScreenTransition() == false)
	{
		theGSM->ChangeState(CLevelShopSelectionState::Instance());
	}
}

void CShopState::Draw(CGameStateManager* theGSM)
{
	theScene->Render();
}
