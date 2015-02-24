#ifndef __MAZE_H__
#define __MAZE_H__

#include <deque>
#include <vector>
#include <time.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <array>

#include "Room.h"

class Maze
{
public:
	Maze(){ rooms = 0; dimensions = 16; initMaze(); makeMaze(); }
	Maze(int dim) { rooms = 0; dimensions = dim; initMaze(); }
	void printMaze(bool walls_hidden);

private:
	std::array<std::array<Room*,16>,16> maze;
	// for (auto& i; maze..) to loop through
	std::deque<Location*> opening_locations;
	Location *start, *current; // Starting/Current location
	std::array<Room*, 4> goal; // goal
	int rooms, dimensions;

	void initMaze();
	void makeMaze();
	void clearMaze();
	void clearChecked();
	void cleanMaze();
	void setWallParents();
	int getChoice(int x, int y);
	int getAdjacentRooms(int x, int y);
	int find(Location * loc);

public:
	// Bracket overload
	// To use:
	// Room r = m[std::pair<int, int>(1,1)];
	Room* operator [] (std::pair<int, int> param) { return maze[param.first][param.second]; }

	// Parentheses overload
	// To use:
	// Room r = m(1,1);
	Room* operator () (int x, int y) { return maze[x][y]; }
};

#endif