#include <iostream>
#include <math.h>

class Polygon {
private:
	Vector ele [3];					//Coordinates of each corner of the triangle, represented by vectors
	Vector center;					//Center of the triangle, calculated after each change
	Vector normal;
	precs area;						//Half the length of the crossprod of two sides
	bool status;					//1: active, 0: inactive
	
	Color color;					//BGRA color, 0-255
	float transparency;				//transparency, 0 (none) to 1 (full)
	
public:
	Polygon ();
	Polygon (Vector, Vector, Vector, Color);
	
	~Polygon ();
	
	//Setters
	bool activate();
	void setEle(Vector, Vector, Vector);
	void setColor(Color);
	
	//Getters
	Vector getEle(int);
	Color getColor();
	Vector getCenter();
	Vector getNormal();
	precs getArea();
	bool isActive();
	
	//Functions
	void moveBy(Vector);			//Move center by a given vector
	void moveTo(Vector);			//Move center to a given vector
	void rotateAbs(Vector, precs);	//(absolute in space, relative to origin)
	void rotateRel(Vector, precs);	//(relative to center): Move to origin, rotate, move back
	void scaleAbs(precs);			//(absolute in space, relative to origin)
	void scaleRel(precs);			//(relative to center): Move to origin, scale, move back
	
	void print(string = "");

};