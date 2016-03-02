#include "SceneManager2D.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "Strategy_Kill.h"

CSceneManager2D::CSceneManager2D()
: m_player(NULL)
, m_save(NULL)
, m_spriteAnimation(NULL)
, Playfield(NULL)
, tempsound(0.5)
, m_Load(NULL)
, m_cLevel(NULL)
, theLevelDetailsHolder(NULL)
, m_LevelDetails(NULL)
, MoveChar(true)
, ShowStart(false)
, ShowMove(false)
, ShowMonster(false)
, ShowExit(false)
, Sign1Exited(false)
, Sign2Exited(false)
, Sign3Exited(false)
, Sign4Exited(false)
, Sign5Exited(false)
, aiPos(0)
/*
: m_cMinimap(NULL)
, m_cMap(NULL)
, tileOffset_x(0)
, tileOffset_y(0)
, m_cRearMap(NULL)
, rearWallOffset_x(0)
, rearWallOffset_y(0)
, rearWallTileOffset_x(0)
, rearWallTileOffset_y(0)
, rearWallFineOffset_x(0)
, rearWallFineOffset_y(0)
, theEnemy(NULL)
*/
{
}

CSceneManager2D::CSceneManager2D(const int m_window_width, const int m_window_height)
: m_player(NULL)
, m_save(NULL)
, m_spriteAnimation(NULL)
, Playfield(NULL)
, tempsound(0.5)
, m_Load(NULL)
, m_cLevel(NULL)
, theLevelDetailsHolder(NULL)
, m_LevelDetails(NULL)
, MoveChar(true)
, ShowStart(false)
, ShowMove(false)
, ShowMonster(false)
, ShowExit(false)
, Sign1Exited(false)
, Sign2Exited(false)
, Sign3Exited(false)
, Sign4Exited(false)
, Sign5Exited(false)
{
	this->m_windowWidth = m_window_width;
	this->m_windowHeight = m_window_height;
}

CSceneManager2D::~CSceneManager2D()
{
	if (m_player)
	{
		delete m_player;
		m_player = NULL;
	}
	
	if (m_save)
	{
		delete m_save;
		m_save = NULL;
	}

	if (Playfield)
	{
		delete Playfield;
		Playfield = NULL;
	}

	if (m_Load)
	{
		delete m_Load;
		m_Load = NULL;
	}

	if (m_cLevel)
	{
		delete m_cLevel;
		m_cLevel = NULL;
	}
	if (m_LevelDetails)
	{
		delete m_LevelDetails;
		m_LevelDetails = NULL;
	}

	if (AIList.size() > 0)
	{
		for (int a = 0; a < AIList.size(); a++)
		{
			delete AIList[a];
		}
		AIList.clear();
	}
	for (vector<AllLevelDetails*>::iterator it = theLevelDetailsHolder.begin(); it != theLevelDetailsHolder.end(); ++it)
	{
		AllLevelDetails* leveldetails = (AllLevelDetails*)*it;
		if (leveldetails != NULL)
		{
			delete leveldetails;
			leveldetails = NULL;
		}
	}
	/*
	if (m_spriteAnimation)
	{
		delete m_spriteAnimation;
		m_spriteAnimation = NULL;
	}*/
	/*
	for (int i=0; i<10; i++)
	{
	delete theArrayOfGoodies[i];
	}
	delete theArrayOfGoodies;

	if (theEnemy)
	{
	delete theEnemy;
	theEnemy = NULL;
	}

	if (m_cMap)
	{
	delete m_cMap;
	m_cMap = NULL;
	}

	if (m_cMinimap)
	{
	delete m_cMinimap;
	m_cMinimap = NULL;
	}
	*/
}

void CSceneManager2D::PreInit()
{
	// Black background
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);

	// Switch on culling
	glEnable(GL_CULL_FACE);

	// Render mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Activate blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate and bind vertex array
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Load the shaders
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Fog.fragmentshader");

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	m_parameters[U_TEXT_ALPHA] = glGetUniformLocation(m_programID, "truetextColor");
	m_parameters[U_TEXT_ALPHA_ENABLED] = glGetUniformLocation(m_programID, "truetextEnabled");
	// Use our shader
	glUseProgram(m_programID);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
}

