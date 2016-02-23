#pragma once
#include "Vector3.h"

class Grid
{
public:
	enum GridType
	{
		FLOOR = 0,
		WALL,
		TOTAL_TYPE,
	};

	Grid();
	~Grid();

	void SetType(int);
	int GetType(void);
	void ChangeType(void);

	void SetPos(float, float);
	Vector3 GetPos();

private:
	GridType Type;
	Vector3 Pos;
};

