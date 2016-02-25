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

	enum GridStatus
	{
		//can walk on
		PASSABLE = 0,
		//cannot walk on(anymore)
		UNPASSABLE,
		TOTAL_STATUS,
	};

	Grid();
	~Grid();

	void SetType(int);
	int GetType(void);
	void ChangeType(void);

	void SetStatus(int);
	int GetStatus(void);

	void SetDirection(char);
	float GetDirection(void);

	void SetPos(float, float);
	Vector3 GetPos();

private:
	GridType Type;
	GridStatus Status;
	Vector3 Pos;
	float direction;
};

