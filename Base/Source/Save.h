#ifndef SAVE_H
#define SAVE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "Player.h"

using namespace std;

class Save
{
public:
	Save(void);
	~Save(void);

	string BoolToStringConversion(bool convert);
	void SavePlayer(Player* playerinfo);
	void SaveMusic(float sound);

	string OpenTable(string text);
	string SaveTableIndiv(string text, int integertosave = 0, string stringtosave = "");
	string CloseTable(bool close = false);
	void SaveIndividual();
	//void SaveLevel(/*Level* Levelinfo*/);
private:
};
#endif