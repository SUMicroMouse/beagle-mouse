#include "Maze.h"

static int openings = 0;

/// <summary>
/// Method find
/// This method checks to see if Location loc
/// exists in the current vector of Locations
/// that contains the candidates for a new
/// Room object.
/// </summary>
int Maze::find(Location loc)
{
	for (unsigned int i = 0; i < opening_locations.size(); i++)
		if (opening_locations[i].x == loc.x && opening_locations[i].y == loc.y)
			return i;
	return -1;
}

/// <summary>
/// Method clearMaze
/// This method clears the maze and initializes
/// the grid.
/// </summary>
void Maze::clearMaze()
{
	int dimensions = 16;
	for (int i = 0; i < dimensions; i++)
		for (int j = 0; j < dimensions; j++)
			maze[i][j] = *new Room(-1,i,j);
}

void Maze::cleanMaze()
{
	Room topLeft, topRight, bottomLeft, bottomRight;
	int dimensions = 16;
	for (int i = 0; i < dimensions-1; i++) // rows
	{
		for (int j = 0; j < dimensions-1; j++) // columns
		{
			if (i == 7 && j == 7)
				continue;
			topLeft = maze[i][j];
			topRight = maze[i][j + 1];
			bottomLeft = maze[i + 1][j];
			bottomRight = maze[i + 1][j + 1];
			if (!topLeft.getOpenings()[2] && !topRight.getOpenings()[0] && !bottomLeft.getOpenings()[2] && !bottomRight.getOpenings()[0] &&
				!topLeft.getOpenings()[1] && !topRight.getOpenings()[1] && !bottomLeft.getOpenings()[3] && !bottomRight.getOpenings()[3])
			{
				maze[i + 1][j + 1].setWall(3,true);
				maze[i][j + 1].setWall(1,true);
			}
		}
	}
}

/// <summary>
/// Method makeMaze
/// This method is called after initMaze() and will
/// generate the remainder of the grid based on the
/// rooms that were initialized in initMaze().
/// </summary>
void Maze::makeMaze()
{
	Location* loc; // a Location (X,Y) object, used for locating open rooms
	Room* r; // a Room object, used for generating rooms to put in grid
	int choice = -1; // decides which room to make
	int dimensions = 16; // dimensions of maze

	// Diregard lastX and lastY; they're here to determine which room
	// was generated last. Variables X and Y refer to the current location
	// being looked at as a candidate for a new room. Variable currentLocSize
	// is a dummy variable to hold the size of the vector containing all of
	// the valid locations to put a room. This changes within the loop and
	// therefore needs to be stored in a temporary variable.
	int X = 0, Y = 0, currentLocSize = 0;

	// Variable iterations is a safety; if anything goes wrong, iterations
	// will tell the program to retry.
	int iterations = 0;

	// while the maze isn't full..
	while (rooms < dimensions*dimensions && openings > 0)
	{
		iterations++;
		if (iterations > dimensions*dimensions)
		{
			// realistically, this number should never be
			// reached if there are no errors because multiple
			// rooms are being created in each iteration.
			rooms = 0;
			initMaze();
			makeMaze();
			return;
		}

		// set the temporary variable
		currentLocSize = opening_locations.size();
		for (int j = 0; j < currentLocSize; j++) // how many spots have openings
		{
			// get the current candidate for a new room
			X = opening_locations[j].x;
			Y = opening_locations[j].y;
			
			choice = getChoice(X, Y); // decide which room to make
			r = new Room(choice,X,Y); // make the room
			rooms++; // increment number of rooms
			openings += r->getPassages() - getAdjacentRooms(X,Y)*2; // update the amounts of pathways remaining in grid
			maze[X][Y] = *r; // add the room to the grid

			// Check all four sides of new rooms and find the
			// existing rooms around it as well as the spots
			// that would be candidates for a new room. I
			// have the initial if statement to avoid IndexOutOfBounds
			// errors.
			if (X - 1 >= 0) // check above
				if (maze[X - 1][Y].getPassages() == 0)
					if (maze[X][Y].getOpenings()[3] == 0)
					{
						loc = new Location(X - 1, Y);
						if (find(*loc) == -1)
							opening_locations.push_back(*loc);
					}

			if (X + 1 < dimensions) // check below
				if (maze[X + 1][Y].getPassages() == 0)
					if (maze[X][Y].getOpenings()[1] == 0)
					{
						loc = new Location(X + 1, Y);
						if (find(*loc) == -1)
							opening_locations.push_back(*loc);
					}

			if (Y - 1 >= 0) // check left
				if (maze[X][Y - 1].getPassages() == 0)
					if (maze[X][Y].getOpenings()[0] == 0)
					{
						loc = new Location(X, Y - 1);
						if (find(*loc) == -1)
							opening_locations.push_back(*loc);
					}

			if (Y + 1 < dimensions) // check right
				if (maze[X][Y + 1].getPassages() == 0)
					if (maze[X][Y].getOpenings()[2] == 0)
					{
						loc = new Location(X, Y + 1);
						if (find(*loc) == -1)
							opening_locations.push_back(*loc);
					}
		}
		for (int j = 0; j < currentLocSize; j++)
			opening_locations.pop_front();
	}

	cleanMaze();
}

