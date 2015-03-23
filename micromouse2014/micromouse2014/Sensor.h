#pragma once
#ifndef __SENSOR_H__
#define __SENSOR_H__

#include <array>
#include "Maze.h"

namespace Hardware
{
	class Sensor
	{
	public:
		Data::Maze* read();
	};
}
#endif