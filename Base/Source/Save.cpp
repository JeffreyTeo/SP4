#include "Save.h"
Save::Save(void)
{

}

Save::~Save(void)
{
}

string Save::BoolToStringConversion(bool convert)
{
	string ReturnString = "";
	if (convert)
		ReturnString = "true";
	else
		ReturnString = "false";
	return ReturnString;
}
void Save::SaveMusic(float sound)
{
	ofstream SaveFile("Lua/Sound.lua");
	if (SaveFile.is_open())
	{
		SaveFile << "Volume = " << sound << endl;
	}
}
void Save::SavePlayer(Player* playerinfo)
{
	ofstream SaveFile("Lua/Player.lua");
	if (SaveFile.is_open())
	{
		SaveFile << "--Player Save File--" << endl;
		SaveFile << "LevelAmt = " << playerinfo->GetAmtOfLevelDiff() << endl;
		SaveFile << "Currency = " << playerinfo->GetAmtOfCurrency() << endl;
		SaveFile << "Player = {" << endl;
		SaveFile << OpenTable("EASYLEVEL") << endl;
		SaveFile << SaveTableIndiv("LevelCleared", playerinfo->GetAmtOfClearedLevelEasy()) << ",";
		SaveFile << SaveTableIndiv("LevelUnlocked", 0, BoolToStringConversion(playerinfo->GetEasyLevelUnlocked())) << endl;
		SaveFile << CloseTable() << endl;
		SaveFile << OpenTable("NORMALLEVEL") << endl;
		SaveFile << SaveTableIndiv("LevelCleared", playerinfo->GetAmtOfClearedLevelNormal()) << ",";
		SaveFile << SaveTableIndiv("LevelUnlocked", 0, BoolToStringConversion(playerinfo->GetNormalLevelUnlocked())) << endl;
		//SaveFile << "		LevelCleared = " << playerinfo->GetAmtOfClearedLevelNormal() << "," << "LevelUnlocked = " << BoolToStringConversion(playerinfo->GetNormalLevelUnlocked()) << endl;
		SaveFile << CloseTable() << endl;
		SaveFile << OpenTable("HARDLEVEL") << endl; 
		SaveFile << SaveTableIndiv("LevelCleared", playerinfo->GetAmtOfClearedLevelHard()) << ",";
		SaveFile << SaveTableIndiv("LevelUnlocked", 0, BoolToStringConversion(playerinfo->GetHardLevelUnlocked())) << endl;
		//SaveFile << "		LevelCleared = " << playerinfo->GetAmtOfClearedLevelHard() << "," << "LevelUnlocked = " << BoolToStringConversion(playerinfo->GetHardLevelUnlocked()) << endl;
		SaveFile << CloseTable(true) << endl;
		SaveFile << "}"  << endl;
		SaveFile.close();
	}
}
string Save::SaveTableIndiv(string text, int integertosave, string stringtosave)
{
	string returnstring = text;
	if (stringtosave == "")
		returnstring = returnstring + " = " + to_string(integertosave);
	else
		returnstring = returnstring + " = " + stringtosave;
	return returnstring;
}
string Save::OpenTable(string text)
{
	string returnstring = text + " = {";
	return returnstring;
}
string Save::CloseTable(bool close)
{
	if (!close)
		return "},";
	else
		return "}";
}