/// <summary>
/// Method initMaze
/// This method will initialze a new maze, clearing
/// any old information and resetting the random seed
/// to ensure each maze is (most likely) different
/// from the last. It also creates the four corners
/// and four middle rooms.
/// </summary>
void Maze::initMaze()
{
	srand(time(NULL));

	// clears/initializes maze
	clearMaze();

	int dimensions = 16;

	Room* r;

	/******CENTER ROOMS******/
	// use choice to randomly choose which center 4 rooms to make
	// There are 8 possible combinations..
	// *NOTE: options go 0-7, although below they are labeled 1-8
	int random = rand() % 8;

	/******OPTIONS******/
	// OPTION 1:
	// opening on left-bottom
	//
	// OPTION 2:
	// opening left-top
	//
	// OPTION 3:
	// opening top-left
	//
	// OPTION 4:
	// opening top-right
	//
	// OPTION 5:
	// opening on right-top
	//
	// OPTION 6:
	// opening right-bottom
	//
	// OPTION 7:
	// opening bottom-right
	//
	// OPTION 8:
	// opening bottom-left
	/*******************/
		
	// using random, make each room individually as opposed to 4 separate set ups..
		
	// top left room
	if (random != 1 && random != 2) // all choices besides 1 and 2 are the same for top left..
		r = new Room(9,7,7); // left and top are walls
	else if (random == 1)
	{
		r = new Room(5,7,7); // top is a wall
		opening_locations.push_back(*(new Location(7,6)));
	}
	else // random = 2
	{
		r = new Room(2,7,7); // left is a wall
		opening_locations.push_back(*(new Location(6,7)));
	}

	rooms++;
	maze[7][7] = *r;

	// top right room
	if (random != 3 && random != 4) // all choices besides 3 and 4 are the same for top right..
		r = new Room(11,7,8); // right and top are walls
	else if (random == 3)
	{
		r = new Room(4,7,8); // right is a wall
		opening_locations.push_back(*(new Location(6,8)));
	}
	else // random = 4
	{
		r = new Room(5,7,8); // top is a wall
		opening_locations.push_back(*(new Location(7,9)));
	}

	rooms++;
	maze[7][8] = *r;

	// bottom right room
	if (random != 5 && random != 6) // all choices besides 5 and 6 are the same for bottom right..
		r = new Room(7,8,8); // right and bottom are walls
	else if (random == 5)
	{
		r = new Room(3,8,8); // bottom is a wall
		opening_locations.push_back(*(new Location(8,9)));
	}
	else // random = 6
	{
		r = new Room(4,8,8); // right is a wall
		opening_locations.push_back(*(new Location(9,8)));
	}

	rooms++;
	maze[8][8] = *r;

	// bottom left room
	if (random != 7 && random != 0) // all choices besides 7 and 0 are the same for bottom left..
		r = new Room(6,8,7); // left and bottom are walls
	else if (random == 7)
	{
		r = new Room(2,8,7); // left is a wall
		opening_locations.push_back(*(new Location(9,7)));
	}
	else // random = 0
	{
		r = new Room(3,8,7); // bottom is a wall
		opening_locations.push_back(*(new Location(8,6)));
	}

	rooms++;
	maze[8][7] = *r;

	/******CORNER ROOMS******/

	// top left
	r = new Room(getChoice(0,0),0,0);
	rooms++;
	maze[0][0] = *r;
	if (!maze[0][0].getOpenings()[1])
		opening_locations.push_back(*(new Location(1, 0)));

	if (!maze[0][0].getOpenings()[2])
		opening_locations.push_back(*(new Location(0, 1)));

	// top right
	r = new Room(getChoice(0, dimensions - 1),0,dimensions-1);
	rooms++;
	maze[0][dimensions-1] = *r;
	if (!maze[0][dimensions-1].getOpenings()[0])
		opening_locations.push_back(*(new Location(1, dimensions - 1)));

	if (!maze[0][dimensions-1].getOpenings()[1])
		opening_locations.push_back(*(new Location(0, dimensions - 2)));

	// bottom right
	r = new Room(getChoice(dimensions - 1, dimensions - 1),dimensions-1,dimensions-1);
	rooms++;
	maze[dimensions - 1][dimensions - 1] = *r;
	if (!maze[dimensions - 1][dimensions - 1].getOpenings()[0])
		opening_locations.push_back(*(new Location(dimensions - 1, dimensions - 2)));

	if (!maze[dimensions - 1][dimensions - 1].getOpenings()[3])
		opening_locations.push_back(*(new Location(dimensions - 2, dimensions - 1)));

	// bottom left
	r = new Room(getChoice(dimensions - 1, 0),dimensions-1,0);
	rooms++;
	maze[dimensions - 1][0] = *r;
	if (!maze[dimensions - 1][0].getOpenings()[2])
		opening_locations.push_back(*(new Location(dimensions - 1, 1)));

	if (!maze[dimensions - 1][0].getOpenings()[3])
		opening_locations.push_back(*(new Location(dimensions - 2, 0)));

	openings = opening_locations.size();
}

