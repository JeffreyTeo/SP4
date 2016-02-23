#ifndef LUAUSAGE_H
#define LUAUSAGE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "lua.hpp"

using namespace std;

class LuaUsage
{
public:
	LuaUsage(void);
	~LuaUsage(void);

	void LuaUsageInit(string LuaFileName);
	void LuaUsageClose();
	void clean();

	vector<int> getIntVector(const std::string& name);
	vector<string> getTableKeys(const std::string& name);

	template<typename T>
	T get(const std::string& variableName) 
	{
		if (!LuaState) 
		{
			Seterrorstring(variableName, "Script is not loaded");
			return lua_getdefault<T>();
		}

		T result;
		if (lua_gettostack(variableName)) { // variable succesfully on top of stack
			result = lua_get<T>(variableName);
		}
		else {
			result = lua_getdefault<T>();
		}
		clean();
		return result;
	}

	bool lua_gettostack(const std::string& variableName) 
	{
		level = 0;
		std::string var = "";
		for (unsigned int i = 0; i < variableName.size(); i++) 
		{
			if (variableName.at(i) == '.') 
			{
				if (level == 0) {
					lua_getglobal(LuaState, var.c_str());
				}
				else 
				{
					lua_getfield(LuaState, -1, var.c_str());
				}

				if (lua_isnil(LuaState, -1)) 
				{
					Seterrorstring(variableName, var + " is not defined");
					return false;
				}
				else 
				{
					var = "";
					level++;
				}
			}
			else 
			{
				var += variableName.at(i);
			}
		}
		if (level == 0) 
		{
			lua_getglobal(LuaState, var.c_str());
		}
		else 
		{
			lua_getfield(LuaState, -1, var.c_str());
		}
		if (lua_isnil(LuaState, -1)) 
		{
			Seterrorstring(variableName, var + " is not defined");
			return false;
		}

		return true;
	}

	// Generic get
	template<typename T>
	T lua_get(const std::string& variableName) 
	{
		return 0;
	}

	template<typename T>
	T lua_getdefault() 
	{
		return 0;
	}


	

	void Seterrorstring(const string& Variablename, const string& reason);
	void LuaErrorLog();
private:
	lua_State *LuaState;
	string Errorstring;
	string Filename;
	int level;
};
template <>
inline bool LuaUsage::lua_get<bool>(const std::string& variableName)
{
	return (bool)lua_toboolean(LuaState, -1);
}

template <>
inline float LuaUsage::lua_get<float>(const std::string& variableName)
{
	if (!lua_isnumber(LuaState, -1)) {
		Seterrorstring(variableName, "Not a number");
	}
	return (float)lua_tonumber(LuaState, -1);
}

template <>
inline int LuaUsage::lua_get<int>(const std::string& variableName)
{
	if (!lua_isnumber(LuaState, -1)) {
		Seterrorstring(variableName, "Not a number");
	}
	return (int)lua_tonumber(LuaState, -1);
}

template <>
inline string LuaUsage::lua_get<std::string>(const std::string& variableName) {
	string s = "null";
	if (lua_isstring(LuaState, -1)) {
		s = string(lua_tostring(LuaState, -1));
	}
	else
	{
		Seterrorstring(variableName, "Not a string");
	}
	return s;
}

template<>
inline string LuaUsage::lua_getdefault<std::string>() {
	return "null";
}

#endif