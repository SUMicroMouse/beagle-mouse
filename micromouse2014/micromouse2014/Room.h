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
	Room() :openings(4), opens(0), loc(0,0)
	{ 
		breadth_heuristic = std::numeric_limits<int>::max(); 
		type = RoomType::NONE;
	}

	Room(int v, int x, int y) :openings(4), opens(0), loc(x,y)
	{
		// Variable v determine which kind of room will be made.
		// Why did I name it v? I have no idea..
		/*
		char topLeftCorner = 201;
		char topRightCorner = 187;
		char bottomLeftCorner = 200;
		char bottomRightCorner = 188;
		char hoLine = 205;
		char vertLine = 186;
		*/
		type = (RoomType)(v - 1);
		switch (v)
		{
		case 1: // none
			opens = 4; break;
		case 2: // left
			openings[0].setClosed(1); opens = 3; break;
		case 3: // bottom
			openings[1].setClosed(1); opens = 3; break;
		case 4: // right
			openings[2].setClosed(1); opens = 3; break;
		case 5: // top
			openings[3].setClosed(1); opens = 3; break;
		case 6: // left bottom
			openings[0].setClosed(1); openings[1].setClosed(1); opens = 2; break;
		case 7: // bottom right
			openings[1].setClosed(1); openings[2].setClosed(1); opens = 2; break;
		case 8: // left right
			openings[0].setClosed(1); openings[2].setClosed(1); opens = 2; break;
		case 9: // left top
			openings[0].setClosed(1); openings[3].setClosed(1); opens = 2; break;
		case 10: // bottom top
			openings[1].setClosed(1); openings[3].setClosed(1); opens = 2; break;
		case 11: // right top
			openings[2].setClosed(1); openings[3].setClosed(1); opens = 2; break;
		case 12: // left bottom right
			openings[0].setClosed(1); openings[1].setClosed(1); openings[2].setClosed(1); opens = 1; break;
		case 13: // left bottom top
			openings[0].setClosed(1); openings[1].setClosed(1); openings[3].setClosed(1); opens = 1; break;
		case 14: // left right top
			openings[0].setClosed(1); openings[2].setClosed(1); openings[3].setClosed(1); opens = 1; break;
		case 15: // bottom right top
			openings[1].setClosed(1); openings[2].setClosed(1); openings[3].setClosed(1); opens = 1; break;

		default:
			for (int i = 0; i < 4; i++) { openings[i].setClosed(1); } break;
		}
	}

	std::vector<bool> getOpenings();

	// Heuristics
	int get_breadth_heuristic();
	void set_breadth_heuristic(int new_value);

	std::vector<Room*> * get_children();
	bool checked;						// boolean used in depth first search to prevent looping
	int getPassages(){ return opens; }
	char* getRoom();

	void setWall(int side, bool value);

	void operator =(Room & room2);
};



#endif