#include "LuaUsage.h"


LuaUsage::LuaUsage(void) 
:LuaState(NULL)
, Errorstring("")
, Filename("")
{
}

LuaUsage::~LuaUsage(void)
{
}

void LuaUsage::LuaUsageInit(string LuaFileName)
{
	Filename = LuaFileName;
	LuaState = lua_open();
	string LuaOpenFilename = "Lua/";
	LuaOpenFilename = LuaOpenFilename + LuaFileName;
	LuaOpenFilename = LuaOpenFilename + ".lua";
	if (luaL_loadfile(LuaState, LuaOpenFilename.c_str()) || lua_pcall(LuaState, 0, 0, 0))
	{
		std::cout << "Error: failed to load (" << LuaFileName << ")" << std::endl;
		LuaState = NULL;
	}
	if (LuaState)
		luaL_openlibs(LuaState);
}

void LuaUsage::Seterrorstring(const string& Variablename, const string& reason)
{
	Errorstring = Errorstring + Variablename + ": " + reason + "\n";
}

void LuaUsage::LuaUsageClose()
{
	if (LuaState)
		lua_close(LuaState);
	if (Errorstring.size() > 1)
	{
		LuaErrorLog();
	}
}
void LuaUsage::clean()
{
	int i = lua_gettop(LuaState);
	lua_pop(LuaState, i);
}
void LuaUsage::LuaErrorLog()
{
	string thefilename = "Lua/";
	thefilename = thefilename + this->Filename;
	thefilename = thefilename + "Error.lua";
	ofstream Error(thefilename);
	if (Error.is_open())
	{
		Error << "--ErrorLog--" << endl;
		Error << this->Errorstring << endl;
	}
}

vector<int> LuaUsage::getIntVector(const std::string& name) {
	vector<int> v;
	lua_gettostack(name.c_str());
	if (lua_isnil(LuaState, -1)) 
	{ 
		return vector<int>();
	}
	lua_pushnil(LuaState);
	while (lua_next(LuaState, -2)) 
	{
		v.push_back((int)lua_tonumber(LuaState, -1));
		lua_pop(LuaState, 1);
	}
	clean();
	return v;
}

vector<std::string> LuaUsage::getTableKeys(const std::string& name) {
	string code =
		"function getKeys(name) "
		"s = \"\""
		"for k, v in pairs(_G[name]) do "
		"    s = s..k..\",\" "
		"    end "
		"return s "
		"end"; // function for getting table keys
	luaL_loadstring(LuaState,code.c_str()); // execute code
	lua_pcall(LuaState, 0, 0, 0);
	lua_getglobal(LuaState, "getKeys"); // get function
	lua_pushstring(LuaState, name.c_str());
	lua_pcall(LuaState, 1, 1, 0); // execute function
	string test = lua_tostring(LuaState, -1);
	vector<std::string> strings;
	string temp = "";
	cout << "TEMP:" << test << std::endl;
	for (unsigned int i = 0; i < test.size(); i++) 
	{
		if (test.at(i) != ',') 
		{
			temp += test.at(i);
		}
		else 
		{
			strings.push_back(temp);
			temp = "";
		}
	}
	clean();
	return strings;
}
