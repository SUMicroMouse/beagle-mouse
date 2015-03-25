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
#include <map>

#include "Direction.h"
#include "Room.h"

namespace Data
{
	//enum Direction
	//{
	//	LEFT,  // left
	//	DOWN,     // Down/bottom
	//	RIGHT,  // right
	//	UP    // Up/top
	//};

	class Maze
	{
	public:
		Maze(){ rooms = 0; dimensions = 16; initMaze(); makeMaze(); }
		Maze(bool instantiate) { rooms = 0; dimensions = 16; if (instantiate) { initMaze(); makeMaze(); } }
		Maze(int dim) { rooms = 0; dimensions = dim; initMaze(); makeMaze(); }
		void printMaze(bool walls_hidden);

		Direction getDirection() { return direction; }
		void setDirection(Direction dir)
		{
			direction = dir;
			// Below is for safety, however this should be handled in turn() functions
			if (direction < 0) // if trying to turn right and facing left (0)..
				direction = (Direction)3;
			else if (direction > 3) // if trying to turn left and facing up (3)..
				direction = (Direction)0;
		}
		Location* Start() { return start; }
		void Current(Location* loc) { current = loc; }
		std::array<Room*, 4> Goal() { return goal; }

	private:
		std::array<std::array<Room*, 16>, 16> maze;
		// for (auto& i; maze..) to loop through
		std::deque<Location*> opening_locations;
		Location *start, *current; // Starting/Current location
		std::array<Room*, 4> goal; // goal
		int rooms, dimensions;
		Direction direction;

		void initMaze();
		void makeMaze();
		void clearMaze();
	public:
		// Clear the boolean checked values for all of the rooms. reset to an empty map
		void clearChecked();
		// Reset the breadth heuristics to the max int
		void resetBreadthHeuristics();
	private:
		void cleanMaze();
		void clearPillars();
		void clearEmptyRooms();
		void setWallParents();
		int getChoice(int x, int y);
		int getNumberOfAdjacentRooms(int x, int y);
		std::vector<Room*> getAdjacentRooms(int x, int y);
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

		/*
		void operator = (Maze m);
		*/
	};

}

#endif