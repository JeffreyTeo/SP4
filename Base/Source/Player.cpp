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
	this->AmtOfClearedLevelEasy = thePlayerinfoLua->GetIntegerValue("EASYLEVELSCLEARED");
	this->AmtOfClearedLevelNormal = thePlayerinfoLua->GetIntegerValue("NORMALLEVELSCLEARED");
	this->AmtOfClearedLevelHard = thePlayerinfoLua->GetIntegerValue("HARDLEVELSCLEARED");
	this->EasyLevelUnlocked = thePlayerinfoLua->GetBooleanValue("EASYLEVELUNLOCKED");
	this->NormalLevelUnlocked = thePlayerinfoLua->GetBooleanValue("NORMALLEVELUNLOCKED"); 
	this->HardLevelUnlocked = thePlayerinfoLua->GetBooleanValue("HARDLEVELUNLOCKED");
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