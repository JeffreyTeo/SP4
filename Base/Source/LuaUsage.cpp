#include "LuaUsage.h"


LuaUsage::LuaUsage(void) 
:LuaState(NULL)
{
}

LuaUsage::~LuaUsage(void)
{
}

void LuaUsage::LuaUsageInit(string LuaFileName)
{
	LuaState = lua_open();
	luaL_openlibs(LuaState);

	if (luaL_loadfile(LuaState, LuaFileName.c_str()) || lua_pcall(LuaState, 0, 0, 0))
	{
		LuaErrorLog(LuaFileName);
		exit(EXIT_FAILURE);
	}
}

void LuaUsage::LuaUsageClose()
{
	lua_close(LuaState);
}

int LuaUsage::GetArrayValue(string Values, int index)
{
	int returnvalue = 0;
	int ReturnIndication = 0;
	lua_getglobal(LuaState, Values.c_str());
	if (lua_isnil(LuaState, -1))
	{
	}
	lua_pushnil(LuaState);
	while (lua_next(LuaState, -2)){
		returnvalue = (int)lua_tonumber(LuaState, -1);
		ReturnIndication++;
		lua_pop(LuaState, 1);
		if (ReturnIndication == index)
			break;
	}
	int n = lua_gettop(LuaState);
	lua_pop(LuaState, n);
	return returnvalue;
}
float LuaUsage::GetFloatValue(string Values)
{
	float returnvalue = 0;

	lua_getglobal(LuaState, Values.c_str());
	if (!lua_isnumber(LuaState, -1))
	{
		LuaErrorLog(Values);
		exit(EXIT_FAILURE);
	}
	returnvalue = (float)lua_tonumber(LuaState, -1);
	int n = lua_gettop(LuaState);
	lua_pop(LuaState, n);
	return returnvalue;
}
int LuaUsage::GetIntegerValue(string Values)
{
	int returnvalue = 0;

	lua_getglobal(LuaState, Values.c_str());
	if (!lua_isnumber(LuaState, -1))
	{
		LuaErrorLog(Values);
		exit(EXIT_FAILURE);
	}
	returnvalue = (int)lua_tonumber(LuaState, -1);
	int n = lua_gettop(LuaState);
	lua_pop(LuaState, n);
	return returnvalue;
}

bool LuaUsage::GetBooleanValue(string Values)
{
	bool returnvalue = false;

	lua_getglobal(LuaState, Values.c_str());
	if (!lua_isboolean(LuaState, -1))
	{
		LuaErrorLog(Values);
		exit(EXIT_FAILURE);
	}
	returnvalue = (bool)lua_toboolean(LuaState, -1);
	int n = lua_gettop(LuaState);
	lua_pop(LuaState, n);
	return returnvalue;
}

string LuaUsage::GetStringValue(string Values)
{
	string returnvalue = "";

	lua_getglobal(LuaState, Values.c_str());
	if (!lua_isstring(LuaState, -1))
	{
		LuaErrorLog(Values);
		exit(EXIT_FAILURE);
	}
	returnvalue = (string)lua_tostring(LuaState, -1);
	int n = lua_gettop(LuaState);
	lua_pop(LuaState, n);
	return returnvalue;
}

char LuaUsage::GetCharacterValue(string Values)
{
	string holder = "";
	lua_getglobal(LuaState, Values.c_str());
	if (!lua_isnumber(LuaState, -1))
	{
		LuaErrorLog(Values);
		exit(EXIT_FAILURE);
	}
	holder = (char)lua_tostring(LuaState, -1);
	return 0;
}

void LuaUsage::LuaErrorLog(string Errorstring)
{
	ofstream Error("Lua/ErrorLog.lua");
	if (Error.is_open())
	{
		Error << "--ErrorLog--" << endl;
		Error << "Error with" << endl;
		Error << Errorstring << endl;
		Error.close();
	}
}