#include "SceneManager2DforScreen.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "Strategy_Kill.h"

SceneManagerLevel2DforScreen::SceneManagerLevel2DforScreen()
: m_player(NULL)
, m_save(NULL)
, m_shop(NULL)
, m_particle(NULL)
, m_particle2(NULL)
, tempsound(0)
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

SceneManagerLevel2DforScreen::SceneManagerLevel2DforScreen(const int m_window_width, const int m_window_height, const int m_screenvalue)
: m_player(NULL)
, m_save(NULL)
, m_shop(NULL)
, m_particle(NULL)
, m_particle2(NULL)
, tempsound(0)
{
	this->m_screenvalue = m_screenvalue;
	this->m_windowWidth = m_window_width;
	this->m_windowHeight = m_window_height;
}

SceneManagerLevel2DforScreen::~SceneManagerLevel2DforScreen()
{
	
	if (m_shop)
	{
		delete m_shop;
		m_shop = NULL;
	}
	if (m_player)
	{
		delete m_player;
		m_player = NULL;
	}
	if (m_loading)
	{
		delete m_loading;
		m_loading = NULL;
	}

	if (m_save)
	{
		delete m_save;
		m_save = NULL;
	}

}

void SceneManagerLevel2DforScreen::PreInit()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

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

void SceneManagerLevel2DforScreen::Init()
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
	meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//sky_background.tga");
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
	meshList[GEO_TILEENEMY_FRAME0] = MeshBuilder::Generate2DMesh("GEO_TILEENEMY_FRAME0", Color(1, 1, 1), 0, 0, 25, 25);
	meshList[GEO_TILEENEMY_FRAME0]->textureID = LoadTGA("Image//tile20_enemy.tga");

	meshList[GEO_MENU] = MeshBuilder::Generate2DMesh("GEO_MENU", Color(1, 1, 1,0.0f), 0, 0, 800, 600);
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
	meshList[GEO_PAUSE] = MeshBuilder::Generate2DMesh("GEO_PAUSE", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_PAUSE]->textureID = LoadTGA("Image//Pause.tga");
	meshList[GEO_SHOP] = MeshBuilder::Generate2DMesh("GEO_SHOP", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_SHOP]->textureID = LoadTGA("Image//Shop.tga");
	meshList[GEO_WIN] = MeshBuilder::Generate2DMesh("GEO_WIN", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_WIN]->textureID = LoadTGA("Image//Win.tga");
	meshList[GEO_LEVELSHOPSELECT] = MeshBuilder::Generate2DMesh("GEO_LEVELSHOPSELECT", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_LEVELSHOPSELECT]->textureID = LoadTGA("Image//LevelShopSelect.tga");
	
	meshList[GEO_LEVELSELECT] = MeshBuilder::Generate2DMesh("GEO_LEVELSELECT", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_LEVELSELECT]->textureID = LoadTGA("Image//LevelSelect.tga");

	meshList[GEO_SELECT] = MeshBuilder::Generate2DMesh("GEO_SELECT", Color(1, 1, 1), 0, 0, 75, 55);
	meshList[GEO_SELECT]->textureID = LoadTGA("Image//Select.tga");


	if (m_screenvalue == Winscreen)
	{
		m_SpriteAnimationLoad = new LuaUsage();
		m_SpriteAnimationLoad->LuaUsageInit("Sprite");
		confettiRightside = false;
		m_particle = new Particle();
		m_particle2 = new Particle();

		SetParticleStyle(m_particle, PARTICLE_STYLE::DROPDOWN);
		SetParticleStyle(m_particle2, PARTICLE_STYLE::DROPDOWN);
		//SetParticleStyle(m_particle, PARTICLE_STYLE::CONFETTI);
		//SetParticleStyle(m_particle2, PARTICLE_STYLE::CONFETTI);

		Math::InitRNG();
		for (int i = 0; i < m_particle->GetSize(); i++)
		{
			SetSpriteAnimation(m_particle, i);
		}

		for (int i = 0; i < m_particle2->GetSize(); i++)
		{
			SetSpriteAnimation(m_particle2, i);
		}
		m_SpriteAnimationLoad->LuaUsageClose();

	}

	m_select = -1;
	m_alpha = 0.0f;
	m_ChangeScreen = false;
	m_ScreenTransition = true;
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	m_save = new Save();
	rotateAngle = 0;
	m_loading = new LuaUsage();
	m_loading->LuaUsageInit("Sound");
	tempsound = m_loading->get<float>("Volume");
	m_loading->LuaUsageClose();
	

	if (m_screenvalue == Shopscreen)
	{
		m_player = new Player();
		m_player->PlayerInit("Player");
		m_shop = new Shop();
		m_shop->PlayerInit(m_player);
		m_shop->ItemInit();
	}
	
	if (m_screenvalue == Highscorescreen)
		AddHighscore();
}

