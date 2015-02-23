#ifndef __PATH_NEW_H__
#define __PATH_NEW_H__

#include <deque>
#include "Room.h"

namespace Algorithm
{
	class Path_new
	{
	public:
		Path_new();
		Path_new(Path_new *old_path, Room * new_room);
		~Path_new();

	private:
		std::deque<Room*> rooms;
	};
}

#endif