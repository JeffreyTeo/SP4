#include "Grid.h"


Grid::Grid()
:Type(FLOOR)
, Status(PASSABLE)
, keyCollected(true)
, Win(true)
{
}


Grid::~Grid()
{
	
}

void Grid::SetType(int newType)
{
	this->Type = (GridType)(newType);
	if (Type == KEY)
		keyCollected = false;
	if (Type == EXIT)
		Win = false;
}

int Grid::GetType(void)
{
	return this->Type;
}

void Grid::ChangeType(void)
{
	this->Type = (GridType)(this->Type + 1);
	if (this->Type >= TOTAL_TYPE)
	{
		this->Type = FLOOR;
	}
}

void Grid::SetStatus(int newStatus)
{
	this->Status = (GridStatus)newStatus;
}

int Grid::GetStatus(void)
{
	return this->Status;
}

void Grid::SetDirection(char newDirection)
{
	if ((this->Type == FLOOR || this->Type == KEY) && this->Status == UNPASSABLE)
	switch (newDirection)
	{
		case 'w':
		{
					this->direction = 0.f;
		}
			break;
		case 's':
		{
					this->direction = 180.f;
		}
			break;
		case 'a':
		{
					this->direction = 90.f;
		}
			break;
		case 'd':
		{
					this->direction = -90.f;
		}
			break;
	}
		
	
}

float Grid::GetDirection(void)
{
	return this->direction;
}

void Grid::SetPos(float x, float y)
{
	this->Pos = Vector3(x, y, 0);
}

Vector3 Grid::GetPos()
{
	return this->Pos;
}