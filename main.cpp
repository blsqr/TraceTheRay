/*
SIMPLE GEOMETRY
 
 Included OPERATIONS for vectors and matrices:
 - add and substract vectors		v+w, v-w
 - multiply vector by scalar		v*s
 - scalar product of vectors		v*w, scalarprod(v,w)
 - cross product of two vector		v/w, crossprod(v,w)
 - add and substract matrices		m+n, m-n
 - multiply matrix by scalar		m*s						(not s*m!)
 - multiply two matrices			m*n
 - multiply matrix and vector		m*v						(not v*m! vectors: 3x1-matrices)
 
 - normalize vector					normalize(v)
 - rotate vector					rotate(m), rotate(axis, angle), rotX(angle)...
 
 - determinant						getDet(m)
 - trace							getTrace(m)
 - transpose						transpose(m)
 - adjugate							adjugate(m)
 - invert							invert(m)				(returns empty matrix, if getDet(m) == 0)
 - create rotation matrix			Matrix rotation (axis, angle)
 
 
 GRAPHICS:
 - create a PixelMap				PixelMap a (name, width, height)
 - create a Color (in BGR!)			Color color (blue, green, red)
 - add noise to PixelMap			a.noise([bool])			(TRUE for color noise)
 - draw a line in a PixelMap		a.drawLine(p1, p2, color, width)
 - anti aliase PixelMap				a2 = a.antialiase(factor)
 - create a bmp file from PixelMap	a.createFile()
 
 
 RAYTRACER:
 - Ray.trace() returns pointer to nearest intersected polygon.
 
 
 TO DO:
 - draw basic shapes in PixelMap (rectangle, circle...)
 - classes for
	bodies (connected polygons)
 - RayTracer
	- lights, shading, shadows, mirrors, transparency...
 - import data for creating the scene, bodies, polygons.
 
 BUGS:
 - drawLine: "Fussel"
 - PixelMap Randbereiche... 
 - Camera applyVC ...
 - Polygon checking...
 
 IMPROVE / CHECK:
 - OOP: main as abstract base class for everything in order to have constant objects to inherit
 - check crossprod operator /
 - speed and memory management
 
*/


//include libs and headers
#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <vector>

#include "main.h"


//#######################
int main () {
	//SETTINGS
	cout.precision(5);
	cout.setf(ios::fixed,ios::floatfield);
	//END OF SETTINGS
	
	
	//MAIN PROGRAM ------------------------
	
	//create vectors, matrices, polygons
	Vector a1 (0,5,0);
	Vector b1 (0,15,5);
	Vector c1 (0,0,5);
	Vector a2 (0,-5,0);
	Vector b2 (0,-10,-5);
	Vector c2 (0,0,-5);
	Vector d (-10,0,0);
	Vector e (1,0,0);
	
	/*Matrix mat_a (0,1,0,1,0,0,0,0,1);
	Matrix mat_b (1,0,1);
	Matrix mat_c (1,2,0,2,3,0,3,4,1);*/
	
	Color red (0, 0, 255);
	Color green (0, 255, 0);
	
	Polygon pg1 (a1,b1,c1,red);
	Polygon pg2 (a2,b2,c2,green);
	
	/*Ray theRay(d,e);
	
	Polygon * intersected = theRay.trace();
	
	intersected->print("intersected");
	*/
	
	
	//Camera tests
	Camera theCam	= Camera(d, e, 0, 640, 360, 46.8);
	//theCam.print("theCam:");
	
	PixelMap * render = theCam.render();
	render->createFile();
	
	
	/*//creates a test PixelMap object (name, width, height)
	PixelMap test ("test", 1024, 1024);
	
	//add noise
	//test.noise();
	
	//set points, colors (BGR!) and then draw three lines (from, to, color, width)
	Coord p1 (511,511);
	Coord p2 (4096,4096);
	Coord p3 (0,1023);
	Coord p4 (1023,0);
	Coord p5 (511,0);
	Coord p6 (511,511);
	
	test.drawLine(p1, p2, white, 42);
	test.drawLine(p1, p2, red, 31);
	test.drawLine(p3, p4, red, 5);
	test.drawLine(p5, p6, green, 5);
	
	//create the file test.bmp
	test.createFile();
	
	//create an antialiased object and the file test_aa.bmp
	PixelMap test_aa = test.antiAliase(8);
	test_aa.createFile();//*/
	
	//END OF MAIN PROGRAM -----------------
	
    return 0;
}
//#######################