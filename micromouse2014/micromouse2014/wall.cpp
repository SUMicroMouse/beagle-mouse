#include "wall.h"

namespace Data
{
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
			if (p != parent)
				return p;
		}
		return NULL;
	}

	// sets the designated parent (mother = 0, father = 1)
	// of the Wall object
	void Wall::setParent(int spouse, Room* parent)
	{
		parents[spouse] = parent;
	}
}