void CSceneManager2D::Init()
{
	PreInit();

	// Initialise the camera
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

	// Create the meshes
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	// Load the ground mesh and texture
	meshList[GEO_BACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//background.tga");
	meshList[GEO_TILEGROUND] = MeshBuilder::Generate2DMesh("GEO_TILEGROUND", Color(1, 1, 1), 0, 0, 25, 25);
	meshList[GEO_TILEGROUND]->textureID = LoadTGA("Image//tile1_ground.tga");
	meshList[GEO_TILEHERO] = MeshBuilder::Generate2DMesh("GEO_TILEHERO", Color(1, 1, 1), 0, 0, 25, 25);
	meshList[GEO_TILEHERO]->textureID = LoadTGA("Image//tile2_hero.tga");
	meshList[GEO_TILETREE] = MeshBuilder::Generate2DMesh("GEO_TILETREE", Color(1, 1, 1), 0, 0, 25, 25);
	meshList[GEO_TILETREE]->textureID = LoadTGA("Image//tile3_tree.tga");
	meshList[GEO_TILESTRUCTURE] = MeshBuilder::Generate2DMesh("GEO_TILESTRUCTURE", Color(1, 1, 1), 0, 0, 25, 25);
	meshList[GEO_TILESTRUCTURE]->textureID = LoadTGA("Image//tile3_structure.tga");
	meshList[GEO_TILEHERO_FRAME0] = MeshBuilder::Generate2DMesh("GEO_TILEHERO_FRAME0", Color(1, 1, 1), 0, 0, 25, 25);
	meshList[GEO_TILEHERO_FRAME0]->textureID = LoadTGA("Image//tile2_hero_frame_0.tga");
	meshList[GEO_TILEHERO_FRAME1] = MeshBuilder::Generate2DMesh("GEO_TILEHERO_FRAME1", Color(1, 1, 1), 0, 0, 25, 25);
	meshList[GEO_TILEHERO_FRAME1]->textureID = LoadTGA("Image//tile2_hero_frame_1.tga");
	meshList[GEO_TILEHERO_FRAME2] = MeshBuilder::Generate2DMesh("GEO_TILEHERO_FRAME2", Color(1, 1, 1), 0, 0, 25, 25);
	meshList[GEO_TILEHERO_FRAME2]->textureID = LoadTGA("Image//tile2_hero_frame_2.tga");
	meshList[GEO_TILEHERO_FRAME3] = MeshBuilder::Generate2DMesh("GEO_TILEHERO_FRAME3", Color(1, 1, 1), 0, 0, 25, 25);
	meshList[GEO_TILEHERO_FRAME3]->textureID = LoadTGA("Image//tile2_hero_frame_3.tga");

	meshList[GEO_TILE_KILLZONE] = MeshBuilder::Generate2DMesh("GEO_TILE_KILLZONE", Color(1, 1, 1), 0, 0, 25, 25);
	meshList[GEO_TILE_KILLZONE]->textureID = LoadTGA("Image//tile10_killzone.tga");
	meshList[GEO_TILE_SAFEZONE] = MeshBuilder::Generate2DMesh("GEO_TILE_SAFEZONE", Color(1, 1, 1), 0, 0, 25, 25);
	meshList[GEO_TILE_SAFEZONE]->textureID = LoadTGA("Image//tile11_safezone.tga");
	meshList[GEO_TILEENEMY_FRAME0] = MeshBuilder::Generate2DMesh("GEO_TILEENEMY_FRAME0", Color(1, 1, 1), 0, 0, 50, 50);
	meshList[GEO_TILEENEMY_FRAME0]->textureID = LoadTGA("Image//ghostEnemy.tga");

	meshList[GEO_MENU] = MeshBuilder::Generate2DMesh("GEO_MENU", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_MENU]->textureID = LoadTGA("Image//MainMenu.tga");
	meshList[GEO_HIGHSCORE] = MeshBuilder::Generate2DMesh("GEO_HIGHSCORE", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_HIGHSCORE]->textureID = LoadTGA("Image//Highscore.tga");
	meshList[GEO_VOL_MUTE] = MeshBuilder::Generate2DMesh("GEO_VOL_MUTE", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_VOL_MUTE]->textureID = LoadTGA("Image//OptionsVolumeSoundOff.tga");
	meshList[GEO_VOL] = MeshBuilder::Generate2DMesh("GEO_VOL", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_VOL]->textureID = LoadTGA("Image//OptionsVolumeSoundOn.tga");
	meshList[GEO_SOUND_MUTE] = MeshBuilder::Generate2DMesh("GEO_SOUND_MUTE", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_SOUND_MUTE]->textureID = LoadTGA("Image//OptionsSoundOff.tga");
	meshList[GEO_SOUND] = MeshBuilder::Generate2DMesh("GEO_SOUND", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_SOUND]->textureID = LoadTGA("Image//OptionsSoundOn.tga");
	meshList[GEO_INSTRUCTION] = MeshBuilder::Generate2DMesh("GEO_INSTRUCTIONS", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_INSTRUCTION]->textureID = LoadTGA("Image//Instructions.tga");

	meshList[GEO_SELECT] = MeshBuilder::Generate2DMesh("GEO_SELECT", Color(1, 1, 1), 0, 0, 75, 55);
	meshList[GEO_SELECT]->textureID = LoadTGA("Image//Select.tga");

	meshList[GEO_KEYSCOLLECTED] = MeshBuilder::Generate2DMesh("GEO_KEYSCOLLECTED", Color(1, 1, 1), 0, 0, 200, 100);
	meshList[GEO_KEYSCOLLECTED]->textureID = LoadTGA("Image//KeysCollected.tga");
	meshList[GEO_MOVESLEFT] = MeshBuilder::Generate2DMesh("GEO_MOVESLEFT", Color(1, 1, 1), 0, 0, 200, 100);
	meshList[GEO_MOVESLEFT]->textureID = LoadTGA("Image//MovesLeft.tga");

	meshList[GEO_CHARACTER] = MeshBuilder::Generate2DMesh("GEO_CHARACTER", Color(1, 1, 1), 0, 0, 50, 50);
	meshList[GEO_CHARACTER]->textureID = LoadTGA("Image//Temp.tga");
	meshList[GEO_FLOORING] = MeshBuilder::Generate2DMesh("GEO_FLOORING", Color(1, 1, 1), 0, 0, 50, 50);
	meshList[GEO_FLOORING]->textureID = LoadTGA("Image//Floor.tga");
	meshList[GEO_ROCK] = MeshBuilder::Generate2DMesh("GEO_ROCK", Color(1, 1, 1), 0, 0, 50, 50);
	meshList[GEO_ROCK]->textureID = LoadTGA("Image//Rock.tga");
	meshList[GEO_TRAP] = MeshBuilder::Generate2DMesh("GEO_TRAP", Color(1, 1, 1), 0, 0, 50, 50);
	meshList[GEO_TRAP]->textureID = LoadTGA("Image//Trap.tga");
	meshList[GEO_SNAKE] = MeshBuilder::Generate2DMesh("GEO_SNAKE", Color(1, 1, 1), 0, 0, 50, 50);
	meshList[GEO_SNAKE]->textureID = LoadTGA("Image//Snake.tga");
	meshList[GEO_MONSTER] = MeshBuilder::Generate2DMesh("GEO_MONSTER", Color(1, 1, 1), 0, 0, 50, 50);
	meshList[GEO_MONSTER]->textureID = LoadTGA("Image//Monster.tga");
	meshList[GEO_WALL] = MeshBuilder::Generate2DMesh("GEO_WALL", Color(1, 1, 1), 0, 0, 50, 50);
	meshList[GEO_WALL]->textureID = LoadTGA("Image//Wall.tga");
	meshList[GEO_KEY] = MeshBuilder::Generate2DMesh("GEO_KEY", Color(1, 1, 1), 0, 0, 50, 50);
	meshList[GEO_KEY]->textureID = LoadTGA("Image//Key.tga");
	meshList[GEO_FEET] = MeshBuilder::Generate2DMesh("GEO_FEET", Color(1, 1, 1), 0, 0, 50, 50);
	meshList[GEO_FEET]->textureID = LoadTGA("Image//Feet.tga");
	meshList[GEO_EXIT] = MeshBuilder::Generate2DMesh("GEO_EXIT", Color(1, 1, 1), 0, 0, 50, 50);
	meshList[GEO_EXIT]->textureID = LoadTGA("Image//Exit.tga");

	meshList[GEO_SIGN] = MeshBuilder::Generate2DMesh("GEO_SIGN", Color(1, 1, 1), 0, 0, 50, 50);
	meshList[GEO_SIGN]->textureID = LoadTGA("Image//Sign.tga");

	meshList[GEO_SIGN1] = MeshBuilder::Generate2DMesh("GEO_SIGN1", Color(1, 1, 1), 0, 0, 260, 230);
	meshList[GEO_SIGN1]->textureID = LoadTGA("Image//Sign1.tga");
	meshList[GEO_SIGN2] = MeshBuilder::Generate2DMesh("GEO_SIGN2", Color(1, 1, 1), 0, 0, 260, 230);
	meshList[GEO_SIGN2]->textureID = LoadTGA("Image//Sign2.tga");
	meshList[GEO_SIGN3] = MeshBuilder::Generate2DMesh("GEO_SIGN3", Color(1, 1, 1), 0, 0, 260, 230);
	meshList[GEO_SIGN3]->textureID = LoadTGA("Image//Sign3.tga");
	meshList[GEO_SIGN4] = MeshBuilder::Generate2DMesh("GEO_SIGN4", Color(1, 1, 1), 0, 0, 260, 230);
	meshList[GEO_SIGN4]->textureID = LoadTGA("Image//Sign4.tga");
	meshList[GEO_SIGN5] = MeshBuilder::Generate2DMesh("GEO_SIGN5", Color(1, 1, 1), 0, 0, 260, 230);
	meshList[GEO_SIGN5]->textureID = LoadTGA("Image//Sign5.tga");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	m_Quitfrompause = false;
	m_WinCondition = 0;
	m_save = new Save();
	m_player = new Player();
	m_player->PlayerInit("Player");
	
	KeysCollected = 0;
	m_LevelDetails = new LevelDetails();
	m_LevelDetails->LevelDetailsInit(m_player->GetLevelToDifficultyStartAt(), m_player->GetLevelToStartAt(), "Level");
	NoOfMoves = m_LevelDetails->GetAmountOfMoves();
	rotateAngle = 0;

playerHealth = 3;
	
	m_Load = new LuaUsage();
	m_Load->LuaUsageInit("LeveltoSave");
	m_maxlevel = m_Load->get<int>("AmountOfLevel");
	m_maxdiff = m_Load->get<int>("AmountOfDiff");
	m_Load->LuaUsageClose();
	string Start = "Level.";
	for (int i = 0; i < m_maxdiff; ++i)
	{
		string Diff = "";
		switch (i)
		{
		case 0:
		{
				  Diff = Start + "Easy.";
				  break;
		}
		case 1:
		{
				  Diff = Start + "Normal.";
				  break;
		}
		case 2:
		{
				  Diff = Start + "Hard.";
				  break;
		}
		}
		for (int j = 0; j < m_maxlevel; ++j)
		{
			string Level = Diff + "Level" + to_string((j + 1)) + ".";
			AllLevelDetails* m_levelofdetail = new AllLevelDetails();
			m_levelofdetail->AllLevelDetailsInit(Level);
			theLevelDetailsHolder.push_back(m_levelofdetail);
		}
	}

	//level loader
	m_cLevel = new LevelLoader();
	m_cLevel->Init((m_LevelDetails->GetNumberOfGridY() + 1) * m_LevelDetails->GetLengthYOfAGrid(), m_LevelDetails->GetNumberOfGridX() * m_LevelDetails->GetLengthXOfAGrid(), m_LevelDetails->GetNumberOfGridY() + 1, m_LevelDetails->GetNumberOfGridX());
	m_cLevel->LevelLoaded(m_LevelDetails->GetNameOfLevelFile());

	//initailise grid system
	Playfield = new GridSystem();
	// in this order: position of the whole grid system, size of grid x, size of grid y, number of grid x, number of grid y 
	Playfield->Init(Vector3(m_LevelDetails->GetPositionXOfGrid(), m_LevelDetails->GetPositionYOfGrid(), 0), m_LevelDetails->GetLengthXOfAGrid(), m_LevelDetails->GetLengthYOfAGrid(), m_LevelDetails->GetNumberOfGridY(), m_LevelDetails->GetNumberOfGridX());

	Playfield->PlayerGridSetUp(m_LevelDetails->GetPlayerPositionX(), m_LevelDetails->GetPlayerPositionY());

	Playfield->SetMap(m_cLevel->screenMap);

	cAI * AI = new cAI();
	AI->init();
	AI->setPos(8, 1);
	AI->setWaypoint(3, 8);
	AI->setWaypoint(3, 3);
	AI->setWaypoint(8, 3);
	AI->setWaypoint(8, 8);
	AI->setDifficulty(m_player->GetLevelToStartAt());
	AIList.push_back(AI);

	Playfield->AIGridSetUp(AIList);
	/*for (int a = 0; a < 11; a++)
	{
		for (int b = 0; b < 10; b++)
		{
			cout << m_cLevel->screenMap[a][b];
		}
		cout << endl;
	}*/
	//direction and offset
	direction = 0.f;
	offset = Vector3(0, 0, 0);

	timeBuffer = 0.f;

	TestField = new GridSystem();
	TestField->Init(Vector3(625, 300, 0), 50.f, 50.f, 6, 12);
	TestField->GridDropInit();
	TestField->PlayerGridSetUp(4, 10);

}

void CSceneManager2D::SetQuitfrompause(bool m_Quitfrompause)
{
	KeysCollected = 0;
	this->m_Quitfrompause = m_Quitfrompause;
	this->m_player->SetLevelStopAt(m_LevelDetails->GetLevelinDifficultyReference(),m_LevelDetails->GetDifficultyReference());
}

void CSceneManager2D::AddHighscore()
{
	const int MAX_SCORES = 5;
	string values[MAX_SCORES];
	for (int i = 0; i < MAX_SCORES; i++)
	{
		theScore[i].ReadTextFile("highscore.txt");
	}
}

int CSceneManager2D::GetWinCondition()
{
	return m_WinCondition;
}

void SceneManagerLevel2DforScreen::setDifficulty(int m_Difficulty)
{
	this->m_player->SetLevelToDifficultyStartAt(m_Difficulty);
}
void SceneManagerLevel2DforScreen::setLevel(int m_Level)
{
	this->m_player->SetLevelToStartAt(m_Level);
}

static float limit = 1.f;
static float timer = 0.f;


void CSceneManager2D::Update(double dt)
{
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//if (Application::IsKeyPressed('5'))
		//m_WinCondition = 1;

	rotateAngle -= (float)Application::camera_yaw;// += (float)(10 * dt);

	//cout << Sound.volume << endl;
	int TempKeyCollectedCalc = 0;
	int count = 0;

	if (ShowStart == true || ShowKey == true || ShowMove == true || ShowMonster == true || ShowExit == true)
	{
		MoveChar = false;
	}

	for (int a = 0; a < Playfield->GetGridsVec().size(); a++)
	{
		
		if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::KEY)
		{
			if (Playfield->GetGridsVec()[a]->keyCollected == true)
			{
				TempKeyCollectedCalc++;
			}
		}
		this->KeysCollected = TempKeyCollectedCalc;

		/*if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::EXIT)
		{
			if (Playfield->GetGridsVec()[a]->Win == true)
			{
				TempWin = 1;
			}
		}*/
		this->m_WinCondition = Playfield->CheckCollisionType(Grid::GridType::EXIT, count);

		/*if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::INTROSIGN)
		{
			if (Playfield->GetGridsVec()[a]->Sign1Exited == true)
			{
				Sign1Touch = true;
				}
				}*/
		if (Sign1Exited == false)
			this->ShowStart = Playfield->CheckCollisionType(Grid::GridType::INTROSIGN, count);
		
		/*if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::MOVESIGN)
		{
			if (Playfield->GetGridsVec()[a]->Sign2Exited == true)
			{
				Sign2Touch = true;
			}
		}*/
		if (Sign2Exited == false)
			this->ShowMove = Playfield->CheckCollisionType(Grid::GridType::MOVESIGN, count);

		/*if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::KEYSIGN)
		{
			if (Playfield->GetGridsVec()[a]->Sign3Exited == true)
			{
				Sign3Touch = true;
			}
		}*/
		if (Sign3Exited == false)
			this->ShowKey = Playfield->CheckCollisionType(Grid::GridType::KEYSIGN, count);

		/*if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::MONSTERSIGN)
		{
			if (Playfield->GetGridsVec()[a]->Sign4Exited == true)
			{
				Sign4Touch = true;
			}
		}*/
		if (Sign4Exited == false)
			this->ShowMonster = Playfield->CheckCollisionType(Grid::GridType::MONSTERSIGN, count);

		/*if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::EXITSIGN)
		{
			if (Playfield->GetGridsVec()[a]->Sign5Exited == true)
			{
				Sign5Touch = true;
			}
		}*/
		if (Sign5Exited == false)
			this->ShowExit = Playfield->CheckCollisionType(Grid::GridType::EXITSIGN, count);
	}

	if (Application::IsKeyPressed('X'))
	{
		MoveChar = true;
		if (ShowStart == true)
		{
			Sign1Exited = true;
			ShowStart = false;
		}
		if (ShowMove == true)
		{
			Sign2Exited = true;
			ShowMove = false;
		}
		if (ShowKey == true)
		{
			Sign3Exited = true;
			ShowKey = false;
		}
		if (ShowMonster == true)
		{
			Sign4Exited = true;
			ShowMonster = false;
		}
		if (ShowExit == true)
		{
			Sign5Exited = true;
			ShowExit = false;
		}
	}

	/*if (Application::IsKeyPressed('X'))
	{
		MoveChar = true;
		if (ShowStart = true)
			ShowStart = false;
		for (int a = 0; a < Playfield->GetGridsVec().size(); a++)
		{
			if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::INTROSIGN)
			{
				if (ShowStart == true)
				{
					ShowStart = false;
				}
			}

			if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::MOVESIGN)
			{
				if (Playfield->GetGridsVec()[a]->Sign2Exited == true)
				{
					Playfield->GetGridsVec()[a]->Sign2Exited = false;
				}
			}
			this->ShowMove = Sign2Touch;

			if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::KEYSIGN)
			{
				if (Playfield->GetGridsVec()[a]->Sign3Exited == true)
				{
					Playfield->GetGridsVec()[a]->Sign3Exited = false;
				}
			}
			this->ShowKey = Sign3Touch;

			if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::MONSTERSIGN)
			{
				if (Playfield->GetGridsVec()[a]->Sign4Exited == true)
				{
					Playfield->GetGridsVec()[a]->Sign4Exited = false;
				}
			}
			this->ShowMonster = Sign4Touch;

			if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::EXITSIGN)
			{
				if (Playfield->GetGridsVec()[a]->Sign5Exited == true)
				{
					Playfield->GetGridsVec()[a]->Sign5Exited = false;
				}
			}
			this->ShowExit = Sign5Touch;
		}
	}*/

	//playfield
	if (NoOfMoves <= 0)
	{
		MoveChar = false;
		NoOfMoves = 0;
	}

	if (MoveChar == true)
	{
		if (timeBuffer > 5.f)
		{
			char key = 'z';
			if (Application::IsKeyPressed('W'))
			{
				key = 'w';
			}
			else if (Application::IsKeyPressed('S'))
			{
				key = 's';
			}
			else if (Application::IsKeyPressed('A'))
			{
				key = 'a';
			}
			else if (Application::IsKeyPressed('D'))
			{
				key = 'd';
			}
			if (Playfield->PlayerGridUpdate(key))
			{
				timeBuffer = 0.f;
				NoOfMoves--;
			}
		}
		timeBuffer += 1.f;
	}


	/*****************************************************************/
	//test field stuff
	{
		char key = 'z';
		if (Application::IsKeyPressed('W'))
		{
			key = 'w';
		}
		else if (Application::IsKeyPressed('S'))
		{
			key = 's';
		}
		else if (Application::IsKeyPressed('A'))
		{
			key = 'a';
		}
		else if (Application::IsKeyPressed('D'))
		{
			key = 'd';
		}
		else if (Application::IsKeyPressed('P'))
		{
			key = 'p';
		}

		TestField->PlayerGridDropUpdate(key);
		TestField->PlayerGridDropStateChange(key);

		//cout << TestField->GetPlayerGrid()->GetDirection() << endl;
		TestField->GridDropUpdate();
	}
	///*****************************************************************/


	camera.Update(dt);
	//m_spriteAnimation->Update(dt);
	
	Playfield->AIGridUpdate();
	/*

	// Update the hero
	if(Application::IsKeyPressed('W'))
	this->theHero->MoveUpDown( true, 1.0f );
	if(Application::IsKeyPressed('S'))
	this->theHero->MoveUpDown( false, 1.0f );
	if(Application::IsKeyPressed('A'))
	this->theHero->MoveLeftRight( true, 1.0f );
	if(Application::IsKeyPressed('D'))
	this->theHero->MoveLeftRight( false, 1.0f );
	if(Application::IsKeyPressed(' '))
	this->theHero->SetToJumpUpwards(true);
	theHero->HeroUpdate(m_cMap);

	// ReCalculate the tile offsets
	tileOffset_x = (int) (theHero->GetMapOffset_x() / m_cMap->GetTileSize());
	if (tileOffset_x + m_cMap->GetNumOfTiles_Width() > m_cMap->getNumOfTiles_MapWidth())
	tileOffset_x = m_cMap->getNumOfTiles_MapWidth() - m_cMap->GetNumOfTiles_Width();

	// if the hero enters the kill zone, then enemy goes into kill strategy mode
	int checkPosition_X = (int) ((theHero->GetMapOffset_x() + theHero->GetPos_x()) / m_cMap->GetTileSize());
	int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ( (theHero->GetPos_y() + m_cMap->GetTileSize()) / m_cMap->GetTileSize());
	if (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 10)
	{
	theEnemy->ChangeStrategy( new CStrategy_Kill());
	}
	else if (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 11)
	{
	theEnemy->ChangeStrategy(NULL);
	//		theEnemy->ChangeStrategy( new CStrategy_Kill());
	}
	else
	{
	//theEnemy->ChangeStrategy(NULL);
	}

	// Update the enemies
	theEnemy->SetDestination( theHero->GetPos_x(), theHero->GetPos_y() );
	theEnemy->Update( m_cMap );
	*/

	//aiPos = Playfield->GetAIGrids();
	timer += 0.1f;
	for (int a = 0; a < Playfield->GetAIGrids().size(); a++)		//check number of ai on the grid
	{
		if (Playfield->GetPlayerGrid() == Playfield->GetAIGrids()[a] && timer >= limit)
		{
			playerHealth--;
			timer = 0;
			if (playerHealth == 0)
			{
				m_WinCondition = 1;
			}
		}
	}

	cout << playerHealth << endl;

	fps = (float)(1.f / dt);
}

