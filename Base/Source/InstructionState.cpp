#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "instructionstate.h"
#include "menustate.h"

CInstructionState CInstructionState::theInstructionState;

void CInstructionState::Init()
{
	theScene = new SceneManagerLevel2DforScreen(800, 600,Instructionscreen);
	theScene->Init();
}

void CInstructionState::Init(const int width, const int height)
{
	theScene = new SceneManagerLevel2DforScreen(width, height, Instructionscreen);
	theScene->Init();
}

void CInstructionState::Cleanup()
{
	theScene->Exit();
	delete theScene;
	theScene = NULL;
}

void CInstructionState::Pause()
{

}

void CInstructionState::Resume(bool m_resume)
{

}

void CInstructionState::HandleEvents(CGameStateManager* theGSM)
{}

void CInstructionState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{}

void CInstructionState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
	const int button_Left, const int button_Middle, const int button_Right)
{}

void CInstructionState::Update(CGameStateManager* theGSM)
{
}

void CInstructionState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	theScene->Update(m_dElapsedTime);
	if (theScene->ReturnScreenTransition() == false)
	{
		if (Application::IsKeyPressed(VK_BACK))
		{
			Sound.engine->stopAllSounds();
			Sound.BackSound();
			theScene->SetScreenTransition(true);
			theScene->SetChangeScreen(true);
		}
	}
	if (theScene->ReturnChangeScreen() && theScene->ReturnScreenTransition() == false)
	{
		theGSM->ChangeState(CMenuState::Instance());
	}
}

void CInstructionState::Draw(CGameStateManager* theGSM)
{
	theScene->Render();
}
