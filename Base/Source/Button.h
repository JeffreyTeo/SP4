#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include "Vector3.h"
#include "LuaUsage.h"
#include <string.h>

class Button
{
public:
	Button();
	~Button();
	void ButtonInit(std::string Name);
	void update(double dt);
	Vector3 getTempPosition();
	Vector3 getPosition();
	Vector3 getOffset();
private:
	LuaUsage* Loading;
	Vector3 Direction;
	Vector3 TempPosition;
	Vector3 Position;
	Vector3 Offset;
	float speed;
};

#endif