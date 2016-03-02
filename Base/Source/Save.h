#ifndef SAVE_H
#define SAVE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "Player.h"
#include "Shop.h"
#include "LevelDetails.h"
#include "AllLevelDetails.h"

using namespace std;

class Save
{
public:
	Save(void);
	~Save(void);

	void SaveEveryThing(int FileNumber);

	string BoolToStringConversion(bool convert);
	void SaveLevelStuff(vector<AllLevelDetails*> theLevelDetailsHolder,int maxlevel,int maxdifficulty);
	void SavePlayer(Player* playerinfo);
	void SaveMusic(float sound);
	void SaveShop(Shop* shopinfo);

	string OpenTable(string text);
	string SaveTableIndiv(string text, int integertosave = 0, string booltosave = "", string stringtosave = "");
	string CloseTable(bool close = false);
	void SaveIndividual();
	//void SaveLevel(/*Level* Levelinfo*/);
private:
};
#endif