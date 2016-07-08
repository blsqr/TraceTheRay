#include <iostream>
#include <math.h>

class Ray {
private:
	Vector pos, dir;						//start-pos and direction of the ray
	
public:
	Ray(Vector, Vector);
	
	void setPos(Vector);
	void setDir(Vector);
	
	Vector getPos();
	Vector getDir();
	
	Polygon * trace(vector<Polygon*> *);	//returns the pointer to the nearest intersected polygon, or null pointer
	
	bool isIntersecting(Polygon *);
	
	void print(string);
};

