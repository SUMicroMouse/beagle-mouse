#include "Sensor.h"

namespace Hardware
{
	Data::Maze* Sensor::read(Data::Maze* maze)
	{
		Data::Maze* sensor_maze = new Data::Maze(5);
		// Below loops clear maze so all rooms are empty with no walls (unknown)
		for (int i = 0; i < 25; i++)
			for (int j = 0; j < 4; j++)
				sensor_maze->operator()(i / 5, i % 5)->setWall(j, false);

		// check if walls exist around you
		Data::Room* current = maze->operator()(maze->Current()->x, maze->Current()->y);
		std::vector<Data::Wall*> walls = current->getWalls();
		int currentX = current->Location().x;
		int currentY = current->Location().y;
		int currentSensorX = 2;
		int currentSesnorY = 2;

		sensor_maze->operator()(currentX, currentY)->operator=(*current);

		bool left, bottom, right, top;
		left = bottom = right = top = false;
		if (!walls[0]->getClosed()) // if left wall isn't there..
		{
			sensor_maze->operator()(currentSensorX, currentSesnorY - 1)->operator=(*maze->operator()(currentX, currentY - 1));
			left = true;
		}
		if (!walls[1]->getClosed()) // if bottom wall isn't there..
		{
			sensor_maze->operator()(currentSensorX + 1, currentSesnorY)->operator=(*maze->operator()(currentX + 1, currentY));
			bottom = true;
		}
		if (!walls[2]->getClosed()) // if right wall isn't there..
		{
			sensor_maze->operator()(currentSensorX, currentSesnorY + 1)->operator=(*maze->operator()(currentX, currentY + 1));
			right = true;
		}
		if (!walls[3]->getClosed()) // if top wall isn't there..
		{
			sensor_maze->operator()(currentSensorX - 1, currentSesnorY)->operator=(*maze->operator()(currentX - 1, currentY));
			top = true;
		}

		if (left)
		{
			current = maze->operator()(maze->Current()->x, maze->Current()->y - 1);
			walls = current->getWalls();
			currentX = current->Location().x;
			currentY = current->Location().y;
			currentSensorX = 2;
			currentSesnorY = 1;

			if (!walls[0]->getClosed()) // if left wall isn't there..
				sensor_maze->operator()(currentSensorX, currentSesnorY - 1)->operator=(*maze->operator()(currentX, currentY - 1));
			if (!walls[1]->getClosed()) // if bottom wall isn't there..
				sensor_maze->operator()(currentSensorX + 1, currentSesnorY)->operator=(*maze->operator()(currentX + 1, currentY));
			if (!walls[2]->getClosed()) // if right wall isn't there..
				sensor_maze->operator()(currentSensorX, currentSesnorY + 1)->operator=(*maze->operator()(currentX, currentY + 1));
			if (!walls[3]->getClosed()) // if top wall isn't there..
				sensor_maze->operator()(currentSensorX - 1, currentSesnorY)->operator=(*maze->operator()(currentX - 1, currentY));
		}
		if (bottom)
		{
			current = maze->operator()(maze->Current()->x + 1, maze->Current()->y);
			walls = current->getWalls();
			currentX = current->Location().x;
			currentY = current->Location().y;
			currentSensorX = 3;
			currentSesnorY = 2;

			if (!walls[0]->getClosed()) // if left wall isn't there..
				sensor_maze->operator()(currentSensorX, currentSesnorY - 1)->operator=(*maze->operator()(currentX, currentY - 1));
			if (!walls[1]->getClosed()) // if bottom wall isn't there..
				sensor_maze->operator()(currentSensorX + 1, currentSesnorY)->operator=(*maze->operator()(currentX + 1, currentY));
			if (!walls[2]->getClosed()) // if right wall isn't there..
				sensor_maze->operator()(currentSensorX, currentSesnorY + 1)->operator=(*maze->operator()(currentX, currentY + 1));
			if (!walls[3]->getClosed()) // if top wall isn't there..
				sensor_maze->operator()(currentSensorX - 1, currentSesnorY)->operator=(*maze->operator()(currentX - 1, currentY));
		}
		if (right)
		{
			current = maze->operator()(maze->Current()->x, maze->Current()->y + 1);
			walls = current->getWalls();
			currentX = current->Location().x;
			currentY = current->Location().y;
			currentSensorX = 2;
			currentSesnorY = 3;

			if (!walls[0]->getClosed()) // if left wall isn't there..
				sensor_maze->operator()(currentSensorX, currentSesnorY - 1)->operator=(*maze->operator()(currentX, currentY - 1));
			if (!walls[1]->getClosed()) // if bottom wall isn't there..
				sensor_maze->operator()(currentSensorX + 1, currentSesnorY)->operator=(*maze->operator()(currentX + 1, currentY));
			if (!walls[2]->getClosed()) // if right wall isn't there..
				sensor_maze->operator()(currentSensorX, currentSesnorY + 1)->operator=(*maze->operator()(currentX, currentY + 1));
			if (!walls[3]->getClosed()) // if top wall isn't there..
				sensor_maze->operator()(currentSensorX - 1, currentSesnorY)->operator=(*maze->operator()(currentX - 1, currentY));
		}
		if (top)
		{
			current = maze->operator()(maze->Current()->x - 1, maze->Current()->y);
			walls = current->getWalls();
			currentX = current->Location().x;
			currentY = current->Location().y;
			currentSensorX = 1;
			currentSesnorY = 2;

			if (!walls[0]->getClosed()) // if left wall isn't there..
				sensor_maze->operator()(currentSensorX, currentSesnorY - 1)->operator=(*maze->operator()(currentX, currentY - 1));
			if (!walls[1]->getClosed()) // if bottom wall isn't there..
				sensor_maze->operator()(currentSensorX + 1, currentSesnorY)->operator=(*maze->operator()(currentX + 1, currentY));
			if (!walls[2]->getClosed()) // if right wall isn't there..
				sensor_maze->operator()(currentSensorX, currentSesnorY + 1)->operator=(*maze->operator()(currentX, currentY + 1));
			if (!walls[3]->getClosed()) // if top wall isn't there..
				sensor_maze->operator()(currentSensorX - 1, currentSesnorY)->operator=(*maze->operator()(currentX - 1, currentY));
		}

		return sensor_maze;
	}

	void Sensor::sense(Data::Maze * maze)
	{
		int x_center = maze->Current()->x, y_center = maze->Current()->y;

		for (int x = x_center - 2; x <= x_center + 2; x++)
		{
			if (x >= 0 && x < 16)
			{
				// Make all the room's walls known, increasing its known value
				Data::Room * Ro = maze->RoomGet(x, y_center);
				std::vector<Data::Wall*> & walls = Ro->getWalls();
				std::vector<Data::Wall*>::iterator wI = walls.begin();
				while (wI != walls.end())
				{
					Data::Wall *cwall = *wI;
					cwall->known++;
					wI++;
				}
			}
		}

		for (int y = y_center - 2; y <= y_center + 2; y++)
		{
			if (y >= 0 && y < 16)
			{
				// Make all the room's walls known, increasing its known value
				Data::Room * Ro = maze->RoomGet(x_center, y);
				std::vector<Data::Wall*> & walls = Ro->getWalls();
				std::vector<Data::Wall*>::iterator wI = walls.begin();
				while (wI != walls.end())
				{
					Data::Wall *cwall = *wI;
					cwall->known++;
					wI++;
				}
			}
		}
	}
}