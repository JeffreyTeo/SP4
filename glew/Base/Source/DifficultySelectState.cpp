#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "DifficultySelectState.h"
#include "LevelShopSelectionState.h"
#include "LevelSelectState.h"
#include "menustate.h"
#include "gamestate.h"

CDifficultySelectState CDifficultySelectState::theDifficultySelectState;

void CDifficultySelectState::Init()
{
	theScene = new SceneManagerLevel2DforScreen(800, 600, DifficultySelectscreen);
	theScene->Init();
	Select = 1;
	timer = 0.0f;
	theScene->SetSelection(Select);
}

void CDifficultySelectState::Init(const int width, const int height)
{
	theScene = new SceneManagerLevel2DforScreen(width, height, DifficultySelectscreen);
	theScene->Init();
	Select = 1;
	timer = 0.0f;
	theScene->SetSelection(Select);
}

void CDifficultySelectState::Cleanup()
{
	theScene->Exit();
	delete theScene;
	theScene = NULL;
}

void CDifficultySelectState::Pause()
{

}

void CDifficultySelectState::Resume(bool m_resume)
{

}

void CDifficultySelectState::HandleEvents(CGameStateManager* theGSM)
{}

void CDifficultySelectState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{}

void CDifficultySelectState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
	const int button_Left, const int button_Middle, const int button_Right)
{}

void CDifficultySelectState::Update(CGameStateManager* theGSM)
{
}

void CDifficultySelectState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	theScene->Update(m_dElapsedTime);
	if (theScene->ReturnChangeScreen() == false && theScene->ReturnScreenTransition() == false)
	{
		timer += m_dElapsedTime;
		if (Application::IsKeyPressed(VK_DOWN) && timer > 0.1f)
		{
			if (Select < 3)
			{
				Sound.engine->stopAllSounds();
				Sound.SelectSound();
				Select++;
				//Sleep(150);
				timer = 0;
			}
		}
		else if (Application::IsKeyPressed(VK_UP) && timer > 0.1f)
		{
			if (Select > 1)
			{
				Sound.engine->stopAllSounds();
				Sound.SelectSound();
				Select--;
				//Sleep(150);
				timer = 0;
			}
		}

		if (Application::IsKeyPressed(VK_RETURN) && timer > 0.1f)
		{
			Sound.engine->stopAllSounds();
			Sound.ConfirmSound();
			theScene->SetScreenTransition(true);
			theScene->SetChangeScreen(true);
		}

		if (Application::IsKeyPressed(VK_BACK) && timer > 0.1f)
		{
			Sound.engine->stopAllSounds();
			Sound.BackSound();
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
				   theGSM->ChangeState(CLevelShopSelectionState::Instance());
				   break;
		}
		case 1:
		{
				  theScene->setDifficulty(1);
				  theGSM->ChangeState(CLevelSelectState::Instance());
				  break;
		}
		case 2:
		{
				  theScene->setDifficulty(2);
				  theGSM->ChangeState(CLevelSelectState::Instance());
				  break;
		}
		case 3:
		{
				  theScene->setDifficulty(3);
				  if (theScene->GetLevelReferencetoContinue() != 0)
				  {
					  theGSM->ChangeState(CLevelSelectState::Instance());
				  }
				  break;
		}
		}
	}
	else
	{
		theScene->SetSelection(Select);
	}
}

void CDifficultySelectState::Draw(CGameStateManager* theGSM)
{
	theScene->Render();
}
