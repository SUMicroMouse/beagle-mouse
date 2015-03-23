#include "Motor_new.h"

namespace Hardware
{
	void Motor::motion(MotorMotion motion)
	{
		switch (motion)
		{
		case 0: // TURN_LEFT
			break;
		case 1: // TURN_RIGHT
			break;
		case 2: // TURN_ABOUT
			break;
		case 3: // MOVE_FORWARD
			break;
		case 4: // MOVE_BACKWARD
			break;
		default:
			return;
		}
	}
}