/// <summary>
/// Method getAdjacentRooms
/// This method is returns the number of rooms directly
/// connected to the given location. This can be modified
/// to return the actual Room objects if needed..
/// </summary>
int Maze::getAdjacentRooms(int x, int y)
{
	int dimensions = 16;
	int total = 0;
	if (x - 1 >= 0) // check above
		if (maze[x - 1][y].getPassages() != 0 && maze[x - 1][y].getOpenings()[1] == 0)
			total++;
	if (x + 1 < dimensions) // check below
		if (maze[x + 1][y].getPassages() != 0 && maze[x + 1][y].getOpenings()[3] == 0)
			total++;
	if (y - 1 >= 0) // check left
		if (maze[x][y - 1].getPassages() != 0 && maze[x][y - 1].getOpenings()[2] == 0)
			total++;
	if (y + 1 < dimensions) // check right
		if (maze[x][y + 1].getPassages() != 0 && maze[x][y + 1].getOpenings()[0] == 0)
			total++;
	return total;
}

/// <summary>
/// Method printMaze
/// This method prints out the maze in a nice fashion.
/// </summary>
void Maze::printMaze()
{
	int dimensions = 16;
	char room[9];
	// 3 "rows" = row; 3 "columns" = column
	for (int i = 0; i < dimensions; i++) // rows
	{
		for (int k = 0; k < 3; k++) // "rows"
		{
			for (int j = 0; j < dimensions; j++) // columns
			{
				memcpy(room, maze[i][j].getRoom(), 9);
				for (int m = 0; m < 3; m++) // "columns"
				{
					if (m == 1 && k == 1)
					{
						if (maze[i][j].getPassages() != 0)
							std::cout << " ";
						else
							std::cout << room[m + (k * 3)];
					}
					else
						std::cout << room[m + (k * 3)];
				}
			}
			if (k % 3 == 1)
				std::cout << " " << i + 1;
			std::cout << std::endl;
		}
	}
	// last row
	for (int i = 0; i < dimensions; i++)
		if (i < 10)
			std::cout << " " << i + 1 << " ";
		else
			std::cout << " " << i + 1;
}