/********************************************************************************
 Update Camera position
 ********************************************************************************/
void CSceneManager2D::UpdateCameraStatus(const unsigned char key, const bool status)
{
	//camera.UpdateStatus(key, status);

	// Update avatar position
}

/********************************************************************************
 Update Weapon status
 ********************************************************************************/
void CSceneManager2D::UpdateMouseStatus(const unsigned char key)
{
	if (key == WA_LEFT_CLICKED)
	{
		//get cursor position
		double x, y;
		Application::GetMousePos(x, y);
		Playfield->UpdateGrid(Vector3(x, y, 0));
		cout << x << ", " << y << endl;
	}
}

/********************************************************************************
 Render text onto the screen
 ********************************************************************************/
void CSceneManager2D::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, bool enablealpha)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, 0);
				modelStack.Scale(size, size, size);
				if (enablealpha == true)
				{
					glUniform1i(m_parameters[U_TEXT_ALPHA_ENABLED], 1);
					glUniform4fv(m_parameters[U_TEXT_ALPHA], 1, &color.r);
				}
				else
				{
					glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
					glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
				}
				//glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
				//glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
				//	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mesh->textureID);
				glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				for(unsigned i = 0; i < text.length(); ++i)
				{
					Mtx44 characterSpacing;
					characterSpacing.SetToTranslation((i*0.5f) + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
					Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
					glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
					mesh->Render((unsigned)text[i] * 6, 6);
				}
				glBindTexture(GL_TEXTURE_2D, 0);
				glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

/********************************************************************************
 Render 2D Mesh
 ********************************************************************************/
void CSceneManager2D::Render2DMesh(Mesh *mesh, bool enableLight, bool enablealpha, int size, int x, int y, bool rotate, float rotateAngle,bool flip)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate( (float)x, (float)y, 0);
				modelStack.Scale( (float)size, (float)size, (float)size);
				if (rotate)
					modelStack.Rotate(rotateAngle, 0, 0, 1);
       
				Mtx44 MVP, modelView, modelView_inverse_transpose;
	
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				if(mesh->textureID > 0)
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
					if (enablealpha == false)
					{
						glUniform1i(m_parameters[U_TEXT_ALPHA_ENABLED], 0);
					}
					else
					{
						glUniform1i(m_parameters[U_TEXT_ALPHA_ENABLED], 1);
					}
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mesh->textureID);
					glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				}
				else
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
				}
				mesh->Render();
				if(mesh->textureID > 0)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}
       
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

