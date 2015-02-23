#include "wall.h"

Wall::Wall()
{
	closed = false;
	known = -1;
}

Wall::Wall(bool closed_value)
{
	closed = closed_value;

	known = 0;	// known
}



Wall::~Wall()
{
}

bool Wall::getClosed()
{
	return closed;
}

void Wall::setClosed(bool _value)
{
	closed = _value;
}

/**
Get the parent opposite from the given one
*/
Room * Wall::getOtherParent(Room * parent)
{
	for (auto p : parents)
	{
		if (p == parent)
			return p;
	}
}

// sets the designated parent (mother = 0, father = 1)
// of the Wall object
void Wall::setParent(int spouse, Room* parent)
{
	// I thought this would work.. but through debugging
	// I noticed that if this method is called (which
	// occurs in the Maze::setWallParents() method),
	// the opposite parent will return to NULL..
	// Carlton - see if you get the same result
	parents[spouse] = parent;
}