#ifndef __WALL_NEW_H__
#define __WALL_NEW_H__

class Wall_new
{
public:
	Wall_new();
	Wall_new(int closed_value);
	~Wall_new();

private:
	bool open;
	int known;	// starts at -1 (unknown), and increments up with more assurances

public:
	
};

Wall_new::Wall_new()
{
	open = false;
	known = -1;
}

Wall_new::Wall_new(int closed_value)
{
	if (!closed_value)
		open = true;

	known = 0;	// known
}



Wall_new::~Wall_new()
{
}

#endif