/********************************************************************************
 Render the background
 ********************************************************************************/
void CSceneManager2D::RenderBackground()
{
	// Render the crosshair
	Render2DMesh(meshList[GEO_BACKGROUND], false, false, 1);
}

void CSceneManager2D::RenderGridSystem()
{
	//render grid layout
	for (int a = 0; a < Playfield->GetGridsVec().size(); a++)
	{
		modelStack.PushMatrix();
		//get position of a grid in the vector 
		Vector3 GridPos = Playfield->GetGridsVec()[a]->GetPos();
		if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::FLOOR)
			Render2DMesh(meshList[GEO_FLOORING], false, false, 1, GridPos.x, GridPos.y);
		else if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::WALL)
			Render2DMesh(meshList[GEO_WALL], false, false, 1, GridPos.x, GridPos.y);	
		else if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::KEY)
		{
			if (Playfield->GetGridsVec()[a]->keyCollected == false)
			{
				Render2DMesh(meshList[GEO_KEY], false, false, 1, GridPos.x, GridPos.y);
			}
			else
			{
				Render2DMesh(meshList[GEO_FLOORING], false, false, 1, GridPos.x, GridPos.y);
			}
		}
		else if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::TRAP)
			Render2DMesh(meshList[GEO_TRAP], false, false, 1, GridPos.x, GridPos.y);	
		else if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::ROCK)
			Render2DMesh(meshList[GEO_ROCK], false, false, 1, GridPos.x, GridPos.y);
		else if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::EXIT)
			Render2DMesh(meshList[GEO_EXIT], false, false, 1, GridPos.x, GridPos.y);
		else if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::INTROSIGN)
			Render2DMesh(meshList[GEO_SIGN], false, false, 1, GridPos.x, GridPos.y);
		else if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::MOVESIGN)
			Render2DMesh(meshList[GEO_SIGN], false, false, 1, GridPos.x, GridPos.y);
		else if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::KEYSIGN)
			Render2DMesh(meshList[GEO_SIGN], false, false, 1, GridPos.x, GridPos.y);
		else if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::MONSTERSIGN)
			Render2DMesh(meshList[GEO_SIGN], false, false, 1, GridPos.x, GridPos.y);
		else if (Playfield->GetGridsVec()[a]->GetType() == Grid::GridType::EXITSIGN)
			Render2DMesh(meshList[GEO_SIGN], false, false, 1, GridPos.x, GridPos.y);
		
		//cout << "rendered at" << Playfield->GetGridsVec()[a]->GetPos().x << ", " << Playfield->GetGridsVec()[a]->GetPos().y << endl;
		modelStack.PopMatrix();
	}
	//render player trail
	for (int a = 0; a < Playfield->GetGridsVec().size(); a++)
	{
		modelStack.PushMatrix();
		//get position of a grid in the vector 
		Vector3 GridPos = Playfield->GetGridsVec()[a]->GetPos();
		if (Playfield->GetGridsVec()[a]->GetStatus() == 1 && (Playfield->GetGridsVec()[a]->GetType() == 0 || Playfield->GetGridsVec()[a]->GetType() == 2))
		{
			float FeetDirection = Playfield->GetGridsVec()[a]->GetDirection();
			if (FeetDirection == 0.f)
			offset = Vector3(0, 0, 0);
			else if (FeetDirection == 180.f)
			offset = Vector3(50, 50, 0);
			else if (FeetDirection == 90.f)
			offset = Vector3(50, 0, 0);
			else if (FeetDirection == -90.f)
			offset = Vector3(0, 50, 0);
			
			Render2DMesh(meshList[GEO_FEET], false, false, 1, GridPos.x + offset.x, GridPos.y + offset.y, true, FeetDirection);
			//Render2DMesh(meshList[GEO_FEET], false, false, 1, GridPos.x + offset.x, GridPos.y + offset.y, true, direction);

		}

		//cout << "rendered at" << Playfield->GetGridsVec()[a]->GetPos().x << ", " << Playfield->GetGridsVec()[a]->GetPos().y << endl;
		modelStack.PopMatrix();
	}

	//render player
	Vector3 PlayerGridPos = Playfield->GetPlayerGrid()->GetPos();
	Render2DMesh(meshList[GEO_CHARACTER], false, false, 1, PlayerGridPos.x, PlayerGridPos.y);
	//render AI
	for (int a = 0; a < Playfield->GetAIGrids().size(); a++)
	{
		Vector3 AIGridPos = Playfield->GetAIGrids()[a]->GetPos();
		Render2DMesh(meshList[GEO_TILEENEMY_FRAME0], false, false, 1, AIGridPos.x, AIGridPos.y);
	}
		


	//{
	//	//test field
	//	//render grid layout
	//	for (int a = 0; a < TestField->GetGridsVec().size(); a++)
	//	{
	//		modelStack.PushMatrix();
	//		//get position of a grid in the vector 
	//		Vector3 GridPos = TestField->GetGridsVec()[a]->GetPos();
	//		if (TestField->GetGridsVec()[a]->GetType() == Grid::GridType::FLOOR)
	//			Render2DMesh(meshList[GEO_FLOORING], false, false, 1, GridPos.x, GridPos.y);
	//		else if (TestField->GetGridsVec()[a]->GetType() == Grid::GridType::WALL)
	//			Render2DMesh(meshList[GEO_WALL], false, false, 1, GridPos.x, GridPos.y);
	//		else if (TestField->GetGridsVec()[a]->GetType() == Grid::GridType::KEY)
	//		{
	//			if (TestField->GetGridsVec()[a]->keyCollected == false)
	//			{
	//				Render2DMesh(meshList[GEO_KEY], false, false, 1, GridPos.x, GridPos.y);
	//			}
	//			else
	//			{
	//				Render2DMesh(meshList[GEO_FLOORING], false, false, 1, GridPos.x, GridPos.y);
	//			}
	//		}
	//		else if (TestField->GetGridsVec()[a]->GetType() == Grid::GridType::TRAP)
	//			Render2DMesh(meshList[GEO_TRAP], false, false, 1, GridPos.x, GridPos.y);
	//		else if (TestField->GetGridsVec()[a]->GetType() == Grid::GridType::ROCK)
	//			Render2DMesh(meshList[GEO_ROCK], false, false, 1, GridPos.x, GridPos.y);
	//		else if (TestField->GetGridsVec()[a]->GetType() == Grid::GridType::EXIT)
	//			Render2DMesh(meshList[GEO_EXIT], false, false, 1, GridPos.x, GridPos.y);
	//		//cout << "rendered at" << Playfield->GetGridsVec()[a]->GetPos().x << ", " << Playfield->GetGridsVec()[a]->GetPos().y << endl;
	//		modelStack.PopMatrix();
	//	}
	//	//render player
	//	PlayerGridPos = TestField->GetPlayerGrid()->GetPos();
	//	Render2DMesh(meshList[GEO_CHARACTER], false, false, 1, PlayerGridPos.x, PlayerGridPos.y);
	//}
	//


}

