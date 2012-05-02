using namespace std;

typedef float precs;                //precision of the coordinates

//Initialize classes and functions
class Vector;
class Matrix;
class Polygon;
class PixelMap;
class Ray;
class Camera;


//color structure, bgra
struct Color {
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;
	
	Color(unsigned char, unsigned char, unsigned char, unsigned char);
	
	void print();
};
Color::Color(unsigned char b=255, unsigned char g=255, unsigned char r=255, unsigned char a=0) {
	this->b = b;
	this->g = g;
	this->r = r;
	this->a = a;
}
void Color::print() {
	cout << "Color (BGRA): " << (short) this->b << ", " << (short) this->g << ", " << (short) this->r << ", " << (short) this->a << endl;
}

//coordinates structure
struct Coord {
	//display coordinates
	short x;
	short y;
	
	//precise coordinates for internal calculations
	precs xp; 
	precs yp; 
	
	Coord(short, short);
	bool operator== (Coord);
	Coord operator= (short);
	void apply(); //rounds the precise coordinates and applies the new values to the shorts
	void sync(); //applies the shorts to the precise coordinates
	void print();
};
Coord::Coord(short x=0, short y=0) {
	this->x = x;
	this->y = y;
	this->xp = x;
	this->yp = y;
}
bool Coord::operator== (Coord param) {
	if (this->x == param.x && this->y == param.y) return true;
	else return false;
}
Coord Coord::operator= (short param) {
	this->x = param;
	this->y = param;
	this->apply();
	return *this;
}
void Coord::apply() {
	this->x = (short) round(this->xp);
	this->y = (short) round(this->yp);
}
void Coord::sync() {
	this->xp = this->x;
	this->yp = this->y;
}
void Coord::print() {
	cout << "Coordinate: " << this->x << ", " << this->y << endl;
}


//Include other headers
#include "Vector.h"
#include "Matrix.h"
#include "Polygon.h"
#include "PixelMap.h"
#include "Ray.h"
#include "Camera.h"


//Define some objects
const Color white (255, 255, 255); const Color black (0, 0, 0);
const Coord nullCoord (0,0);
const Vector origin;
const Vector baseVx (1,0,0);
const Vector baseVy (0,1,0);
const Vector baseVz (0,0,1);
const Matrix identity (1,1,1);
const Polygon nullPolygon ();

//dynamic array holding all polygons
vector<Polygon*> allPolygons;


//Include classes
#include "Vector.cpp"
#include "Matrix.cpp"
#include "Polygon.cpp"
#include "PixelMap.cpp"
#include "Ray.cpp"
#include "Camera.cpp"