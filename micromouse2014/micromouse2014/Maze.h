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
	Maze(){ rooms = 0; initMaze(); makeMaze(); }
		
	void printMaze();
private:
	std::array<std::array<Room*,16>,16> maze;
	// for (auto& i; maze..) to loop through
	std::deque<Location*> opening_locations;
	int rooms;

	void initMaze();
	void makeMaze();
	void clearMaze();
	void clearChecked();
	void cleanMaze();
	int getChoice(int x, int y);
	int getAdjacentRooms(int x, int y);
	int find(Location * loc);

public:
	// Bracket overload
	// To use:
	// Room r = m[std::pair<int, int>(1,1)];
	Room * operator [] (std::pair<int, int> param) { return maze[param.first][param.second]; }

	// Parentheses overload
	// To use:
	// Room r = m(1,1);
	Room * operator () (int x, int y) { return maze[y][x]; }
};

#endif