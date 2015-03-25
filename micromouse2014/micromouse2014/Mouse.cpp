#include "Mouse.h"

using namespace Data;
using namespace Hardware;

namespace Algorithm
{
	Mouse::Mouse() :
		// member-initializer list
		maze(new Data::Maze()),
		sensor(new Hardware::Sensor())
	{
		location = maze->Start();
		currentRoom = maze->CurrentRoom();
		maze->printMaze(false);
	}

	Mouse::~Mouse()
	{
		//delete maze;
		//delete sensor;
	}

	void Mouse::Explore()
	{
		std::unique_ptr<Path_new> * path_chosen;

		auto senseGrid = this->sensor->read(maze.get());

		// Start off going forward once, set previous to start
		MoveForward();

		while (true)
		{
			// Get information from sensors
			//auto senseGrid = this->sensor->read(maze.get());

			// Check to see if there's more than one new path in the current physical room
			switch (CheckForOptions())
			{
			case 0:	// dead end
				motor->motion(MotorMotion::TURN_AROUND);
				break;

			case 1:	// only option is straight ahead
				MoveForward();
				break;

			case 2:	// 2 or 3 options in direction
			case 3:
				// Guess all the possible paths from the current "cross-roads"
				path_chosen = Evaluate();
				// Start down the new path
				Turn(path_chosen->get());
				MoveForward();
				break;
			}
			int i = 0;
		}

	}

	void Mouse::Turn(Path_new * newPath)
	{
		// make path the new complete guess Path
		Path_new *newCompleteGuessPath = new Path_new();

		//get room in front of mouse and move to room
		Room * nextRoom = newPath->Rooms()->front();
		//Move(nextRoom);
	}

	/* Turn to face the given room. Then move to it */
	void Mouse::Turn(Room * room)
	{
		if (room == nullptr)
			return;
		// If the room was not the room just visited
		if ((room->Location().x != previousRoom->Location().x)
			&& (room->Location().y != previousRoom->Location().y))
		{
			switch (this->direction)
			{
			case Direction::Left:

				if (room->Location().x < location->x);
				else if (room->Location().x > location->x)
					motor->motion(MotorMotion::TURN_AROUND);
				else if (room->Location().y < location->y)
					motor->motion(MotorMotion::TURN_LEFT);
				else if (room->Location().y > location->y)
					motor->motion(MotorMotion::TURN_RIGHT); 
				else
					return;
				// always move forward
				//motor->motion(MotorMotion::MOVE_FORWARD);
				break;

			case Direction::Up:
				if (room->Location().x < location->x)
					motor->motion(MotorMotion::TURN_LEFT);
				else if (room->Location().x > location->x)
					motor->motion(MotorMotion::TURN_RIGHT);
				else if (room->Location().y < location->y)
					motor->motion(MotorMotion::TURN_AROUND);
				else if (room->Location().y > location->y);
				else
					return;
				// always move forward
				//motor->motion(MotorMotion::MOVE_FORWARD);
				break;

			case Direction::Right:
				if (room->Location().x < location->x)
					motor->motion(MotorMotion::TURN_AROUND);
				else if (room->Location().x > location->x);
				else if (room->Location().y < location->y)
					motor->motion(MotorMotion::TURN_RIGHT);
				else if (room->Location().y > location->y)
					motor->motion(MotorMotion::TURN_LEFT);
				else
					return;
				// always move forward
				//motor->motion(MotorMotion::MOVE_FORWARD);
				break;

			case Direction::Down:
				if (room->Location().x < location->x)
					motor->motion(MotorMotion::TURN_RIGHT);
				else if (room->Location().x > location->x)
					motor->motion(MotorMotion::TURN_LEFT);
				else if (room->Location().y < location->y);
				else if (room->Location().y > location->y)
					motor->motion(MotorMotion::TURN_AROUND);
				else
					return;
				// always move forward
				//motor->motion(MotorMotion::MOVE_FORWARD);
				break;

			default:
				break;
			}
		}
	}

	void Mouse::MoveForward()
	{
		motor->motion(MotorMotion::MOVE_FORWARD);
		previousRoom = currentRoom;

		int x = currentRoom->Location().x, y = currentRoom->Location().y;

		switch (this->direction)
		{
		case Direction::Up:
			currentRoom = maze->RoomGet(x, y + 1);
			break;
		case Direction::Right:
			currentRoom = maze->RoomGet(x + 1, y);
			break;
		case Direction::Left:
			currentRoom = maze->RoomGet(x - 1, y);
			break;
		case Direction::Down:
			currentRoom = maze->RoomGet(x, y - 1);
			break;
		}
	}

	std::unique_ptr<Path_new> * Mouse::Evaluate()
	{
		

		// Update the cells' heuristic values starting from the farthest goal cell
		Searches::breadth_first_search(*maze, 
				8,//
				8);	////
			//	/	//	/	/	//	///	//	/

		// Generate Paths
		// Filter/Choose Path
		auto path_selected = GeneratePaths();


		return path_selected;
	}

	int Mouse::CheckForOptions()
	{
		std::vector<Room*>* children = currentRoom->get_children(previousRoom);
		return children->size();
	}

	/**
	Generate possible paths and filter down to the best path
	Return that chosen path
	*/
	std::unique_ptr<Path_new> * Mouse::GeneratePaths()
	{
		// Generate paths
		std::deque<Path_new*> *ps = Searches::depth_search(*maze, 
															*location, 
															*new Location(8, 8));// 
		// / / /// ///
		//
		///
		//////////
		///
		//
		std::deque<std::unique_ptr<Path_new>> paths(ps->begin(), ps->end());

		// Filter paths
		Filter::mode = ExploreMode::Direct;
		std::unique_ptr<Path_new> * path_selected = Filter::bestPath(&paths);

		return path_selected;
	}
}