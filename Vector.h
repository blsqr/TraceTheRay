#include <iostream>
#include <math.h>
#include <string.h>


class Vector {
private:
    precs ele[3];					//Coordinates. 0 is x, 1 is y, 2 is z
    
public:
    
    //constructors / destructors
    Vector ();
	Vector (precs, precs, precs);
    Vector (precs [3]);
    
    ~Vector () {};
    
    //Operators
    Vector operator+ (Vector);
    Vector operator- (Vector);
    precs operator* (Vector);       //scalarprod
    Vector operator* (precs);       //multiplicated by scalar
    Vector operator/ (Vector);      //crossprod (yep, ugly, I know. But convenient)
	bool operator== (Vector);		//compare whether two vectors have equal components
    
    //Functions
    void setEle(precs, precs, precs);
	void setEle(precs [3]);
	void setEle(short, precs);
	
	void normalize();
	
	void rotate(Matrix);			//rotate around origin with given matrix (includes the angle)
	void rotate(Vector, precs);		//rotate around axis given by vector with angle
	void rotX(precs);				//rotate around 1-axis / x-axis with angle
	void rotY(precs);				//rotate around 2-axis / y-axis with angle
	void rotZ(precs);				//rotate around 3-axis / z-axis with angle
	
	precs scalarprod(Vector, Vector);
	Vector crossprod(Vector, Vector);
	precs angleTo(Vector);			//angle between this and another vector
	
	bool isSameSide(Vector, Vector, Vector, Vector);
	bool isInPolygon(Polygon);
	bool isInCone(Vector, Vector, Vector, Vector, Vector);
	
	//Getters
    precs getLength();

	precs getEle(short);	
	precs getX();
	precs getY();
	precs getZ();
    
    void print(string = "");
};