/// <summary>
/// Method getChoice
/// This method is where the algorithm decides which
/// room to generate based on the current location
/// and the rooms around the given location. The
/// number it returns is passed to a Room object
/// and in the Room constructor is where the initialization
/// of the actual Room object occurs. This can be modified
/// to simply return the Room object...
/// </summary>
int Maze::getChoice(int x, int y) // current X(row) value, current Y(column) value
{
	int dimensions = 16;
	std::vector<int> choices; // contains a list of possible rooms for each location (x,y)
	int choice = -1; // the chosen room, to be returned
	
	// Variables up, down, left, right determine whether or not
	// the generated room MUST have an opening on a specific side.
	// Variables roomUp, roomDown, roomLeft, roomRight state
	// whether or not there is a room adjacent to the given location (x,y).
	bool up = false, down = false, left = false, right = false; // true if there's an exit
	bool roomUp = false, roomDown = false, roomLeft = false, roomRight = false; // true if room exists

	// first if statements are to avoid IndexOutOfBounds errors
	if (x - 1 >= 0) // check above
	{
		if (maze[x - 1][y].getPassages() != 0) // room exists
		{
			roomUp = true;
			if (!maze[x - 1][y].getOpenings()[1])
				up = true;
		}
	}
	else
		roomUp = true;

	if (x + 1 < dimensions) // check below
	{
		if (maze[x + 1][y].getPassages() != 0)
		{
			roomDown = true;
			if (!maze[x + 1][y].getOpenings()[3])
				down = true;
		}
	}
	else
		roomDown = true;

	if (y - 1 >= 0) // check left
	{
		if (maze[x][y - 1].getPassages() != 0)
		{
			roomLeft = true;
			if (!maze[x][y - 1].getOpenings()[2])
				left = true;
		}
	}
	else
		roomLeft = true;

	if (y + 1 < dimensions) // check right
	{
		if (maze[x][y + 1].getPassages() != 0)
		{
			roomRight = true;
			if (!maze[x][y + 1].getOpenings()[0])
				right = true;
		}
	}
	else
		roomRight = true;

	// *****************************************************************************
	// which rooms can go where?
	// This portion of code may seem long; however,
	// it's broken down so that any case will only
	// hit a maximum of 8 if statements before deciding
	// on which rooms to add. The last case technically
	// can allow for more if statements to occur, but
	// that case is only accessed if you arbitrarily
	// add a room, like in the initMaze method.
	if (up) // up
	{
		if (down) // up,down
		{
			if (left) // up,down,left
			{
				if (right) // up,down,left,right
					choices.push_back(1);
				else // up,down,left,!right
				{
					if (roomRight || (openings > 3 && rooms > dimensions*dimensions*(3.0 / 4.0)) || (rooms == dimensions*dimensions - 1))
						choices.push_back(4);
					else
						choices.push_back(1);
				}
			}
			else // up,down,!left
			{
				if (right) // up,down,!left,right
				{
					if (roomLeft || (openings > 3 && rooms > dimensions*dimensions*(3.0 / 4.0)) || (rooms == dimensions*dimensions - 1))
						choices.push_back(2);
					else
						choices.push_back(1);
				}
				else // up,down,!left,!right
				{
					if (roomLeft)
					{
						if (roomRight || (openings > 3 && rooms > dimensions*dimensions*(3.0 / 4.0)) || (rooms == dimensions*dimensions - 1))
							choices.push_back(8);
						else
							choices.push_back(2);
					}
					else
					{
						choices.push_back(4);
						if (!roomRight)
						{
							choices.push_back(1);
							choices.push_back(2);
						}
					}
				}
			}
		}
		else // up,!down
		{
			if (left) // up,!down,left
			{
				if (right) // up,!down,left,right
				{
					if (roomDown || (openings > 3 && rooms > dimensions*dimensions*(3.0 / 4.0)) || (rooms == dimensions*dimensions - 1))
						choices.push_back(3);
					else
						choices.push_back(1);
				}
				else // up,!down,left,!right
				{
					if (roomDown)
					{
						if (roomRight || (openings > 3 && rooms > dimensions*dimensions*(3.0 / 4.0)) || (rooms == dimensions*dimensions - 1))
							choices.push_back(7);
						else
							choices.push_back(3);
					}
					else
					{
						choices.push_back(4);
						if (!roomRight)
						{
							choices.push_back(1);
							choices.push_back(3);
						}
					}
				}
			}
			else // up,!down,!left
			{
				if (right) // up,!down,!left,right
				{
					if (roomDown)
					{
						if (roomLeft || (openings > 3 && rooms > dimensions*dimensions*(3.0 / 4.0)) || (rooms == dimensions*dimensions - 1))
							choices.push_back(6);
						else
							choices.push_back(3);
					}
					else
					{
						choices.push_back(2);
						if (!roomLeft)
						{
							choices.push_back(1);
							choices.push_back(3);
						}
					}
				}
				else // up,!down,!left,!right
				{
					if (roomDown)
					{
						if (roomLeft)
						{
							if (roomRight || (openings > 3 && rooms > dimensions*dimensions*(3.0 / 4.0)) || (rooms == dimensions*dimensions - 1))
								choices.push_back(12);
							else
								choices.push_back(6);
						}
						else
						{
							choices.push_back(7);
							if (!roomRight)
							{
								choices.push_back(3);
								choices.push_back(6);
							}
						}
					}
					else
					{
						if (roomLeft)
						{
							choices.push_back(8);
							if (!roomRight)
							{
								choices.push_back(2);
								choices.push_back(6);
							}
						}
						else
						{
							choices.push_back(4);
							choices.push_back(7);
							choices.push_back(8);
							if (!roomRight)
							{
								choices.push_back(1);
								choices.push_back(2);
								choices.push_back(3);
								choices.push_back(6);
							}
						}
					}
				}
			}
		}
	}
	else // !up
	{
		if (down)  // !up,down
		{
			if (left) // !up,down,left
			{
				if (right) // !up,down,left,right
				{
					if (roomUp || (openings > 3 && rooms > dimensions*dimensions*(3.0 / 4.0)) || (rooms == dimensions*dimensions - 1))
						choices.push_back(5);
					else
						choices.push_back(1);
				}
				else // !up,down,left,!right
				{
					if (roomUp)
					{
						if (roomRight || (openings > 3 && rooms > dimensions*dimensions*(3.0 / 4.0)) || (rooms == dimensions*dimensions - 1))
							choices.push_back(11);
						else
							choices.push_back(5);
					}
					else
					{
						choices.push_back(4);
						if (!roomRight)
						{
							choices.push_back(1);
							choices.push_back(5);
						}
					}
				}
			}
			else // !up,down,!left
			{
				if (right) // !up,down,!left,right
				{
					if (roomUp)
					{
						if (roomLeft || (openings > 3 && rooms > dimensions*dimensions*(3.0 / 4.0)) || (rooms == dimensions*dimensions - 1))
							choices.push_back(9);
						else
							choices.push_back(5);
					}
					else
					{
						choices.push_back(2);
						if (!roomLeft)
						{
							choices.push_back(1);
							choices.push_back(5);
						}
					}
				}
				else // !up,down,!left,!right
				{
					if (roomUp)
					{
						if (roomLeft)
						{
							if (roomRight || (openings > 3 && rooms > dimensions*dimensions*(3.0 / 4.0)) || (rooms == dimensions*dimensions - 1))
								choices.push_back(14);
							else
								choices.push_back(9);
						}
						else
						{
							choices.push_back(11);
							if (!roomRight)
							{
								choices.push_back(5);
								choices.push_back(9);
							}
						}
					}
					else
					{
						if (roomLeft)
						{
							choices.push_back(8);
							if (!roomRight)
							{
								choices.push_back(2);
								choices.push_back(9);
							}
						}
						else
						{
							choices.push_back(4);
							choices.push_back(8);
							choices.push_back(11);
							if (!roomRight)
							{
								choices.push_back(1);
								choices.push_back(2);
								choices.push_back(5);
								choices.push_back(9);
							}
						}
					}
				}
			}
		}
		else // !up,!down
		{
			if (left) // !up,!down,left
			{
				if (right) // !up,!down,left,right
				{
					if (roomUp)
					{
						if (roomDown || (openings > 3 && rooms > dimensions*dimensions*(3.0 / 4.0)) || (rooms == dimensions*dimensions - 1))
							choices.push_back(10);
						else
							choices.push_back(5);
					}
					else
					{
						choices.push_back(3);
						if (!roomDown)
						{
							choices.push_back(1);
							choices.push_back(5);
						}
					}
				}
				else // !up,!down,left,!right
				{
					if (roomUp)
					{
						if (roomDown)
						{
							if (roomRight || (openings > 3 && rooms > dimensions*dimensions*(3.0 / 4.0)) || (rooms == dimensions*dimensions - 1))
								choices.push_back(15);
							else
								choices.push_back(10);
						}
						else
						{
							choices.push_back(11);
							if (!roomRight)
							{
								choices.push_back(5);
								choices.push_back(10);
							}
						}
					}
					else
					{
						if (roomDown)
						{
							choices.push_back(7);
							if (!roomRight)
							{
								choices.push_back(3);
								choices.push_back(10);
							}
						}
						else
						{
							choices.push_back(4);
							choices.push_back(7);
							choices.push_back(11);
							if (!roomRight)
							{
								choices.push_back(1);
								choices.push_back(3);
								choices.push_back(5);
								choices.push_back(10);
							}
						}
					}
				}
			}
			else // !up,!down,!left
			{
				if (right) // !up,!down,!left,right
				{
					if (roomUp)
					{
						if (roomDown)
						{
							if (roomLeft || (openings > 3 && rooms > dimensions*dimensions*(3.0 / 4.0)) || (rooms == dimensions*dimensions - 1))
								choices.push_back(13);
							else
								choices.push_back(10);
						}
						else
						{
							choices.push_back(9);
							if (!roomLeft)
							{
								choices.push_back(5);
								choices.push_back(10);
							}
						}
					}
					else
					{
						if (roomDown)
						{
							choices.push_back(6);
							if (!roomLeft)
							{
								choices.push_back(3);
								choices.push_back(10);
							}
						}
						else
						{
							choices.push_back(2);
							choices.push_back(6);
							choices.push_back(9);
							if (!roomLeft)
							{
								choices.push_back(1);
								choices.push_back(3);
								choices.push_back(5);
								choices.push_back(10);
							}
						}
					}
				}
				else // !up,!down,!left,!right
				{
					if (x == 0 && y == 0) // top left
					{
						choices.push_back(9);
						choices.push_back(13);
						choices.push_back(14);
					}
					else if (x == 0 && y == dimensions - 1) // top right
					{
						choices.push_back(11);
						choices.push_back(14);
						choices.push_back(15);
					}
					else if (x == dimensions - 1 && y == 0) // bottom left
					{
						choices.push_back(6);
						choices.push_back(12);
						choices.push_back(13);
					}
					else if (x == dimensions - 1 && y == dimensions - 1) // bottom right
					{
						choices.push_back(7);
						choices.push_back(12);
						choices.push_back(15);
					}
					else // not a corner
					{
						for (int i = 1; i < 16; i++)
							choices.push_back(i);
					}
				}
			}
		}
	}

	// As a safety; this statement should always
	// be satisfied.
	if (choices.size() > 0)
	{
		int rando = rand() % choices.size();
		choice = choices[rando];
		choices.clear();
	}

	return choice;
}



/*****TEST THE MAZE*****/
/* For testing purposes. Put this into another */
/* cpp file to test it. */
void main()
{
	Maze m;
	std::cout << "\n\n";
	m.printMaze();
	getchar();
}