void SceneManagerLevel2DforScreen::AddHighscore()
{
	const int MAX_SCORES = 5;
	string values[MAX_SCORES];
	for (int i = 0; i < MAX_SCORES; i++)
	{
		theScore[i].ReadTextFile("highscore.txt");
	}
}
void SceneManagerLevel2DforScreen::SetChangeScreen(bool m_ChangeScreen)
{
	this->m_ChangeScreen = m_ChangeScreen;
}
void SceneManagerLevel2DforScreen::SetSelection(short m_select)
{
	this->m_select = m_select;
}
void SceneManagerLevel2DforScreen::SetScreenTransition(bool m_ScreenTransition)
{
	this->m_ScreenTransition = m_ScreenTransition;
}
bool SceneManagerLevel2DforScreen::ReturnScreenTransition()
{
	return this->m_ScreenTransition;
}
bool SceneManagerLevel2DforScreen::ReturnChangeScreen()
{
	return this->m_ChangeScreen;
}


void SceneManagerLevel2DforScreen::SetParticleStyle(Particle *ParticleVector, int ParticleStyle)
{

	if (ParticleStyle == PARTICLE_STYLE::DROPDOWN)
		ParticleVector->ParticleInit(20, 0, m_windowHeight, PARTICLE_STYLE::DROPDOWN);
	else if (ParticleStyle == PARTICLE_STYLE::CONFETTI && confettiRightside == false)
	{
		ParticleVector->ParticleInit(20, m_windowWidth * 0.125, m_windowHeight* 0.5, PARTICLE_STYLE::CONFETTI);
		confettiRightside = true;
	}
	else if (ParticleStyle == PARTICLE_STYLE::CONFETTI && confettiRightside)
	{
		ParticleVector->ParticleInit(20, m_windowWidth * 0.875, m_windowHeight* 0.5, PARTICLE_STYLE::CONFETTI);
		ParticleVector->SetConfettiRightSide(confettiRightside);
	}

}

void SceneManagerLevel2DforScreen::SetSpriteAnimation(Particle *ParticleVector, int SAIndex)
{
	/*meshList[GEO_SPRITE_ANIMATION] = MeshBuilder::GenerateSpriteAnimation("star", 6, 3);
	meshList[GEO_SPRITE_ANIMATION]->textureID = LoadTGA("Image//StarSprite.tga");
	m_spriteAnimation = dynamic_cast<SpriteAnimation*>(meshList[GEO_SPRITE_ANIMATION]);
	m_spriteAnimation->m_anim = new Animation();
	if (m_spriteAnimation)
	{
	Math::InitRNG();
	m_spriteAnimation->m_anim->Set(0, 16, 0, 0.5f);
	//m_spriteAnimation->x = Math::RandIntMinMax(100,700);//64;
	//	m_spriteAnimation->y = 600 + Math::RandIntMinMax(100, 200);

	m_spriteAnimation->x = m_particle->GetX();
	m_spriteAnimation->y = m_particle->GetY();

	m_spriteAnimation->speed = Math::RandIntMinMax(100, 800);
	m_spriteAnimation->index = i;
	m_particle->SpritePushBack(m_spriteAnimation, /*0*///m_spriteAnimation->y + Math::RandIntMinMax(-600, 300), 400/*0*/);
	/*m_particle->SpritePushBack(m_spriteAnimation, 0, 0, i);*/
	//}
	meshList[GEO_SPRITE_ANIMATION] = MeshBuilder::GenerateSpriteAnimation("star", Color(), m_SpriteAnimationLoad->get<int>("StarRow"), m_SpriteAnimationLoad->get<int>("StarCol"));
	meshList[GEO_SPRITE_ANIMATION]->textureID = LoadTGA("Image//StarSprite.tga");
	m_spriteAnimation = dynamic_cast<SpriteAnimation*>(meshList[GEO_SPRITE_ANIMATION]);
	m_spriteAnimation->m_anim = new Animation();
	if (m_spriteAnimation)
	{
		m_spriteAnimation->m_anim->Set(0, 18, 0, Math::RandFloatMinMax((m_SpriteAnimationLoad->get<float>("StarMinTime")), (m_SpriteAnimationLoad->get<float>("StarMaxTime"))));
		if (ParticleVector->Getparticlestyle() == PARTICLE_STYLE::DROPDOWN)
		{
			m_spriteAnimation->x = m_spriteAnimation->x + Math::RandIntMinMax(m_windowWidth*0.125, m_windowWidth - m_windowWidth*0.125);//64;
			m_spriteAnimation->y = ParticleVector->GetY() + Math::RandIntMinMax(m_windowWidth*0.125, m_windowWidth*0.25);
			m_spriteAnimation->speed = Math::RandIntMinMax(m_windowWidth*0.125, m_windowWidth);
			m_spriteAnimation->index = SAIndex;
			ParticleVector->SpritePushBack(m_spriteAnimation, 0, 0);
		}
		else if (ParticleVector->Getparticlestyle() == PARTICLE_STYLE::CONFETTI)
		{
			m_spriteAnimation->x = ParticleVector->GetX();
			m_spriteAnimation->y = ParticleVector->GetY();
			m_spriteAnimation->speed = Math::RandIntMinMax(m_windowWidth*0.125, m_windowWidth);
			m_spriteAnimation->index = SAIndex;
			ParticleVector->SpritePushBack(m_spriteAnimation, m_spriteAnimation->y + Math::RandIntMinMax(0, m_windowWidth*0.125 + m_windowWidth*0.25), m_windowWidth*0.5);
		}
	}
}

