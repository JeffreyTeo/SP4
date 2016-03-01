#ifndef SCENE_MANAGER_2D_H
#define SCENE_MANAGER_2D_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Minimap.h"
#include "Map.h"
#include "Vector2.h"
#include "PlayerInfo.h"
#include "Enemy.h"
#include "Player.h"
#include "Save.h"
#include "SpriteAnimation.h"
#include "SoundManager.h"
#include "LevelDetails.h"

#include "Highscore.h"
#include "HighscoreData.h"
#include "LuaUsage.h"

// Goodies and Goodies Factory
#include "GoodiesFactory.h"
#include "Goodies.h"
#include "TreasureChest.h"

//grid system and grid class
#include "GridSystem.h"

// level loader
#include "LevelLoader.h"

class CSceneManager2D : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_LIGHTENABLED,
		U_COLOR_TEXTURE0,
		U_COLOR_TEXTURE_ENABLED0,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TEXT_ALPHA_ENABLED,
		U_TEXT_ALPHA,
		U_TOTAL,
	};
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_BACKGROUND,
		GEO_TILEGROUND,
		GEO_TILEHERO,
		GEO_TILETREE,
		GEO_TILESTRUCTURE,
		GEO_TILEHERO_FRAME0,
		GEO_TILEHERO_FRAME1,
		GEO_TILEHERO_FRAME2,
		GEO_TILEHERO_FRAME3,
		GEO_TILE_KILLZONE,
		GEO_TILE_SAFEZONE,
		GEO_TILEENEMY_FRAME0,
		GEO_TILE_TREASURECHEST,
		GEO_SPRITE_ANIMATION,
		GEO_OBJECT,

		GEO_MENU,
		GEO_SELECT,
		GEO_INSTRUCTION,
		GEO_HIGHSCORE,
		GEO_VOL_MUTE,
		GEO_VOL,
		GEO_SOUND_MUTE,
		GEO_SOUND,

		GEO_FLOORING,
		GEO_CHARACTER,
		GEO_ROCK,
		GEO_TRAP,
		GEO_SNAKE,
		GEO_MONSTER,
		GEO_WALL,
		GEO_KEY,
		GEO_EXIT,

		// Tutorial
		GEO_SIGN,
		GEO_SIGN1,
		GEO_SIGN2,
		GEO_SIGN3,
		GEO_SIGN4,
		GEO_SIGN5,

		GEO_KEYSCOLLECTED,
		GEO_MOVESLEFT,
		// Show Movement
		GEO_FEET,

		GEO_TEXT,
		NUM_GEOMETRY,
	};
	enum PARTICLE_STYLE
	{
		DROPDOWN = 0,
		CONFETTI,
		NUM_PARTICLE_STYLE
	};
public:
	CSceneManager2D();
	CSceneManager2D(const int m_window_width, const int m_window_height);
	~CSceneManager2D();

	virtual void Init();
	virtual void Update(double dt);
	// Update Camera status
	virtual void UpdateCameraStatus(const unsigned char key, const bool status = true);
	// Update mouse status
	virtual void UpdateMouseStatus(const unsigned char key);
	virtual void Render();
	virtual void Exit();


	int GetWinCondition();
	void SetQuitfrompause(bool m_Quitfrompause);

	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, bool enablealpha = false);
	void RenderBackground();
	void RenderUI();
	void Render2DMesh(Mesh *mesh, const bool enableLight, bool enablealpha = false, const int size = 1, const int x = 0, const int y = 0, const bool rotate = false, float rotateAngle = 0.f, const bool flip = false);
	void PreInit();
	void RenderGridSystem();

	// Menu States
	bool PlaySelect;
	bool InstructionSelect;
	bool HighscoreSelect;
	bool OptionSelect;
	bool ExitSelect;

	// Option States
	bool SoundSelect;
	bool VolumeSelect;
	bool muted;
	
	// UI
	int KeysCollected;
	int NoOfMoves;
	bool MoveChar;

	

	void AddHighscore();
	HighscoreData theScore[5];
	CSoundManager Sound;

	float tempsound;

	enum WEAPON_ACTION
	{
		WA_NIL = 0,
		WA_LEFT_CLICKED,
		WA_FIRE,
		WA_RELOAD,
		WA_CHANGEWEAPON,
		WA_TOTAL,
	};

private:
	bool m_Quitfrompause;

	int m_WinCondition;

	LuaUsage* m_SpriteAnimationLoad;
	Player* m_player;
	Save* m_save;
	SpriteAnimation *m_spriteAnimation;
	LevelDetails* m_LevelDetails;

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;

	float rotateAngle;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	bool confettiRightside;
	float fps;

	// Tutorial
	bool ShowStart;		// Sign.1
	bool ShowMove;		// Sign.2
	bool ShowKey;		// Sign.3
	bool ShowMonster;	// Sign.4
	bool ShowExit;		// Sign.5
	// Checks if player has pressed 'X'
	bool Sign1Exited;
	bool Sign2Exited;
	bool Sign3Exited;
	bool Sign4Exited;
	bool Sign5Exited;

	//time Buffer for continuous key press

	float timeBuffer;

	//grid system and grids
	GridSystem* Playfield;
	GridSystem* TestField;
	//determine direction : 0 = up,180 = down, -90 = left, 90 = right and offset
	float direction;
	Vector3 offset;
	//window height and width
	int m_windowHeight;
	int m_windowWidth;

	//loaded level;
	LevelLoader *m_cLevel;
	// vector of AI
	vector<cAI*> AIList;
};

#endif