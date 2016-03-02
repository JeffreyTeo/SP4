#include "GridSystem.h"
#include <iostream>


GridSystem::GridSystem()
	:PlayerMoved(false)
	, PrevMovedBlockIdx(0)
	, movingActive(false)
	, MovingBlock(NULL)
	, timer(5.f)
	, PlayerJumped(false)
	, PlayerJumpedTimer(5.f)
	, PlayerBuffer(0.f)
	, PlayerPushPull(false)
	, PushPull(false)
	, PushPullTimer(0.f)
	, CanPush(false)
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


	/*if (FallingBlocks.size() > 0)
	{
		for (int a = 0; a < FallingBlocks.size(); a++)
		{
			delete FallingBlocks[a];
		}
		FallingBlocks.clear();
	}*/
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
				if (maplayout[a + 1][b] == 0 || 2 || 5 || 6 || 7 || 8 || 9 || 10)
					this->GridsVec[a * NumOfGridsX + b]->SetStatus(0);
				else
					this->GridsVec[a * NumOfGridsX + b]->SetStatus(1);
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

bool GridSystem::PlayerGridUpdate(char key)
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
			
			if ((GridsVec[a]->GetType() == Grid::GridType::FLOOR || GridsVec[a]->GetType() == Grid::GridType::KEY || GridsVec[a]->GetType() == Grid::GridType::MOVESIGN || GridsVec[a]->GetType() == Grid::GridType::EXIT || GridsVec[a]->GetType() == Grid::GridType::EXITSIGN || GridsVec[a]->GetType() == Grid::GridType::INTROSIGN || GridsVec[a]->GetType() == Grid::GridType::KEYSIGN || GridsVec[a]->GetType() == Grid::GridType::MONSTERSIGN) && GridsVec[a]->GetStatus() != 1)
			{
				if (PlayerPos == GridPos)
				{
					this->PlayerGrid = GridsVec[a];
					this->PlayerGridPos = Vector3(a % NumOfGridsX, a / NumOfGridsX, 0);
					GridsVec[a]->SetStatus(1);
					GridsVec[a]->SetDirection(key);
					PlayerMoved = true;
					if (GridsVec[a]->GetType() == Grid::GridType::KEY)
					{
						GridsVec[a]->keyCollected = true;
					}
					if (GridsVec[a]->GetType() == Grid::GridType::EXIT)
					{
						GridsVec[a]->Win = true;
					}
					if (GridsVec[a]->GetType() == Grid::GridType::INTROSIGN)
					{
						GridsVec[a]->Sign1Touched = true;
					}
					if (GridsVec[a]->GetType() == Grid::GridType::MOVESIGN)
					{
						GridsVec[a]->Sign2Touched = true;
					}
					if (GridsVec[a]->GetType() == Grid::GridType::KEYSIGN)
					{
						GridsVec[a]->Sign3Touched = true;
					}
					if (GridsVec[a]->GetType() == Grid::GridType::MONSTERSIGN)
					{
						GridsVec[a]->Sign4Touched = true;
					}
					if (GridsVec[a]->GetType() == Grid::GridType::EXITSIGN)
					{
						GridsVec[a]->Sign5Touched = true;
					}
					return true;
				}
			}
		}
	}
	return false;
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


void GridSystem::GridDropInit()
{

	for (int a = 0; a < this->GridsVec.size(); a++)
	{
		int xPos = (a + 1) % this->NumOfGridsX;
		//check if current index is not the leftmost, rightmost or bottom rows so that blocks can spawn
		if (xPos != 0 && xPos != 1 && (a / this->NumOfGridsX) != this->NumOfGridsY - 1)
		{
			//variable to make sure only 1 block spawn at a time
			if (!movingActive && MovingBlock == NULL)
			{
				PrevMovedBlockIdx++;
				movingActive = true;
				MovingBlock = GridsVec[PrevMovedBlockIdx];
				MovingBlock->SetType(1);
				//add into falling blocks vector
				this->FallingBlocks.push_back(MovingBlock);
			}
		}
		else
		{
			/*if (xPos == PrevMovedBlockIdx)
			{
			if (GridsVec[a] == MovingBlock || (a / this->NumOfGridsX) == this->NumOfGridsY - 1)
			{
			GridsVec[a]->SetType(1);
			}
			}
			else*/
			//the leftmost, rightmost and bottom rows are all automatically blocks
			GridsVec[a]->SetType(1);
		}
	}


}

