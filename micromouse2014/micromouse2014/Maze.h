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
	Room maze[16][16];
	Maze(){ rooms = 0; initMaze(); makeMaze(); }

	// Bracket overload
	// To use:
	// Room r = m[std::pair<int, int>(1,1)];
	Room operator [] (std::pair<int,int> param) { return maze[param.first][param.second]; }

	// Parentheses overload
	// To use:
	// Room r = m(1,1);
	Room operator () (int x, int y) { return maze[x][y]; }
	
	void printMaze();
private:
	std::deque<Location> opening_locations;
	int rooms;

	void initMaze();
	void makeMaze();
	void clearMaze();
	void cleanMaze();
	int getChoice(int x, int y);
	int getAdjacentRooms(int x, int y);
	int find(Location loc);
};