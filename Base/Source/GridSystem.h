#pragma once
#include "Grid.h"
#include <vector>


using std::vector;

class GridSystem
{
public:
	GridSystem();
	~GridSystem();

	//initialise various needed variables
	void Init(Vector3 Pos, float LengthX, float LengthY, int NumOfGridsX, int NumOfGridsY);
	//update the grid system 
	void UpdateGrid(Vector3);
	//set map layout
	void SetMap(vector<vector<int>>);
	//set player grid and Position
	Vector3 PlayerGridSetUp(int, int);
	//Player Update
	void PlayerGridUpdate(char key);
	// set correct answer
	void SetAnswer();

	// get grid vector to get each grids pos
	vector<Grid*> GetGridsVec();
	Grid* GetPlayerGrid();
private:
	vector<Grid*> GridsVec;
	Grid* PlayerGrid;

	Vector3 Pos;
	
	int NumOfGridsX;
	int NumOfGridsY;
	float LengthOfGridsX;
	float LengthOfGridsY;
	Vector3 PosOfGrid;
	
};

