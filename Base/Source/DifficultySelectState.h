#pragma once

#include "gamestate.h"
#include "application.h"

class CDifficultySelectState : public CGameState
{
public:
	void Init();
	void Init(const int width, const int height);
	void Cleanup();

	void Pause();
	void Resume(bool m_resume);

	void HandleEvents(CGameStateManager* theGSM);
	void HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status = true);
	void HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
		const int button_Left, const int button_Middle, const int button_Right);
	void Update(CGameStateManager* theGSM);
	void Update(CGameStateManager* theGSM, const double m_dElapsedTime);
	void Draw(CGameStateManager* theGSM);

	static CDifficultySelectState* Instance() {
		return &theDifficultySelectState;
	}

	CSoundManager Sound;

protected:
	CDifficultySelectState() { }

private:
	static CDifficultySelectState theDifficultySelectState;
	SceneManagerLevel2DforScreen* theScene;
	int Select;
	double timer;
};