void CSceneManager2D::RenderUI()
{
	modelStack.PushMatrix();
	Render2DMesh(meshList[GEO_MOVESLEFT], false, false, 1, 0, 500);

	std::ostringstream MovesLeft;
	MovesLeft << "x " << NoOfMoves;
	RenderTextOnScreen(meshList[GEO_TEXT], MovesLeft.str(), Color(0, 0, 0), 40, 100, 520, true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	Render2DMesh(meshList[GEO_KEYSCOLLECTED], false, false, 1, 200, 500);

	std::ostringstream Keys;
	Keys << "x " << KeysCollected;
	RenderTextOnScreen(meshList[GEO_TEXT], Keys.str(), Color(0, 0, 0), 40, 300, 520, true);
	modelStack.PopMatrix();

	if (ShowStart)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_SIGN1], false, false, 1, 520, 350);
		modelStack.PopMatrix();
	}
	if (ShowMove)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_SIGN2], false, false, 1, 520, 350);
		modelStack.PopMatrix();
	}
	if (ShowKey)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_SIGN3], false, false, 1, 520, 350);
		modelStack.PopMatrix();
	}
	if (ShowMonster)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_SIGN4], false, false, 1, 520, 350);
		modelStack.PopMatrix();
	}
	if (ShowExit)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_SIGN5], false, false, 1, 520, 350);
		modelStack.PopMatrix();
	}

}

