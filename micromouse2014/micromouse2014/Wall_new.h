#pragma once
#ifndef __WALL_NEW_H__
#define __WALL_NEW_H__

#include <vector>
#include "Room.h"

class Wall_new
{
public:
	Wall_new();
	Wall_new(int closed_value);
	Wall_new(int closed_value, Room * new_parent);
	~Wall_new();

private:
	bool open;
	int known;	// starts at -1 (unknown), and increments up with more assurances
	std::vector<Room*> parents;
public:
	
};



#endif