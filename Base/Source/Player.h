#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include "LuaUsage.h"

using namespace std;



class Player
{
	enum LevelType
	{
		Easylevel = 0,
		Normallevel,
		Hardlevel,

		TotalLevel,
	};
public:
	Player(void);
	~Player(void);

	void PlayerInit(string PlayerFileName);

	void SetAmtOfClearedLevelEasy(short AmtOfClearedLevelEasy);
	short GetAmtOfClearedLevelEasy();
	void SetAmtOfClearedLevelNormal(short AmtOfClearedLevelNormal);
	short GetAmtOfClearedLevelNormal();
	void SetAmtOfClearedLevelHard(short AmtOfClearedLevelHard);
	short GetAmtOfClearedLevelHard();

	void SetEasyLevelUnlocked(bool EasyLevelUnlocked);
	bool GetEasyLevelUnlocked();
	void SetNormalLevelUnlocked(bool NormalLevelUnlocked);
	bool GetNormalLevelUnlocked();
	void SetHardLevelUnlocked(bool HardLevelUnlocked);
	bool GetHardLevelUnlocked();

	void SetAmtOfLevelDiff(short AmtOfLevelDiff);
	short GetAmtOfLevelDiff();
private:
	short AmtOfLevelDiff;
	short AmtOfClearedLevelEasy;
	bool EasyLevelUnlocked;
	short AmtOfClearedLevelNormal;
	bool NormalLevelUnlocked;
	short AmtOfClearedLevelHard;
	bool HardLevelUnlocked;
	LuaUsage* thePlayerinfoLua;
};
#endif