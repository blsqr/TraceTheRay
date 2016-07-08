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
	- check voxels for intersections first
	- lights, shading, shadows, mirrors, transparency...
 - import data for creating the scene, bodies, polygons.
 - use Eigen library for geometry?
 
 BUGS:
 - drawLine: "Fussel"
 - PixelMap outer regions
 - Polygon::isInCone does not check for small pieces of polygons, that are not intersected by the five rays used
 - nextPixel does weird things at high resolutions (jumps from 1300 to -1300, from 179 to 280 ...)
 
 IMPROVE / CHECK:
 - OOP: main as abstract base class for everything in order to have constant objects to inherit
 - speed and memory management
 
*/


//include libs and headers
#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <string.h>
#include <time.h>
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
	Vector a0 (0,0,0);
	Vector a1 (5,0,10);
	Vector a2 (0,5,10);
	Vector a3 (-5,0,10);
	Vector a4 (0,-5,10);
	Vector a5 (10,0,-10);
	Vector a6 (0,10,-10);
	Vector a7 (-10,0,-10);
	Vector a8 (0,-10,-10);
	Vector a9 (0,0,-5);
	
	//Doppelpyramide
	Polygon pg_top_1 (a1,a2,a3,white);
	Polygon pg_top_2 (a1,a3,a4,white);
	
	Polygon pg_tri_1 (a1,a0,a2,red);
	Polygon pg_tri_2 (a2,a0,a3,green);
	Polygon pg_tri_3 (a3,a0,a4,red);
	Polygon pg_tri_4 (a4,a0,a1,green);
	
	Polygon pg_tri_5 (a5,a0,a6,pink);
	Polygon pg_tri_6 (a6,a0,a7,blue);
	Polygon pg_tri_7 (a7,a0,a8,pink);
	Polygon pg_tri_8 (a8,a0,a5,blue);
	
	Polygon pg_btm_1 (a5,a6,a7,white);
	Polygon pg_btm_2 (a5,a7,a8,white);

	
	/*Matrix mat_a (0,1,0,1,0,0,0,0,1);
	Matrix mat_b (1,0,1);
	Matrix mat_c (1,2,0,2,3,0,3,4,1);*/
	
	//Camera tests
	Vector pos (-45,27,15);
	Vector dir (1,-0.6,-0.32);
	Camera theCam	= Camera(pos, dir, 0, 500, 500, 46.8, 25);
	//theCam.print("theCam:");
	
	t_init = clock();
	
	PixelMap * render = theCam.render();
	
	t_render = clock();
	
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
	t_finish = clock();
	
	cout << "Runtimes: " << endl;
	cout << "\tInit:   " << (t_init-t_start) << endl;
	cout << "\tRender: " << (t_render-t_init) << endl;
	cout << "\tFile:   " << (t_finish-t_render) << endl;
	
    return 0;
}
//#######################

void importPolygons(string path) {

}