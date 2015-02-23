#pragma once
#ifndef __ROOM_H__
#define __ROOM_H__

#undef min
#undef max

#include <vector>
//#include "Maze.h"
#include "Wall.h"
#include "Location.h"


// Designates where the walls are
enum RoomType
{
	NONE, LEFT, BOTTOM, RIGHT, TOP,
	LEFT_BOTTOM, BOTTOM_RIGHT, LEFT_RIGHT, LEFT_TOP, BOTTOM_TOP, RIGHT_TOP,
	LEFT_BOTTOM_RIGHT, LEFT_BOTTOM_TOP, LEFT_RIGHT_TOP, BOTTOM_RIGHT_TOP,ALL
};

class Room
{
private:
	std::vector<Wall> openings; // Vector of Wall objects
	int opens; // Number of openings
	Location loc;
	RoomType type;

	// neighboring rooms
	std::vector<Room*> neighbors;

	int breadth_heuristic;
	void reset_breadth_heuristic();
	//void set_adjacent_rooms(Maze &maze);

public:
	//Room() :openings(4), opens(0), loc(0,0)
	Room();

	//Room(int v, int x, int y) :openings(4), opens(0), loc(x, y);
	Room(int v, int x, int y);

	std::vector<bool> getOpenings();

	// Heuristics
	float weight();	// lower = better?

	int get_breadth_heuristic();
	void set_breadth_heuristic(int new_value);

	std::vector<Room*> * get_children();
	bool checked;						// boolean used in depth first search to prevent looping
	int getPassages(){ return opens; }
	char* getRoom();

	void setWall(int side, bool value);

	void operator =(Room & room2);
	bool operator <(Room & room2);
	bool operator >(Room & room2);
	bool operator ==(Room & room2);
};



#endif