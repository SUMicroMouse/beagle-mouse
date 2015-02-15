#include <deque>
#include <vector>
#include <time.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include "Room.h"
#include "Location.h"
#include <string.h>

class Maze
{
public:
	vector<vector<Room>> maze;
	Maze(){ mazeType = ""; rooms = 0; dimensions = 10; initMaze(); makeMaze(); }
	Maze(int dim){ mazeType = ""; rooms = 0; dimensions = dim; initMaze(); makeMaze(); }
	Maze(string maze){ mazeType = "MicroMouse"; rooms = 0; dimensions = 16; initMaze(); makeMaze(); }
	
	void printMaze();

	string getMazeType() { return mazeType; }
	int getDimensions() { return dimensions; }
	Location* getEnd(){ return end; }
private:
	deque<Location> opening_locations;
	Location* end;
	int dimensions;
	int rooms;
	string mazeType;

	void initMaze();
	void makeMaze();
	void clearMaze();
	int getChoice(int x, int y);
	int getAdjacentRooms(int x, int y);
	int find(Location loc);
};