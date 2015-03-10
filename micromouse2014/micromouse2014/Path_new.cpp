#include "Path_new.h"

namespace Algorithm
{
	int Path_new::pathCount;	// identifier for each path
	ExploreMode Path_new::mode = ExploreMode::None;

	Path_new::Path_new()
	{
	}

	/**
	Create a new path that is a copy of the given one with the addition of the new room
	*/
	Path_new::Path_new(Path_new *old_path, Room *new_room)
	{
		rooms = new std::deque<Room*>(*old_path->rooms);	// copy
		rooms->push_back(new_room);
		path_number = pathCount++;
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

	/********* Variables for calculating path weight *********/

	// Distance from beginning room to ending room
	float Path_new::displacement()
	{

	}

	int Path_new::length()
	{
		return rooms->size();
	}

	// Accumulated certainty from all of the cells
	float Path_new::certainty()
	{

	}
}