/********************************************************************************
 Render this scene
 ********************************************************************************/
void CSceneManager2D::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	/*
	modelStack.PushMatrix();
	Render2DMesh(meshList[GEO_SPRITE_ANIMATION], false,50,400,300);
	modelStack.PopMatrix();*/
	
	// Render the background image
	RenderBackground();
	/*// Render the rear tile map
	RenderRearTileMap();
	// Render the tile map
	RenderTileMap();
	// Render the goodies
	RenderGoodies();
	*/

	RenderUI();

	//render the grid system and the corresponding image for the each grid
	RenderGridSystem();

	//On screen text
	/*
	std::ostringstream ss;
	ss.precision(5);
	ss << "theEnemy: " << theEnemy->GetPos_x() << ", " << theEnemy->GetPos_y();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 6);
	std::ostringstream sss;
	sss.precision(5);
	sss << "mapOffset_x: "<<theHero->GetMapOffset_x();
	RenderTextOnScreen(meshList[GEO_TEXT], sss.str(), Color(0, 1, 0), 30, 0, 30);
	*/
	
	std::ostringstream ss;
	ss.precision(5);
	ss << "test text: " << " 9999999 ";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0, 0.5f), 30, 0, 6, true);
}

/********************************************************************************
 Exit this scene
 ********************************************************************************/
