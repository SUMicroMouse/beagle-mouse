#include "Path_new.h"

namespace Algorithm
{
	int Path_new::pathCount;	// identifier for each path
	ExploreMode Path_new::mode = ExploreMode::None;

	Path_new::Path_new()
	{
		_rooms = new std::deque<Room*>();
		path_number = pathCount++;
		edgeCertainty = -1;
		innerCertainty = -1;
		success = false;
	}

	Path_new::Path_new(Room *new_room)
	{
		_rooms = new std::deque<Room*>();
		_rooms->push_back(new_room);
		path_number = pathCount++;
		edgeCertainty = -1;
		innerCertainty = -1;
		success = false;
	}

	/**
	Create a new path that is a copy of the given one with the addition of the new room
	*/
	Path_new::Path_new(Path_new *old_path, Room *new_room)
	{
		_rooms = new std::deque<Room*>(*old_path->_rooms);	// copy
		_rooms->push_back(new_room);
		path_number = pathCount++;
		edgeCertainty = -1;
		innerCertainty = -1;

		success = old_path->success;
		//copyCheckedValues(old_path);
	}

	Path_new::~Path_new()
	{
	}

	int Path_new::Number()
	{
		return path_number;
	}

	bool Path_new::operator<(Path_new &p2)
	{
		
		return false;
	}

	bool Path_new::operator>(Path_new &p2)
	{
		
		return false;
	}

	bool Path_new::operator==(Path_new &p2)
	{

		return false;
	}

	bool Path_new::operator<=(Path_new &p2)
	{

		return false;
	}

	bool Path_new::operator>=(Path_new &p2)
	{

		return false;
	}

	// Get the checked values in each cell corresponding to the old path
	// and copy these to the position for the new path
	void Path_new::copyCheckedValues(Path_new * old_path)
	{
		std::deque<Room*> *rpoint = old_path->_rooms;
		std::deque<Room*>::iterator rIt = rpoint->begin();
		while (rIt != rpoint->end())
		{
			(*rIt)->checked[this->path_number] = (*rIt)->checked[old_path->path_number];
			rIt++;
		}
	}

	/********* Variables for calculating path weight *********/

	// Distance from beginning room to ending room
	float Path_new::displacement()
	{
		Room * beginning = _rooms->front(), *end = _rooms->back();

		float xsquared = std::abs(beginning->Location().x - end->Location().x) * std::abs(beginning->Location().x - end->Location().x);
		float ysquared = std::abs(beginning->Location().y - end->Location().y) * std::abs(beginning->Location().y - end->Location().y);

		return std::sqrt(xsquared + ysquared);
	}

	int Path_new::length()
	{
		return _rooms->size();
	}

	// Accumulated certainty from all of the cells
	float Path_new::confidence(bool edges)
	{
		int _certainty = 0;
		std::deque<Room*>::iterator dIt = _rooms->begin();
		while (dIt != _rooms->end())
		{
			if (!edges)
				_certainty += (*dIt)->confidence(this->path_number, 0);
			else
				_certainty += (*dIt)->confidence(this->path_number, 1);
			++dIt;
		}
		
		return _certainty;
	}

	int Path_new::turns()
	{
		int c = 0;
		std::deque<Room*>::iterator dIt = _rooms->begin();
		while (dIt != _rooms->end())
		{
			c += (*dIt)->turn(this->path_number);
			++dIt;
		}

		return 0;
	}

	std::deque<Room*> * Path_new::Rooms()
	{
		return _rooms;	// probably should return a copy. oh well
	}

	std::vector<Room*> * Path_new::RoomsVector()
	{
		std::vector<Room*> * rooms = new std::vector<Room*>(_rooms->begin(), _rooms->end());
		return rooms;
	}

	/* Check if the path contains the location */
	bool Path_new::Contains(int x, int y)
	{
		std::deque<Room*>::iterator dIt = _rooms->begin();
		while (dIt != _rooms->end())
		{
			auto room = *dIt;
			if (room->Location().x == x && room->Location().y == y)
				return true;
			++dIt;
		}

		return false;
	}
}