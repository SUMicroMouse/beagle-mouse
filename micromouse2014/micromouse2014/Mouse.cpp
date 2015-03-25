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
		currentRoom = maze->CurrentRoom();

		// Set initial direction
		InitialDirection();

		// Print maze after setting direction
		maze->printMaze(false);
	}

	Mouse::~Mouse()
	{
		//delete maze;
		//delete sensor;
	}

	/* Set the initial direction using what's available as determined by the walls */
	void Mouse::InitialDirection()
	{
		/*
		// (0, 0)
		if (location()->x == 0 && location()->y == 0)
		{
			direction = Direction::Up;
		}
		// (0, 15)
		else if (location()->x == 0 && location()->y == 15)
		{
			direction = Direction::Right;
		}
		// (15, 0)
		else if (location()->x == 15 && location()->y == 0)
		{
			direction = Direction::Up;
		}
		// (15, 15)
		else if (location()->x == 15 && location()->y == 15)
		{
			direction = Direction::Left;
		}
		*/

		// Face the wall that's open

		std::vector<Wall*> & walls = currentRoom->getWalls();
		if (walls[0]->getClosed() == false) // down
			direction = Direction::Down;
		else if (!walls[1]->getClosed()) // right
			direction = Direction::Right;
		else if (!walls[2]->getClosed()) // up
			direction = Direction::Up;
		else
			direction = Direction::Left;	// left

		// Update maze's direction
		maze->setDirection(direction);
	}

	void Mouse::Explore()
	{
		std::unique_ptr<Path_new> * path_chosen;

		//auto senseGrid = this->sensor->read(maze.get());

		// Turn to face a direction where there's actually an opening
		// Start off going forward once, set previous to start
		MoveForward();

		maze->printClean();
		maze->printMaze(false);
		maze->printClean();
		
		while (true)
		{
			// Get information from sensors
			//auto senseGrid = this->sensor->read(maze.get());

			// Check to see if there's more than one new path in the current physical room
			switch (CheckForOptions())
			{
			// dead end
			case 0:	
				TurnAround();
				break;

			// only one option
			case 1:	
				// Get the next cell to turn to as there's only one option 
				Turn(currentRoom->get_children(previousRoom)->front());
				MoveForward();
				break;

			// 2 or 3 options in direction
			case 2:	
			case 3:
				// Guess all the possible paths from the current "cross-roads"
				path_chosen = Evaluate();

				// Start down the new path
				Turn(path_chosen->get());
				MoveForward();
				break;
			}

			maze->printClean();
			maze->printMaze(false);
			maze->printClean();
		}

	}

	void Mouse::Turn(Path_new * newPath)
	{
		// make path the new complete guess Path
		Path_new *newCompleteGuessPath = new Path_new();

		//get room to continue to
		Room * nextRoom = newPath->Rooms()->front();

		// Turn toward the room
		Turn(nextRoom);
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

				if (room->Location().x < location()->x);
				else if (room->Location().x > location()->x)
				{
					direction = Direction::Right;
					maze->setDirection(direction);
					motor->motion(MotorMotion::TURN_AROUND);
				}
				else if (room->Location().y < location()->y)
				{
					direction = Direction::Down;
					maze->setDirection(direction);
					motor->motion(MotorMotion::TURN_LEFT);
				}
				else if (room->Location().y > location()->y)
				{
					direction = Direction::Up;
					maze->setDirection(direction);
					motor->motion(MotorMotion::TURN_RIGHT);
				}
				else
					return;
				// always move forward
				//motor->motion(MotorMotion::MOVE_FORWARD);
				break;

			case Direction::Up:
				if (room->Location().x < location()->x)
				{
					direction = Direction::Left;
					maze->setDirection(direction);
					motor->motion(MotorMotion::TURN_LEFT);
				}
				else if (room->Location().x > location()->x)
				{
					direction = Direction::Right;
					maze->setDirection(direction);
					motor->motion(MotorMotion::TURN_RIGHT);
				}
				else if (room->Location().y < location()->y)
				{
					direction = Direction::Down;
					maze->setDirection(direction);
					motor->motion(MotorMotion::TURN_AROUND);
				}
				else if (room->Location().y > location()->y);
				else
					return;
				// always move forward
				//motor->motion(MotorMotion::MOVE_FORWARD);
				break;

			case Direction::Right:
				if (room->Location().x < location()->x)
				{
					direction = Direction::Left;
					maze->setDirection(direction);
					motor->motion(MotorMotion::TURN_AROUND);
				}
				else if (room->Location().x > location()->x);
				else if (room->Location().y < location()->y)
				{
					direction = Direction::Down;
					maze->setDirection(direction);
					motor->motion(MotorMotion::TURN_RIGHT);
				}
				else if (room->Location().y > location()->y)
				{
					direction = Direction::Up;
					maze->setDirection(direction);
					motor->motion(MotorMotion::TURN_LEFT);
				}
				else
					return;
				// always move forward
				//motor->motion(MotorMotion::MOVE_FORWARD);
				break;

			case Direction::Down:
				if (room->Location().x < location()->x)
				{
					direction = Direction::Left;
					maze->setDirection(direction);
					motor->motion(MotorMotion::TURN_RIGHT);
				}
				else if (room->Location().x > location()->x)
				{
					direction = Direction::Right;
					maze->setDirection(direction);
					motor->motion(MotorMotion::TURN_LEFT);
				}
				else if (room->Location().y < location()->y);
				else if (room->Location().y > location()->y)
				{
					direction = Direction::Up;
					maze->setDirection(direction);
					motor->motion(MotorMotion::TURN_AROUND);
				}
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

	/* Switch to the opposite direction. Call the motor to do turn around */
	void Mouse::TurnAround()
	{
		switch (direction)
		{
		case Data::Left:
			direction = Direction::Right;
			maze->setDirection(direction);
			break;
		case Data::Up:
			direction = Direction::Down;
			maze->setDirection(direction);
			break;
		case Data::Right:
			direction = Direction::Left;
			maze->setDirection(direction);
			break;
		case Data::Down:
			direction = Direction::Up;
			maze->setDirection(direction);
			break;
		}
		motor->motion(MotorMotion::TURN_AROUND);
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

		// Update the Maze's current pointer for printing & future reference
		maze->Current(&currentRoom->Location());
	}

	Location * Mouse::location()
	{
		if (currentRoom == nullptr)
			return nullptr;

		return new Location(currentRoom->Location().x, currentRoom->Location().y);
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
															*location(),
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