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
	ofstream SaveFile("Test/Player.lua");
	if (SaveFile.is_open())
	{
		SaveFile << "--Player Save File--" << endl;
		SaveFile << "EASYLEVELSCLEARED = " << playerinfo->GetAmtOfClearedLevelEasy() << endl;
		SaveFile << "NORMALLEVELSCLEARED = " << playerinfo->GetAmtOfClearedLevelNormal() << endl;
		SaveFile << "HARDLEVELSCLEARED = " << playerinfo->GetAmtOfClearedLevelHard() << endl;
		SaveFile << "EASYLEVELUNLOCKED = " << BoolToStringConversion(playerinfo->GetEasyLevelUnlocked()) << endl;
		SaveFile << "NORMALLEVELUNLOCKED = " << BoolToStringConversion(playerinfo->GetNormalLevelUnlocked()) << endl;
		SaveFile << "HARDLEVELUNLOCKED = " << BoolToStringConversion(playerinfo->GetHardLevelUnlocked()) << endl;
		SaveFile.close();
	}
}