void GridSystem::GridDropUpdate()
{
	if (movingActive && MovingBlock != NULL)
	{
		//timer buffer so that blocks would not fall too fast
		if (timer <= 0)
		{
			for (int a = 0; a < this->GridsVec.size(); a++)
			{
				bool skip = false;
				for (int b = 0; b < FallingBlocks.size(); b++)
				{
					//check if falling blocks have space below to fall to
					if (FallingBlocks[b] == GridsVec[a])
					{
						if (a + NumOfGridsX <= GridsVec.size() && GridsVec[a + NumOfGridsX]->GetType() != 1)
						{
							FallingBlocks[b]->SetType(0);
							FallingBlocks[b] = GridsVec[a + NumOfGridsX];
							FallingBlocks[b]->SetType(1);
							skip = true;
							timer = 5.f;
							break;
						}
						else
						{
							//only when the latest block can no longer move(no space below/stuck to another block) then add new block
							if (FallingBlocks[FallingBlocks.size() - 1] == FallingBlocks[b])
							{
								if (PrevMovedBlockIdx >= NumOfGridsX - 2)
									PrevMovedBlockIdx = 0;
								movingActive = false;
								MovingBlock = NULL;
								GridDropInit();
							}

						}
					}

				}
				if (skip)
				{
					break;
				}

			}
		}
		else
			timer -= 0.5f;
	}


	//std::cout << timer << std::endl;
}

bool GridSystem::PlayerGridDropUpdate(char key)
{
	Vector3 PlayerPos = this->PlayerGrid->GetPos();

	//buffer to prevent player from moving too fast
	if (PlayerBuffer <= 0.f)
	{
		if (key == 'w')
		{
			//no double jump
			if (PlayerJumped == false)
			{
				PlayerPos += Vector3(0, LengthOfGridsY, 0);
				PlayerJumped = true;
				PlayerBuffer = 5.f;

			}

		}
		else if (key == 'a')
		{
			PlayerPos -= Vector3(LengthOfGridsX, 0, 0);
			PlayerBuffer = 5.f;
			this->PlayerGrid->SetDirection(key);
			CanPush = true;
		}
		else if (key == 'd')
		{
			PlayerPos += Vector3(LengthOfGridsX, 0, 0);
			PlayerBuffer = 5.f;
			this->PlayerGrid->SetDirection(key);
			CanPush = true;
		}

	}
	else
		PlayerBuffer -= 1.f;
	//jumping buffer for dropping
	if (PlayerJumped)
	{
		PlayerJumpedTimer -= 0.5f;
		if (PlayerJumpedTimer <= 0)
		{
			PlayerPos -= Vector3(0, LengthOfGridsY, 0);
			PlayerJumpedTimer = 5.f;
			PlayerJumped = false;
		}
	}

	Vector3 TopLeft = this->GridsVec[0]->GetPos();

	Vector3 BottomRight = this->GridsVec[this->GridsVec.size() - 1]->GetPos();



	if (TopLeft.x <= PlayerPos.x && PlayerPos.x <= BottomRight.x
		&& BottomRight.y <= PlayerPos.y && PlayerPos.y <= TopLeft.y)
	{

		for (int a = 0; a < this->GridsVec.size(); a++)
		{
			Vector3 GridPos = GridsVec[a]->GetPos();
			//check if next grid to be landed on is a floor
			if ((GridsVec[a]->GetType() == Grid::GridType::FLOOR) && GridsVec[a]->GetStatus() != 1)
			{
				if (PlayerPos == GridPos)
				{
					//when landing after jumping
					if (GridsVec[a + NumOfGridsX]->GetType() == 0 && !PlayerJumped)
					{
						//get direction before changing grid
						float newDirection = this->PlayerGrid->GetDirection();
						this->PlayerGrid = GridsVec[a + NumOfGridsX];
						//set direction when in previous grid to current grid after changing grids
						this->PlayerGrid->SetDirection(newDirection);
						this->PlayerGridPos = Vector3((a + NumOfGridsX) % NumOfGridsX, a / NumOfGridsX, 0);
						CanPush = false;
					}
					else
					{

						//if jumped,set previous direction to new grid landed on
						if (key == 'w')
						{
							//get direction before changing grid
							float newDirection = this->PlayerGrid->GetDirection();
							this->PlayerGrid = GridsVec[a];
							//set direction when in previous grid to current grid after changing grids
							this->PlayerGrid->SetDirection(newDirection);
							CanPush = false;
						}
						else
							this->PlayerGrid = GridsVec[a];
						this->PlayerGridPos = Vector3(a % NumOfGridsX, a / NumOfGridsX, 0);
					}


					PlayerMoved = true;
					return true;
				}
			}
		}
	}

	return false;
}