void CSceneManager2D::Exit()
{
	if (!m_Quitfrompause)
	{
		int i = m_player->GetLevelToDifficultyStartAt()- 1;
		int j = i * m_maxlevel;
		int k = m_player->GetLevelToStartAt();
		theLevelDetailsHolder[((j + k)-1)]->SetCleared(true);
		if (theLevelDetailsHolder[((j + k)-1)]->GetCollectedKeys() < KeysCollected)
		{
			m_player->SetAmtOfCurrency(m_player->GetAmtOfCurrency() + (KeysCollected - theLevelDetailsHolder[((j + k) - 1)]->GetCollectedKeys()));
			theLevelDetailsHolder[((j + k) - 1)]->SetCollectedKeys(KeysCollected);
		}
			
		if (theLevelDetailsHolder[((j + k) - 1)]->GetCollectedKeys() == 3)
			theLevelDetailsHolder[((j + k) - 1)]->SetCollectedKeys(3);	
	}
	m_save->SaveLevelStuff(theLevelDetailsHolder, m_maxlevel, m_maxdiff);
	m_save->SavePlayer(m_player);


	
	if (m_spriteAnimation)
	{
		delete m_spriteAnimation->m_anim;
		m_spriteAnimation->m_anim = NULL;
	}
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{

		if (meshList[i] && i != GEO_SPRITE_ANIMATION)
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	
}


/*

void CSceneManager2D::RenderTileMap()
{
	int m = 0;
	for (int i = 0; i < m_cMap->GetNumOfTiles_Height(); i++)
	{
		for (int k = 0; k < m_cMap->GetNumOfTiles_Width() + 1; k++)
		{
			m = tileOffset_x + k;
			// If we have reached the right side of the Map, then do not display the extra column of tiles.
			if ((tileOffset_x + k) >= m_cMap->getNumOfTiles_MapWidth())
				break;
			if (m_cMap->theScreenMap[i][m] == 1)
			{
				Render2DMesh(meshList[GEO_TILEGROUND], false, 1, k*m_cMap->GetTileSize() - theHero->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][m] == 2)
			{
				Render2DMesh(meshList[GEO_TILETREE], false, 1, k*m_cMap->GetTileSize() - theHero->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][m] == 10)
			{
				Render2DMesh(meshList[GEO_TILE_KILLZONE], false, 1, k*m_cMap->GetTileSize() - theHero->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize());
			}
			else if (m_cMap->theScreenMap[i][m] == 11)
			{
				Render2DMesh(meshList[GEO_TILE_SAFEZONE], false, 1, k*m_cMap->GetTileSize() - theHero->GetMapFineOffset_x(), 575 - i*m_cMap->GetTileSize());
			}
		}
	}

	if (theHero->GetAnimationInvert() == false)
	{
		if (theHero->GetAnimationCounter() == 0)
			Render2DMesh(meshList[GEO_TILEHERO_FRAME0], false, 1, theHero->GetPos_x(), theHero->GetPos_y());
		else if (theHero->GetAnimationCounter() == 1)
			Render2DMesh(meshList[GEO_TILEHERO_FRAME1], false, 1, theHero->GetPos_x(), theHero->GetPos_y());
		else if (theHero->GetAnimationCounter() == 2)
			Render2DMesh(meshList[GEO_TILEHERO_FRAME2], false, 1, theHero->GetPos_x(), theHero->GetPos_y());
		else if (theHero->GetAnimationCounter() == 3)
			Render2DMesh(meshList[GEO_TILEHERO_FRAME3], false, 1, theHero->GetPos_x(), theHero->GetPos_y());
		else
			Render2DMesh(meshList[GEO_TILEHERO_FRAME0], false, 1, theHero->GetPos_x(), theHero->GetPos_y());
	}
	else
	{
		if (theHero->GetAnimationCounter() == 0)
			Render2DMesh(meshList[GEO_TILEHERO_FRAME0], false, 1, theHero->GetPos_x(), theHero->GetPos_y(), false, true);
		else if (theHero->GetAnimationCounter() == 1)
			Render2DMesh(meshList[GEO_TILEHERO_FRAME1], false, 1, theHero->GetPos_x(), theHero->GetPos_y(), false, true);
		else if (theHero->GetAnimationCounter() == 2)
			Render2DMesh(meshList[GEO_TILEHERO_FRAME2], false, 1, theHero->GetPos_x(), theHero->GetPos_y(), false, true);
		else if (theHero->GetAnimationCounter() == 3)
			Render2DMesh(meshList[GEO_TILEHERO_FRAME3], false, 1, theHero->GetPos_x(), theHero->GetPos_y(), false, true);
		else
			Render2DMesh(meshList[GEO_TILEHERO_FRAME0], false, 1, theHero->GetPos_x(), theHero->GetPos_y(), false, true);
	}

	// Render the enemy
	int theEnemy_x = theEnemy->GetPos_x() - theHero->GetMapFineOffset_x();
	int theEnemy_y = theEnemy->GetPos_y();
	if (((theEnemy_x >= 0) && (theEnemy_x<800)) &&
		((theEnemy_y >= 0) && (theEnemy_y<600)))
	{
		Render2DMesh(meshList[GEO_TILEENEMY_FRAME0], false, 1, theEnemy_x, theEnemy_y);
	}
}


void CSceneManager2D::RenderRearTileMap()
{
	rearWallOffset_x = (int)(theHero->GetMapOffset_x() / 2);
	rearWallOffset_y = 0;
	rearWallTileOffset_y = 0;
	rearWallTileOffset_x = (int)(rearWallOffset_x / m_cRearMap->GetTileSize());
	if (rearWallTileOffset_x + m_cRearMap->GetNumOfTiles_Width() > m_cRearMap->getNumOfTiles_MapWidth())
		rearWallTileOffset_x = m_cRearMap->getNumOfTiles_MapWidth() - m_cRearMap->GetNumOfTiles_Width();
	rearWallFineOffset_x = rearWallOffset_x % m_cRearMap->GetTileSize();

	int m = 0;
	for (int i = 0; i < m_cRearMap->GetNumOfTiles_Height(); i++)
	{
		for (int k = 0; k < m_cRearMap->GetNumOfTiles_Width() + 1; k++)
		{
			m = rearWallTileOffset_x + k;
			// If we have reached the right side of the Map, then do not display the extra column of tiles.
			if ((rearWallTileOffset_x + k) >= m_cRearMap->getNumOfTiles_MapWidth())
				break;
			if (m_cRearMap->theScreenMap[i][m] == 3)
			{
				Render2DMesh(meshList[GEO_TILESTRUCTURE], false, 1, k*m_cRearMap->GetTileSize() - rearWallFineOffset_x, 575 - i*m_cRearMap->GetTileSize());
			}
		}
	}
}


void CSceneManager2D::RenderGoodies()
{
	// Render the goodies
	for (int i = 0; i<10; i++)
	{
		Render2DMesh(theArrayOfGoodies[i]->GetMesh(), false, 1, theArrayOfGoodies[i]->GetPos_x(), theArrayOfGoodies[i]->GetPos_y());
	}
}
*/