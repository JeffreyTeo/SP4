#include "GridSystem.h"
#include <iostream>


GridSystem::GridSystem()
	:PlayerMoved(false)
{
}


GridSystem::~GridSystem()
{
	if (GridsVec.size() > 0)
	{
		for (int a = 0; a < GridsVec.size(); a++)
		{
			delete GridsVec[a];
		}
		GridsVec.clear();
	}
}

void GridSystem::Init(Vector3 Pos, float LengthX, float LengthY, int NumOfGridsX, int NumOfGridsY)
{
	this->Pos = Pos;
	this->LengthOfGridsX = LengthX;
	this->LengthOfGridsY = LengthY;
	this->NumOfGridsX = NumOfGridsX;
	this->NumOfGridsY = NumOfGridsY;
	
	
	int checkOddGridsX = NumOfGridsX % 2;
	int checkOddGridsY = NumOfGridsY % 2;

	float x = 0.f;
	float y = 0.f;

	float offsetX = 0.f;
	float offsetY = 0.f;

	for (int a = NumOfGridsY; a > 0; a--)
	{
		for (int b = 0; b < NumOfGridsX; b++)
		{
			//calculation of x pos for grids
			if (checkOddGridsX)
			{
				int mid = (NumOfGridsX + 1) * 0.5;
				offsetX = LengthX * (mid - (b + 1));
			}
			else
			{
				int mid = NumOfGridsX * 0.5;
				offsetX = LengthX * (mid - (b + 1)) + LengthX * 0.5;
			}
			x = this->Pos.x - offsetX;

			//calculation of y pos for grids
			if (checkOddGridsY)
			{
				int mid = (NumOfGridsY + 1) * 0.5;
				offsetY = LengthY * (mid - a);
			}
			else
			{
				int mid = NumOfGridsY * 0.5;
				offsetY = LengthY * (mid - a) + LengthY * 0.5;
			}
			y = this->Pos.y - offsetY;

			//set x and y as grid pos and push into vector
			Grid* PlayGrid = new Grid();
			PlayGrid->SetPos(x, y);
			GridsVec.push_back(PlayGrid);
			//std:: cout << x << ", " << y << std::endl;
		}
	}
	
}

void GridSystem::UpdateGrid(Vector3 CursorPos)
{
	for (int a = 0; a < GridsVec.size(); a++)
	{
		Vector3 GridPos = GridsVec[a]->GetPos();
		// detection is due to reference point being at the bottom left
		if (GridPos.x < CursorPos.x && CursorPos.x < GridPos.x + LengthOfGridsX)
		{
			if (GridPos.y < CursorPos.y && CursorPos.y < GridPos.y + LengthOfGridsY)
			{
				GridsVec[a]->ChangeType();
			//	std::cout << GridsVec[a]->GetPos() << std::endl;
			}
		}
	}
}

void GridSystem::SetMap(vector<vector<int>> maplayout)
{
	for (int a = 0; a < this->NumOfGridsY; a++)
	{
		for (int b = 0; b < this->NumOfGridsX; b++)
		{
			//if ((a * 5 + b) > maplayout.size() * maplayout[0].size())
			{
				std::cout << maplayout[NumOfGridsY - a][b];
				//maplayout has to have +1 in the y axis the first line in the excel file is not part of the actual map
				this->GridsVec[a * NumOfGridsX + b]->SetType(maplayout[a + 1][b]);
				this->GridsVec[a * NumOfGridsX + b]->SetStatus(maplayout[a + 1][b]);
			}
		}
		std::cout << std::endl;
	}
}

Vector3 GridSystem::PlayerGridSetUp(int xGrid, int yGrid)
{
	this->PlayerGrid = this->GridsVec[yGrid * NumOfGridsX + xGrid];
	//std::cout << PlayerGrid->GetPos() << std::endl;
	this->PlayerGridPos = Vector3(xGrid, yGrid, 0);
	return this->GridsVec[yGrid * NumOfGridsX + xGrid]->GetPos();
}

void GridSystem::PlayerGridUpdate(char key)
{
	Vector3 PlayerPos = this->PlayerGrid->GetPos();
	
	if (key == 'w')
	{
		PlayerPos += Vector3(0, LengthOfGridsY, 0);
		
	}
	else if (key == 's')
	{
		PlayerPos -= Vector3(0, LengthOfGridsY, 0);
	}
	else if (key == 'a')
	{
		PlayerPos -= Vector3(LengthOfGridsX, 0, 0);
	}
	else if (key == 'd')
	{
		PlayerPos += Vector3(LengthOfGridsX, 0, 0);
	}
	
	Vector3 TopLeft = this->GridsVec[0]->GetPos();
	
	Vector3 BottomRight = this->GridsVec[this->GridsVec.size() - 1]->GetPos();
	
	
	if (TopLeft.x <= PlayerPos.x && PlayerPos.x <= BottomRight.x
		&& BottomRight.y <= PlayerPos.y && PlayerPos.y <= TopLeft.y)
	{
		
		for (int a = 0; a < this->GridsVec.size(); a++)
		{
			Vector3 GridPos = GridsVec[a]->GetPos();
			
			if (GridsVec[a]->GetType() == Grid::GridType::FLOOR && GridsVec[a]->GetStatus() != 1)
			{
				if (PlayerPos == GridPos)
				{
					this->PlayerGrid = GridsVec[a];
					this->PlayerGridPos = Vector3(a % NumOfGridsX, a / NumOfGridsX, 0);
					GridsVec[a]->SetStatus(1);
					GridsVec[a]->SetDirection(key);
					PlayerMoved = true;
				}
			}
		}
	}

}

void GridSystem::AIGridSetUp(vector<cAI*> AIList)
{
	for (int a = 0; a < AIList.size(); a++)
	{
		this->AIVec.push_back(AIList[a]);
		this->AIGridsVec.push_back(AIList[a]->getPos().y * NumOfGridsX + AIList[a]->getPos().x);
	}
}

void GridSystem::AIGridUpdate()
{
	if (this->AIGridsVec.size() == 0)
	{
		return;
	}
	else
	{
		for (int a = 0; a < this->AIGridsVec.size(); a++)
		{
			//if (AIVec[a]->getState() == cAI::FSM_PLAYER::STATE_MOVE)
			{
				
				AIVec[a]->update(this->PlayerGridPos, PlayerMoved);
				//AIVec[a]->UpdateWaypoint();
				AIGridsVec[a] = AIVec[a]->getPos().y * NumOfGridsX + AIVec[a]->getPos().x;

			}
		}
	}
}

void GridSystem::SetAnswer()
{

}

vector<Grid*> GridSystem::GetGridsVec()
{
	return this->GridsVec;
}

Grid* GridSystem::GetPlayerGrid()
{
	return this->PlayerGrid;
}

vector<Grid*> GridSystem::GetAIGrids()
{
	vector<Grid*> aigrid;
	for (int a = 0; a < this->AIGridsVec.size(); a++)
	{
		aigrid.push_back(this->GridsVec[AIGridsVec[a]]);
	}
	return aigrid;
}