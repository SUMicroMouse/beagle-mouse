#include <vector>

using namespace std;

class Room
{
	char* room;
	vector<int> openings; // 0 means open, 1 means closed
	int opens;
public:
	Room()
	{
		opens = 0;
		room = new char[9];
		for (int i = 0; i < 4; i++) // left,bottom,right,top is order of walls
			openings.push_back(0); // initially all walls are open
	}
	Room(int v)
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
		room = new char[9]; opens = 0;
		for (int i = 0; i < 4; i++) // left,bottom,right,top is order of walls
			openings.push_back(0); // initially all walls are open
		switch (v)
		{
		case 1: // none
			room[0] = 188; room[1] = 32; room[2] = 200; room[3] = 32; room[4] = 32; room[5] = 32; room[6] = 187; room[7] = 32; room[8] = 201;
			opens = 4; break;
		case 2: // left
			room[0] = 186; room[1] = 32; room[2] = 200; room[3] = 186; room[4] = 32; room[5] = 32; room[6] = 186; room[7] = 32; room[8] = 201;
			openings[0] = 1; opens = 3; break;
		case 3: // bottom
			room[0] = 188; room[1] = 32; room[2] = 200; room[3] = 32; room[4] = 32; room[5] = 32; room[6] = 205; room[7] = 205; room[8] = 205;
			openings[1] = 1; opens = 3; break;
		case 4: // right
			room[0] = 188; room[1] = 32; room[2] = 186; room[3] = 32; room[4] = 32; room[5] = 186; room[6] = 187; room[7] = 32; room[8] = 186;
			openings[2] = 1; opens = 3; break;
		case 5: // top
			room[0] = 205; room[1] = 205; room[2] = 205; room[3] = 32; room[4] = 32; room[5] = 32; room[6] = 187; room[7] = 32; room[8] = 201;
			openings[3] = 1; opens = 3; break;
		case 6: // left bottom
			room[0] = 186; room[1] = 32; room[2] = 200; room[3] = 186; room[4] = 32; room[5] = 32; room[6] = 200; room[7] = 205; room[8] = 205;
			openings[0] = 1; openings[1] = 1; opens = 2; break;
		case 7: // bottom right
			room[0] = 188; room[1] = 32; room[2] = 186; room[3] = 32; room[4] = 32; room[5] = 186; room[6] = 205; room[7] = 205; room[8] = 188;
			openings[1] = 1; openings[2] = 1; opens = 2; break;
		case 8: // left right
			room[0] = 186; room[1] = 32; room[2] = 186; room[3] = 186; room[4] = 32; room[5] = 186; room[6] = 186; room[7] = 32; room[8] = 186;
			openings[0] = 1; openings[2] = 1; opens = 2; break;
		case 9: // left top
			room[0] = 201; room[1] = 205; room[2] = 205; room[3] = 186; room[4] = 32; room[5] = 32; room[6] = 186; room[7] = 32; room[8] = 201;
			openings[0] = 1; openings[3] = 1; opens = 2; break;
		case 10: // bottom top
			room[0] = 205; room[1] = 205; room[2] = 205; room[3] = 32; room[4] = 32; room[5] = 32; room[6] = 205; room[7] = 205; room[8] = 205;
			openings[1] = 1; openings[3] = 1; opens = 2; break;
		case 11: // right top
			room[0] = 205; room[1] = 205; room[2] = 187; room[3] = 32; room[4] = 32; room[5] = 186; room[6] = 187; room[7] = 32; room[8] = 186;
			openings[2] = 1; openings[3] = 1; opens = 2; break;
		case 12: // left bottom right
			room[0] = 186; room[1] = 32; room[2] = 186; room[3] = 186; room[4] = 32; room[5] = 186; room[6] = 200; room[7] = 205; room[8] = 188;
			openings[0] = 1; openings[1] = 1; openings[2] = 1; opens = 1; break;
		case 13: // left bottom top
			room[0] = 201; room[1] = 205; room[2] = 205; room[3] = 186; room[4] = 32; room[5] = 32; room[6] = 200; room[7] = 205; room[8] = 205;
			openings[0] = 1; openings[1] = 1; openings[3] = 1; opens = 1; break;
		case 14: // left right top
			room[0] = 201; room[1] = 205; room[2] = 187; room[3] = 186; room[4] = 32; room[5] = 186; room[6] = 186; room[7] = 32; room[8] = 186;
			openings[0] = 1; openings[2] = 1; openings[3] = 1; opens = 1; break;
		case 15: // bottom right top
			room[0] = 205; room[1] = 205; room[2] = 187; room[3] = 32; room[4] = 32; room[5] = 186; room[6] = 205; room[7] = 205; room[8] = 188;
			openings[1] = 1; openings[2] = 1; openings[3] = 1; opens = 1; break;

		default:
			for (int i = 0; i < 9; i++){ room[i] = 176; }
			for (int i = 0; i < 4; i++){ openings[i] = -1; } break;
		}
	}

	vector<int> getOpenings(){ return openings; }
	int getPassages(){ return opens; }
	char* getRoom(){ return room; }
	void setOpenings(int index, int newNum){ openings[index] = newNum; }
	void setPassages(int numOpenings){ opens = numOpenings; }
	void setRoom(char* s){ memcpy(room, s, 9); }
};