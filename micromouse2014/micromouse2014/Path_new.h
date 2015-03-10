#ifndef __PATH_NEW_H__
#define __PATH_NEW_H__

#include <deque>
#include <assert.h>
#include "ExploreMode.h"
#include "Room.h"

namespace Algorithm
{
	class Path_new
	{
	public:
		Path_new();
		Path_new(Path_new *old_path, Room * new_room);
		~Path_new();

		static int pathCount;	// identifier for each path
		
		int Number();

		bool operator <(Path_new &p2);
		bool operator >(Path_new &p2);
		bool operator ==(Path_new &p2);
		static ExploreMode mode;
	private:
		std::deque<Room*> *rooms;
		int path_number;

		/********* Variables for calculating path weight *********/

		// Distance from beginning room to ending room
		float displacement();
	public:
		// Number of rooms
		int length();
	private:
		// Accumulated certainty from all of the cells
		float certainty();

	};
}

#endif