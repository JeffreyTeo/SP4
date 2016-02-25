#ifndef LEVELDETAILS_H
#define LEVELDETAILS_H

#include <iostream>
#include <string>
#include "LuaUsage.h"

using namespace std;

class LevelDetails
{
public:
	LevelDetails(void);
	~LevelDetails(void);

	void LevelDetailsInit(int Difficulty, int LevelinDifficulty, string FilePath);

	float GetPositionXOfGrid();
	float GetPositionYOfGrid();
	float GetLengthXOfAGrid();
	float GetLengthYOfAGrid();
	int GetNumberOfGridX();
	int GetNumberOfGridY();
	int GetPlayerPositionX();
	int GetPlayerPositionY();
	string GetNameOfLevelFile();
	string GetDifficulty();
	bool GetCleared();

	void SetPositionXOfGrid(float m_PositionXOfGrid);
	void SetPositionYOfGrid(float m_PositionYOfGrid);
	void SetLengthXOfAGrid(float m_LengthXOfAGrid);
	void SetLengthYOfAGrid(float m_LengthYOfAGrid);
	void SetNumberOfGridX(int m_NumberOfGridX);
	void SetNumberOfGridY(int m_NumberOfGridY);
	void SetPlayerPositionX(int m_PlayerPositionX);
	void SetPlayerPositionY(int m_PlayerPositionY);
	void SetNameOfLevelFile(string m_NameOfLevelFile);
	void SetDifficulty(string m_Difficulty);
	void SetCleared(bool m_Cleared);


private:
	float m_PositionXOfGrid;
	float m_PositionYOfGrid;
	float m_LengthXOfAGrid;
	float m_LengthYOfAGrid;
	int m_NumberOfGridX;
	int m_NumberOfGridY;
	int m_PlayerPositionX;
	int m_PlayerPositionY;
	string m_NameOfLevelFile;
	string m_Difficulty;
	bool m_Cleared;

	LuaUsage* theLevelDetailsinfoLua;
};
#endif