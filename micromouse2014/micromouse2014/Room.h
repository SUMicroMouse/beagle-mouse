#include <vector>
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
	std::vector<Room&> neighbors;

	int breadth_heuristic;
	void reset_breadth_heuristic();
	void set_adjacent_rooms(Room(&maze)[16][16]);

public:
	Room() :openings(4), opens(0), type(NONE), loc(0,0)
	{ breadth_heuristic = std::numeric_limits<int>::max(); }

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

	std::vector<bool> getOpenings()
	{ return { openings[0].getClosed(), openings[1].getClosed(), openings[2].getClosed(), openings[3].getClosed() }; }

	// Heuristics
	int get_breadth_heuristic();
	void set_breadth_heuristic(int new_value);

	std::vector<Room&> & get_children();
	bool checked;						// boolean used in depth first search to prevent looping
	int getPassages(){ return opens; }
	char* getRoom()
	{
		char room[9];
		switch (type+1)
		{
		case 1: // none
			room[0] = 188; room[1] = 32; room[2] = 200; room[3] = 32; room[4] = 32; room[5] = 32; room[6] = 187; room[7] = 32; room[8] = 201;
			break;
		case 2: // left
			room[0] = 186; room[1] = 32; room[2] = 200; room[3] = 186; room[4] = 32; room[5] = 32; room[6] = 186; room[7] = 32; room[8] = 201;
			break;
		case 3: // bottom
			room[0] = 188; room[1] = 32; room[2] = 200; room[3] = 32; room[4] = 32; room[5] = 32; room[6] = 205; room[7] = 205; room[8] = 205;
			break;
		case 4: // right
			room[0] = 188; room[1] = 32; room[2] = 186; room[3] = 32; room[4] = 32; room[5] = 186; room[6] = 187; room[7] = 32; room[8] = 186;
			break;
		case 5: // top
			room[0] = 205; room[1] = 205; room[2] = 205; room[3] = 32; room[4] = 32; room[5] = 32; room[6] = 187; room[7] = 32; room[8] = 201;
			break;
		case 6: // left bottom
			room[0] = 186; room[1] = 32; room[2] = 200; room[3] = 186; room[4] = 32; room[5] = 32; room[6] = 200; room[7] = 205; room[8] = 205;
			break;
		case 7: // bottom right
			room[0] = 188; room[1] = 32; room[2] = 186; room[3] = 32; room[4] = 32; room[5] = 186; room[6] = 205; room[7] = 205; room[8] = 188;
			break;
		case 8: // left right
			room[0] = 186; room[1] = 32; room[2] = 186; room[3] = 186; room[4] = 32; room[5] = 186; room[6] = 186; room[7] = 32; room[8] = 186;
			break;
		case 9: // left top
			room[0] = 201; room[1] = 205; room[2] = 205; room[3] = 186; room[4] = 32; room[5] = 32; room[6] = 186; room[7] = 32; room[8] = 201;
			break;
		case 10: // bottom top
			room[0] = 205; room[1] = 205; room[2] = 205; room[3] = 32; room[4] = 32; room[5] = 32; room[6] = 205; room[7] = 205; room[8] = 205;
			break;
		case 11: // right top
			room[0] = 205; room[1] = 205; room[2] = 187; room[3] = 32; room[4] = 32; room[5] = 186; room[6] = 187; room[7] = 32; room[8] = 186;
			break;
		case 12: // left bottom right
			room[0] = 186; room[1] = 32; room[2] = 186; room[3] = 186; room[4] = 32; room[5] = 186; room[6] = 200; room[7] = 205; room[8] = 188;
			break;
		case 13: // left bottom top
			room[0] = 201; room[1] = 205; room[2] = 205; room[3] = 186; room[4] = 32; room[5] = 32; room[6] = 200; room[7] = 205; room[8] = 205;
			break;
		case 14: // left right top
			room[0] = 201; room[1] = 205; room[2] = 187; room[3] = 186; room[4] = 32; room[5] = 186; room[6] = 186; room[7] = 32; room[8] = 186;
			break;
		case 15: // bottom right top
			room[0] = 205; room[1] = 205; room[2] = 187; room[3] = 32; room[4] = 32; room[5] = 186; room[6] = 205; room[7] = 205; room[8] = 188;
			break;
		default:
			for (int i = 0; i < 9; i++) { room[i] = 176; } break;
		}
		return room;
	}
	void setWall(int side, bool value)
	{
		openings[side].setClosed(value);
		if (value)
		{
			opens--;
			if (side == 0) // add wall to left
			{
				if (type == NONE)
					type = LEFT;
				else if (type == BOTTOM)
					type = LEFT_BOTTOM;
				else if (type == RIGHT)
					type = LEFT_RIGHT;
				else if (type == TOP)
					type = LEFT_TOP;
				else if (type == BOTTOM_RIGHT)
					type = LEFT_BOTTOM_RIGHT;
				else if (type == BOTTOM_TOP)
					type = LEFT_BOTTOM_TOP;
				else if (type == RIGHT_TOP)
					type = LEFT_RIGHT_TOP;
				else if (type == BOTTOM_RIGHT_TOP)
					type = ALL;
			}
			else if (side == 1) // add wall to bottom
			{
				if (type == NONE)
					type = BOTTOM;
				else if (type == LEFT)
					type = LEFT_BOTTOM;
				else if (type == RIGHT)
					type = BOTTOM_RIGHT;
				else if (type == TOP)
					type = BOTTOM_TOP;
				else if (type == LEFT_RIGHT)
					type = LEFT_BOTTOM_RIGHT;
				else if (type == LEFT_TOP)
					type = LEFT_BOTTOM_TOP;
				else if (type == RIGHT_TOP)
					type = BOTTOM_RIGHT_TOP;
				else if (type == LEFT_RIGHT_TOP)
					type = ALL;
			}
			else if (side == 2) // add wall to right
			{
				if (type == NONE)
					type = RIGHT;
				else if (type == LEFT)
					type = LEFT_RIGHT;
				else if (type == BOTTOM)
					type = BOTTOM_RIGHT;
				else if (type == TOP)
					type = RIGHT_TOP;
				else if (type == LEFT_BOTTOM)
					type = LEFT_BOTTOM_RIGHT;
				else if (type == BOTTOM_TOP)
					type = BOTTOM_RIGHT_TOP;
				else if (type == LEFT_TOP)
					type = LEFT_RIGHT_TOP;
				else if (type == LEFT_BOTTOM_TOP)
					type = ALL;
			}
			else if (side == 3) // add wall to top
			{
				if (type == NONE)
					type = TOP;
				else if (type == LEFT)
					type = LEFT_TOP;
				else if (type == BOTTOM)
					type = BOTTOM_TOP;
				else if (type == RIGHT)
					type = RIGHT_TOP;
				else if (type == LEFT_BOTTOM)
					type = LEFT_BOTTOM_TOP;
				else if (type == BOTTOM_RIGHT)
					type = BOTTOM_RIGHT_TOP;
				else if (type == LEFT_RIGHT)
					type = LEFT_RIGHT_TOP;
				else if (type == LEFT_BOTTOM_RIGHT)
					type = ALL;
			}
		}
	}
};

int Room::get_breadth_heuristic()
{
	return breadth_heuristic;
}

void Room::set_breadth_heuristic(int new_value)
{
	breadth_heuristic = new_value;
}

void Room::reset_breadth_heuristic()
{
	breadth_heuristic = std::numeric_limits<int>::max();
}

void Room::set_adjacent_rooms(Room(&maze)[16][16])
{
	// left
	if (loc.x - 1 > 0)
		neighbors.push_back(maze[loc.y][loc.x - 1]);

	// top
	if (loc.y + 1 < 16)
		neighbors.push_back(maze[loc.y + 1][loc.x]);

	// right
	if (loc.x + 1 < 16)
		neighbors.push_back(maze[loc.y][loc.x + 1]);

	// bottom
	if (loc.y - 1 > 0)
		neighbors.push_back(maze[loc.y - 1][loc.x]);
}

std::vector<Room&> & Room::get_children()
{

}