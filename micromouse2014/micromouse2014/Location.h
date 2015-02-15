class Location
{
	int x;
	int y;
public:
	Location(int xx, int yy){ x = xx; y = yy; }
	int getX(){ return x; }
	int getY(){ return y; }
	void setX(int xx){ x = xx; }
	void setY(int yy){ y = yy; }
};