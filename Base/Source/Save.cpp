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
void Save::SaveLevelKeys(LevelDetails* Levelinfo)
{
	ofstream SaveFile("Lua/LeveltoSave.lua");
	if (SaveFile.is_open())
	{
		SaveFile << "--Level Template--" << endl;
		SaveFile << "--LevelNumber = { Key }--" << endl;
		SaveFile << "Level = {" << endl;
		//Create a class with 3 vector
		//Should only accept money,highscore
		SaveFile << "Easy = {" << endl;
		for (int i = 0; i < 5; ++i)
		{
			string itemnumber = "Level";
			itemnumber = itemnumber + to_string(i + 1);
			SaveFile << OpenTable(itemnumber) << endl;
			SaveFile << SaveTableIndiv("CollectedKeys",Levelinfo->GetCollectedKeys()) << endl;
			if (i == 4)
				SaveFile << CloseTable(true) << endl;
			else
				SaveFile << CloseTable() << endl;
		}
		SaveFile << "}" << endl;

		SaveFile << "Normal = {" << endl;
		for (int i = 0; i < 5; ++i)
		{
			string itemnumber = "Level";
			itemnumber = itemnumber + to_string(i + 1);
			SaveFile << OpenTable(itemnumber) << endl;
			SaveFile << SaveTableIndiv("CollectedKeys", Levelinfo->GetCollectedKeys()) << endl;
			if (i == 4)
				SaveFile << CloseTable(true) << endl;
			else
				SaveFile << CloseTable() << endl;
		}
		SaveFile << "}" << endl;

		SaveFile << "Hard = {" << endl;
		for (int i = 0; i < 5; ++i)
		{
			string itemnumber = "Level";
			itemnumber = itemnumber + to_string(i + 1);
			SaveFile << OpenTable(itemnumber) << endl;
			SaveFile << SaveTableIndiv("CollectedKeys", Levelinfo->GetCollectedKeys()) << endl;
			if (i == 4)
				SaveFile << CloseTable(true) << endl;
			else
				SaveFile << CloseTable() << endl;
		}
		SaveFile << "}" << endl;

		SaveFile << "}" << endl;
	}
}

void Save::SaveMusic(float sound)
{
	ofstream SaveFile("Lua/Sound.lua");
	if (SaveFile.is_open())
	{
		SaveFile << "Volume = " << sound << endl;
	}
}
void Save::SaveShop(Shop* shopinfo)
{
	ofstream SaveFile("Lua/Item.lua");
	if (SaveFile.is_open())
	{
		SaveFile << "--Item Save File--" << endl;
		SaveFile << "Amount = " << shopinfo->theItemHolder.size() << endl;
		SaveFile << "Item = {" << endl;
		
		for (int i = 0; i < shopinfo->theItemHolder.size(); ++i)
		{
			string itemnumber = "Number";
			itemnumber = itemnumber + to_string(i + 1);
			SaveFile << OpenTable(itemnumber) << endl;
			SaveFile << SaveTableIndiv("Name", 0, "",shopinfo->theItemHolder[i]->GetName()) << ",";
			SaveFile << SaveTableIndiv("Bought", 0, BoolToStringConversion(shopinfo->theItemHolder[i]->GetBought()),"") << ",";
			SaveFile << SaveTableIndiv("Price", shopinfo->theItemHolder[i]->GetPrice()) << endl;
			if (i == shopinfo->theItemHolder.size()-1)
				SaveFile << CloseTable(true) << endl;
			else
				SaveFile << CloseTable() << endl;
		}
		SaveFile << "}" << endl;
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
		SaveFile << "LevelToStartAt = " << playerinfo->GetLevelToStartAt() << endl;
		SaveFile << "LevelDifficultyToStartAt = " << playerinfo->GetLevelToDifficultyStartAt() << endl;
		SaveFile << "LevelStopAt = " << playerinfo->GetLevelStopAt() << endl;
		SaveFile << "LevelDifficultyStopAt = " << playerinfo->GetLevelDifficultyStopAt() << endl;
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
string Save::SaveTableIndiv(string text, int integertosave, string booltosave, string stringtosave)
{
	string returnstring = text;
	if (booltosave == "" && stringtosave == "")
		returnstring = returnstring + " = " + to_string(integertosave);
	else if (stringtosave == "")
		returnstring = returnstring + " = " + booltosave;
	else
		returnstring = returnstring + " = \"" + stringtosave + "\"";
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