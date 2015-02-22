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

Wall_new::Wall_new()
{
	open = false;
	known = -1;
	parents = { nullptr, nullptr };	// initialize parents vector to 2 null pointers
}

Wall_new::Wall_new(int closed_value)
{
	if (!closed_value)
		open = true;

	known = 0;	// known

	parents = { nullptr, nullptr };	// initialize parents vector to 2 null pointers
}

Wall_new::Wall_new(int closed_value, Room * new_parent)
{
	if (!closed_value)
		open = true;

	known = 0;	// known
	parents = { new_parent, nullptr };	// initialize parents vector 
}

Wall_new::~Wall_new()
{
}

#endif