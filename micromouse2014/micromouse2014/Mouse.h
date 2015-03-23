#ifndef __MOUSE_H__
#define __MOUSE_H__

#include <deque>
#include <memory>

#include "Direction.h"
#include "Filter.h"
#include "Searches.h"
#include "Sensor.h"

namespace Algorithm
{
	class Mouse
	{
		Location *location;
		std::unique_ptr<Data::Maze> maze;
		std::unique_ptr<Hardware::Sensor> sensor;
		
	public:
		Mouse();
		~Mouse();

		void Evaluate();

	private:
		/* Generate paths, return best path */
		std::unique_ptr<Path_new> * GeneratePaths();
	};	
}

#endif