void SceneManagerLevel2DforScreen::Update(double dt)
{
	//cout << m_player->GetAmtOfClearedLevelEasy() << " " << m_player->GetAmtOfClearedLevelNormal() << " " << m_player->GetAmtOfClearedLevelHard();
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if (Application::IsKeyPressed('5'))
		m_alpha += 0.05f;
	if (Application::IsKeyPressed('6'))
		m_alpha -= 0.05f;

	if (m_screenvalue == Winscreen)
	{
		m_particle->Update(dt);
		m_particle2->Update(dt);
	}
	
	if (m_ScreenTransition && m_ChangeScreen)
	{
		m_alpha -= 0.04f;
		if (m_alpha < 0)
		{
			m_ScreenTransition = false;
		}
	}
	else if (m_ScreenTransition)
	{
		m_alpha += 0.04f;
		if (m_alpha >= 1.0f)
			m_ScreenTransition = false;
	}


	rotateAngle -= (float)Application::camera_yaw;// += (float)(10 * dt);

	//cout << Sound.volume << endl;

	camera.Update(dt);
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
	*/


	fps = (float)(1.f / dt);
}

/********************************************************************************
 Update Camera position
 ********************************************************************************/
void SceneManagerLevel2DforScreen::UpdateCameraStatus(const unsigned char key, const bool status)
{
	//camera.UpdateStatus(key, status);

	// Update avatar position
}

/********************************************************************************
 Update Weapon status
 ********************************************************************************/
void SceneManagerLevel2DforScreen::UpdateMouseStatus(const unsigned char key)
{
}

/********************************************************************************
 Render text onto the screen
 ********************************************************************************/
void SceneManagerLevel2DforScreen::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, bool enablealpha)
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
void SceneManagerLevel2DforScreen::Render2DMesh(Mesh *mesh, bool enableLight, bool enablealpha, int size, int x, int y, bool rotate, bool flip)
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
void SceneManagerLevel2DforScreen::RenderBackground()
{
	// Render the crosshair
	Render2DMesh(meshList[GEO_BACKGROUND], false, false, 1);
}

/********************************************************************************
 Render this scene
 ********************************************************************************/
void SceneManagerLevel2DforScreen::Render()
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

	switch (m_screenvalue)
	{
	case Menuscreen:
	{
					   RenderMainMenu();
					   break;
	}
	case Instructionscreen:
	{
							  RenderInstructions();
							  break;
	}
	case Highscorescreen:
	{
							RenderHighscore();
							break;
	}
	case Optionscreen:
	{
						 RenderOption();
						 break;
	}
	case LevelShopSelectionscreen:
	{
									 RenderLevelShopSelect();
									 break;
	}
	case Shopscreen:
	{
					   RenderShop();
						break;
	}
	case LevelSelectscreen:
	{
							  RenderLevelSelect();
							   break;
	}
	case Pausescreen:
	{
						RenderPause();
						 break;
	}
	case Winscreen:
	{
					  RenderWin();
					  break;
	}
	}
	

	/*std::ostringstream ss;
	ss.precision(5);
	ss << "test text: " << " 9999999 ";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 6 ,false);*/
}

