#include "Grid.h"


Grid::Grid()
:Type(FLOOR)
{
}


Grid::~Grid()
{
	
}

void Grid::SetType(int newType)
{
	this->Type = (GridType)(newType);
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

void Grid::SetPos(float x, float y)
{
	this->Pos = Vector3(x, y, 0);
}

Vector3 Grid::GetPos()
{
	return this->Pos;
}