void GridSystem::PlayerGridDropPushPull(char key, bool grab)
{

	bool CheckifPlayerInAir = this->GridsVec[(PlayerGridPos.y + 1) * NumOfGridsX + PlayerGridPos.x]->GetType();
	//player can only push if not in the air
	if (PlayerPushPull == true && CheckifPlayerInAir && CanPush)
	{

		int PlayerGridIdx = this->PlayerGridPos.y * NumOfGridsX + this->PlayerGridPos.x;
		//get offset based on direction of player
		int offset = PlayerGrid->GetDirection() / 90;
		int TargetGridPos = PlayerGridIdx - offset;
		Grid* TargetGrid = GridsVec[TargetGridPos];

		int tempTypeStorage = TargetGrid->GetType();
		for (int a = 0; a < FallingBlocks.size(); a++)
		{
			//check if the targeted gris is in falling block vector as it needs to be updated so tat it can fall if pushed
			if (TargetGrid == FallingBlocks[a])
			{
				//if in push state
				if (grab)
				{
					//can only push blocks
					if (tempTypeStorage)
					{
						//direction = 90
						if (key == 'a')
						{
							/*
							Feature: block fixing in the air
							only the pushed grid is updated
							the switched grid if is in falling block vector will not be updated
							thus creating the grid not falling is in the air
							*/
							TargetGrid->SetType(GridsVec[TargetGridPos - 1]->GetType());
							GridsVec[TargetGridPos - 1]->SetType(tempTypeStorage);
							FallingBlocks[a] = GridsVec[TargetGridPos - 1];
						}
						//direction = -90
						else if (key == 'd')
						{
							/*
							Feature: block fixing in the air
							only the pushed grid is updated
							the switched grid if is in falling block vector will not be updated
							thus creating the grid not falling is in the air
							*/
							TargetGrid->SetType(GridsVec[TargetGridPos + 1]->GetType());
							GridsVec[TargetGridPos + 1]->SetType(tempTypeStorage);
							FallingBlocks[a] = GridsVec[TargetGridPos + 1];
						}
					}
				}
			}
		}
	}



}

void GridSystem::PlayerGridDropStateChange(char key)
{
	if (PushPullTimer <= 0.f)
	{
		if (key == 'p')
		{
			// false = neutral, true = push
			PlayerPushPull = !PlayerPushPull;
			PushPullTimer = 5.f;

		}


	}
	else
	{
		PushPullTimer -= 0.05f;
	}

	PlayerGridDropPushPull(key, PlayerPushPull);
}

bool GridSystem::CheckCollisionType(Grid::GridType CheckType, int& NumOfCollides)
{
	bool collided = false;
	/*for (int a = 0; a < GridsVec.size(); a++)
	{
		if (GridsVec[a]->GetType() == CheckType)
		{
			collided = true;
			NumOfCollides++;
		}
	}*/
	if (PlayerGrid->GetType() == CheckType)
	{
		collided = true;
		NumOfCollides++;
	}
	return collided;
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