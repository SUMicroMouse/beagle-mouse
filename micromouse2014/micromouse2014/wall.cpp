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
