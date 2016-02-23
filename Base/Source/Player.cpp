#include "Player.h"


Player::Player(void) 
: thePlayerinfoLua(NULL)
, AmtOfClearedLevelEasy(0)
, EasyLevelUnlocked(false)
, AmtOfClearedLevelNormal(0)
, NormalLevelUnlocked(false)
, AmtOfClearedLevelHard(0)
, HardLevelUnlocked(false)
{
}

Player::~Player(void)
{
	if (thePlayerinfoLua)
	{
		delete thePlayerinfoLua;
		thePlayerinfoLua = NULL;
	}
}

void Player::PlayerInit(string PlayerFileName)
{
	thePlayerinfoLua = new LuaUsage();
	thePlayerinfoLua->LuaUsageInit(PlayerFileName);
	AmtOfLevelDiff = thePlayerinfoLua->get<int>("LevelAmt");
	for (int i = 0; i < AmtOfLevelDiff; ++i)
	{
		string GetLevel = PlayerFileName;
		switch (i)
		{
		case Easylevel:
		{
						  GetLevel = GetLevel + ".EASYLEVEL.";
						  string IntLevel = GetLevel + "LevelCleared";
						  string BoolLevel = GetLevel + "LevelUnlocked";
						  this->AmtOfClearedLevelEasy = thePlayerinfoLua->get<int>(IntLevel);
						  this->EasyLevelUnlocked = thePlayerinfoLua->get<bool>(BoolLevel);
						  break;
		}
		case Normallevel:
		{
							GetLevel = GetLevel + ".NORMALLEVEL.";
							string IntLevel = GetLevel + "LevelCleared";
							string BoolLevel = GetLevel + "LevelUnlocked";
							this->AmtOfClearedLevelNormal = thePlayerinfoLua->get<int>(IntLevel);
							this->NormalLevelUnlocked = thePlayerinfoLua->get<bool>(BoolLevel);
							break;
		}
		case Hardlevel:
		{
						  GetLevel = GetLevel + ".HARDLEVEL.";
						  string IntLevel = GetLevel + "LevelCleared";
						  string BoolLevel = GetLevel + "LevelUnlocked";
						  this->AmtOfClearedLevelHard = thePlayerinfoLua->get<int>(IntLevel);
						  this->HardLevelUnlocked = thePlayerinfoLua->get<bool>(BoolLevel);
						  break;
		}
		}
	}
	thePlayerinfoLua->LuaUsageClose();
}

void Player::SetAmtOfClearedLevelEasy(short AmtOfClearedLevelEasy)
{
	this->AmtOfClearedLevelEasy = AmtOfClearedLevelEasy;
}
short Player::GetAmtOfClearedLevelEasy()
{
	return this->AmtOfClearedLevelEasy;
}
void Player::SetAmtOfClearedLevelNormal(short AmtOfClearedLevelNormal)
{
	this->AmtOfClearedLevelNormal = AmtOfClearedLevelNormal;
}
short Player::GetAmtOfClearedLevelNormal()
{
	return this->AmtOfClearedLevelNormal;
}
void Player::SetAmtOfClearedLevelHard(short AmtOfClearedLevelHard)
{
	this->AmtOfClearedLevelHard = AmtOfClearedLevelHard;
}
short Player::GetAmtOfClearedLevelHard()
{
	return this->AmtOfClearedLevelHard;
}

void Player::SetEasyLevelUnlocked(bool EasyLevelUnlocked)
{
	this->EasyLevelUnlocked = EasyLevelUnlocked;
}
bool Player::GetEasyLevelUnlocked()
{
	return this->EasyLevelUnlocked;
}
void Player::SetNormalLevelUnlocked(bool NormalLevelUnlocked)
{
	this->NormalLevelUnlocked = NormalLevelUnlocked;
}
bool Player::GetNormalLevelUnlocked()
{
	return this->NormalLevelUnlocked;
}
void Player::SetHardLevelUnlocked(bool HardLevelUnlocked)
{
	this->HardLevelUnlocked = HardLevelUnlocked;
}
bool Player::GetHardLevelUnlocked()
{
	return this->HardLevelUnlocked;
}
void Player::SetAmtOfLevelDiff(short AmtOfLevelDiff)
{
	this->AmtOfLevelDiff = AmtOfLevelDiff;
}
short Player::GetAmtOfLevelDiff()
{
	return this->AmtOfLevelDiff;
}