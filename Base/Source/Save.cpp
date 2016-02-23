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

void Save::SavePlayer(Player* playerinfo)
{
	ofstream SaveFile("Lua/Player.lua");
	if (SaveFile.is_open())
	{
		SaveFile << "--Player Save File--" << endl;
		SaveFile << "AmtOfLevelDiff = " << playerinfo->GetAmtOfLevelDiff() << endl;
		SaveFile << "Player = {" << endl;
		SaveFile << "		EASYLEVEL = {" << endl;
		SaveFile << "		LevelCleared = " << playerinfo->GetAmtOfClearedLevelEasy() << "," << "LevelUnlocked = " << BoolToStringConversion(playerinfo->GetEasyLevelUnlocked()) << endl;
		SaveFile << "		}," << endl;
		SaveFile << "		NORMALLEVEL = {" << endl;
		SaveFile << "		LevelCleared = " << playerinfo->GetAmtOfClearedLevelNormal() << "," << "LevelUnlocked = " << BoolToStringConversion(playerinfo->GetNormalLevelUnlocked()) << endl;
		SaveFile << "		}," << endl;
		SaveFile << "		HARDLEVEL = {" << endl;
		SaveFile << "		LevelCleared = " << playerinfo->GetAmtOfClearedLevelHard() << "," << "LevelUnlocked = " << BoolToStringConversion(playerinfo->GetHardLevelUnlocked()) << endl;
		SaveFile << "		}," << endl;
		SaveFile << "}"  << endl;
		SaveFile.close();
	}
}