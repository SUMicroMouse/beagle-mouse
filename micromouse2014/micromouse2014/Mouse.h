#ifndef __MOUSE_H__
#define __MOUSE_H__

#include <deque>
#include <memory>

#include "Direction.h"
#include "Filter.h"
#include "Motor_new.h"
#include "Room.h"
#include "Searches.h"
#include "Sensor.h"

namespace Algorithm
{
	class Mouse
	{
		Data::Direction direction;
		Location *location;
		std::unique_ptr<Data::Maze> maze;
		std::unique_ptr<Hardware::Sensor> sensor;
		std::unique_ptr<Hardware::Motor> motor;
		
	public:
		Mouse();
		~Mouse();

		void Explore();
		std::unique_ptr<Path_new> * Evaluate();
		
		// Turn to the path's front room and add the path to the current path
		void Turn(Path_new * newPath);

		void Turn(Data::Room * room);

	private:
		bool CheckForOptions();

		/* Generate paths, return best path */
		std::unique_ptr<Path_new> * GeneratePaths();

		// Path that includes the actual path + the current best-guess to the goal
		Path_new * completePathGuess;

		/* Path that the mouse has actually taken */
		Path_new * actualPath;

		std::unique_ptr<Data::Room> previousRoom;

		std::deque<Data::Room*> visitedRooms;
	};	
}

#endif