void SceneManagerLevel2DforScreen::RenderPause()
{
	modelStack.PushMatrix();
	Render2DMesh(meshList[GEO_PAUSE], false, true);
	RenderTextOnScreen(meshList[GEO_TEXT], "", Color(1, 1, 1, m_alpha), 30, 0, 6, true);
	modelStack.PopMatrix();
	switch (m_select)
	{
	case 1:
	{
			  modelStack.PushMatrix();
			  Render2DMesh(meshList[GEO_SELECT], false, true, 1.5, 230, 275);
			  modelStack.PopMatrix();
			  break;
	}
	case 2:
	{
			  modelStack.PushMatrix();
			  Render2DMesh(meshList[GEO_SELECT], false, true, 1.5, 120, 220);
			  modelStack.PopMatrix();
			  break;
	}
	}
}
void SceneManagerLevel2DforScreen::RenderShop()
{
	modelStack.PushMatrix();
	Render2DMesh(meshList[GEO_SHOP], false, true);
	modelStack.PopMatrix();
	for (int i = 0; i < m_shop->theItemHolder.size(); i++)
	{
		modelStack.PushMatrix();
		string Naming = "Name: " + m_shop->theItemHolder[i]->GetName();
		RenderTextOnScreen(meshList[GEO_TEXT], Naming, Color(0, 0, 1), 30, 300, (430 - i * 75), true);
		string pricing = "Cost: " + std::to_string(m_shop->theItemHolder[i]->GetPrice());
		RenderTextOnScreen(meshList[GEO_TEXT], pricing, Color(0, 0, 1), 30, 300, (410 - i * 75), true);
		if (m_shop->theItemHolder[i]->GetBought())
			RenderTextOnScreen(meshList[GEO_TEXT], "bought", Color(0, 0, 1), 30, 300, (390 - i * 75), true);
		else
			RenderTextOnScreen(meshList[GEO_TEXT], "nope", Color(0, 0, 1), 30, 300, (390 - i * 75), true);
		modelStack.PopMatrix();
	}
	RenderTextOnScreen(meshList[GEO_TEXT], "", Color(1, 1, 1, m_alpha), 30, 0, 6, true);
}
void SceneManagerLevel2DforScreen::RenderWin()
{
	modelStack.PushMatrix();
	Render2DMesh(meshList[GEO_WIN], false, true);
	RenderTextOnScreen(meshList[GEO_TEXT], "", Color(1, 1, 1, m_alpha), 30, 0, 6, true);
	modelStack.PopMatrix();

	for (int i = 0; i < m_particle->GetSize(); i++)
	{
		modelStack.PushMatrix();
		Render2DMesh(m_particle->theSpriteHolder[i], false, true, 50, m_particle->theSpriteHolder[i]->x, m_particle->theSpriteHolder[i]->y);
		Render2DMesh(m_particle2->theSpriteHolder[i], false, true, 50, m_particle2->theSpriteHolder[i]->x, m_particle2->theSpriteHolder[i]->y);
		modelStack.PopMatrix();
	}
}
void SceneManagerLevel2DforScreen::RenderLevelShopSelect()
{
	modelStack.PushMatrix();
	Render2DMesh(meshList[GEO_LEVELSHOPSELECT], false, true);
	RenderTextOnScreen(meshList[GEO_TEXT], "", Color(1, 1, 1, m_alpha), 30, 0, 6, true);
	modelStack.PopMatrix();

	switch (m_select)
	{
	case 1:
	{
			  modelStack.PushMatrix();
			  Render2DMesh(meshList[GEO_SELECT], false, true, 1.5, 120, 275);
			  modelStack.PopMatrix();
			  break;
	}
	case 2:
	{
			  modelStack.PushMatrix();
			  Render2DMesh(meshList[GEO_SELECT], false, true, 1.5, 250, 220);
			  modelStack.PopMatrix();
			  break;
	}
	}
}
void SceneManagerLevel2DforScreen::RenderLevelSelect()
{
	modelStack.PushMatrix();
	Render2DMesh(meshList[GEO_LEVELSELECT], false, true);
	RenderTextOnScreen(meshList[GEO_TEXT], "", Color(1, 1, 1, m_alpha), 30, 0, 6, true);
	modelStack.PopMatrix();
}
void SceneManagerLevel2DforScreen::RenderMainMenu()
{
	modelStack.PushMatrix();
	Render2DMesh(meshList[GEO_MENU], false, true);
	RenderTextOnScreen(meshList[GEO_TEXT], "", Color(1, 1, 1, m_alpha), 30, 0, 6, true);
	modelStack.PopMatrix();
	switch (m_select)
	{
	case 1:
	{
			  modelStack.PushMatrix();
			  Render2DMesh(meshList[GEO_SELECT], false, true, 1.5, 250, 275);
			  modelStack.PopMatrix();
			  break;
	}
	case 2:
	{
			  modelStack.PushMatrix();
			  Render2DMesh(meshList[GEO_SELECT], false, true, 1.5, 120, 220);
			  modelStack.PopMatrix();
			  break;
	}
	case 3:
	{
			  modelStack.PushMatrix();
			  Render2DMesh(meshList[GEO_SELECT], false, true, 1.5, 180, 160);
			  modelStack.PopMatrix();
			  break;
	}
	case 4:
	{
			  modelStack.PushMatrix();
			  Render2DMesh(meshList[GEO_SELECT], false, true, 1.5, 210, 110);
			  modelStack.PopMatrix();
			  break;
	}
	case 5:
	{
			  modelStack.PushMatrix();
			  Render2DMesh(meshList[GEO_SELECT], false, true, 1.5, 260, 50);
			  modelStack.PopMatrix();
			  break;
	}
	}
}

