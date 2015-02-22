#ifndef __WALL_H__
#define __WALL_H__

//#include "Room.h"

class Wall
{
public:
	Wall();
	Wall(bool closed_value);
	~Wall();

	int Confidence();
	int Value();

	bool getClosed() { return closed; }
	void setClosed(bool _closed) { closed = _closed; }

private:
	bool closed;
	int known;	// starts at -1 (unknown), and increments up with more assurances

	//friend class Room;
};

#endif