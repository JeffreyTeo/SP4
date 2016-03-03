#ifndef ALLLEVELDETAILS_H
#define ALLLEVELDETAILS_H

#include <iostream>
#include <string>
#include "LuaUsage.h"

using namespace std;

class AllLevelDetails
{
public:
	AllLevelDetails(void);
	~AllLevelDetails(void);

	void AllLevelDetailsInit(string Varfilepath);

	bool GetCleared();
	void SetCleared(bool m_Cleared);

	short GetCollectedKeys();
	void SetCollectedKeys(int m_CollectedKeys);
	
	int GetHighscore();
	void SetHighscore(int m_Highscore);
private:
	bool m_Cleared;
	short m_CollectedKeys;
	int m_Highscore;

	
};
#endif