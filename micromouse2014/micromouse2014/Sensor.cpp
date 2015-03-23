#include "Sensor.h"

namespace Hardware
{
	Data::Maze* Sensor::read()
	{
		return new Data::Maze();
	}
}