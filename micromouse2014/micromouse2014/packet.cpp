#include "packet.h"

packet::packet()
{
	m1 = 0;
	m2 = 0;
	m3 = 0;
	m4 = 0;
}

packet::packet(double one, double two, double three, double four, int i)
{
	m1 = one;
	m2 = two;
	m3 = three;
	m4 = four;

	aveRadius = (one + two + three + four) / 4;
}

void packet::fill(double a, double b, double c, double d)
{
	m1 = a;
	m2 = b;
	m3 = c;
	m4 = d;
}