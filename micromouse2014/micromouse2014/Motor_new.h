#pragma once
#ifndef __MOTOR_H__
#define __MOTOR_H__

namespace Hardware
{
	enum MotorMotion
	{
		TURN_LEFT,
		TURN_RIGHT,
		TURN_ABOUT,
		MOVE_FORWARD,
		MOVE_BACKWARD
	};

	class Motor
	{
		void motion(MotorMotion motion);
	};
}
#endif