void SceneManagerLevel2DforScreen::RenderHighscore()
{
	modelStack.PushMatrix();

	Render2DMesh(meshList[GEO_HIGHSCORE], false, true);
	
	std::ostringstream ss;
	const int size = 5;
	for (int i = 0; i < size; i++)
	{
		ss.str(std::string());
		ss << i + 1 << ". " << theScore[i].GetAllHighscores(i);
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 1, m_alpha), 60, 300, 300 - (i * 40), true);
	}
	RenderTextOnScreen(meshList[GEO_TEXT], "", Color(1, 1, 1, m_alpha), 30, 0, 6, true);
	modelStack.PopMatrix();
}

void SceneManagerLevel2DforScreen::RenderOption()
{
	modelStack.PushMatrix();
	Render2DMesh(meshList[GEO_SOUND], false, true);

	if (SoundSelect && !muted)
	{
		Render2DMesh(meshList[GEO_SOUND], false, true);
	}
	else if (SoundSelect && muted)
	{
		Render2DMesh(meshList[GEO_SOUND_MUTE], false, true);
	}
	else if (VolumeSelect && !muted)
	{
		Render2DMesh(meshList[GEO_VOL], false, true);
	}
	else if (VolumeSelect && muted)
	{
		Render2DMesh(meshList[GEO_VOL_MUTE], false, true);
	}
	
	std::ostringstream ssVol;
	ssVol << tempsound;
	RenderTextOnScreen(meshList[GEO_TEXT], ssVol.str(), Color(0, 1, 0), 30, 250, 180, true);

	RenderTextOnScreen(meshList[GEO_TEXT], "", Color(1, 1, 1, m_alpha), 30, 0, 6, true);
	modelStack.PopMatrix();
}

void SceneManagerLevel2DforScreen::RenderInstructions()
{
	modelStack.PushMatrix();
	Render2DMesh(meshList[GEO_INSTRUCTION], false, true);
	RenderTextOnScreen(meshList[GEO_TEXT], "", Color(1, 1, 1, m_alpha), 30, 0, 6, true);
	modelStack.PopMatrix();
}

/********************************************************************************
 Exit this scene
 ********************************************************************************/
void SceneManagerLevel2DforScreen::Exit()
{
	if (m_screenvalue == Optionscreen)
		m_save->SaveMusic(tempsound);


	if (m_screenvalue == Shopscreen)
	{
		m_shop->Set(m_player);
		m_save->SavePlayer(m_player);
	}
	if (m_screenvalue == Winscreen)
	{
		if (m_SpriteAnimationLoad)
		{
			delete m_SpriteAnimationLoad;
			m_SpriteAnimationLoad = NULL;
		}
		if (m_particle)
		{
			delete m_particle;
			m_particle = NULL;
		}
		if (m_particle2)
		{
			delete m_particle2;
			m_particle2 = NULL;
		}
		for (int i = 0; i < NUM_GEOMETRY; ++i)
		{
			if (meshList[i] && i != GEO_SPRITE_ANIMATION)
				delete meshList[i];
		}
	}
	else
	{
		for (int i = 0; i < NUM_GEOMETRY; ++i)
		{
			if (meshList[i])
				delete meshList[i];
		}
	}
	// Cleanup VBO
	
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	
}


/*

void SceneManagerLevel2DforScreen::RenderTileMap()
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


void SceneManagerLevel2DforScreen::RenderRearTileMap()
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


void SceneManagerLevel2DforScreen::RenderGoodies()
{
	// Render the goodies
	for (int i = 0; i<10; i++)
	{
		Render2DMesh(theArrayOfGoodies[i]->GetMesh(), false, 1, theArrayOfGoodies[i]->GetPos_x(), theArrayOfGoodies[i]->GetPos_y());
	}
}
*/