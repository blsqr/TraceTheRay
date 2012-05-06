#include <iostream>
#include <math.h>

class Camera {
private:
	//basic variables. c stands for camera, v for view.
	Vector cPos;					//position of the virtual camera
	Vector cDir;					//vector pointing along the center center ray, normalized
	precs cAngle;					//angle the camera is turned around camDir != viewAngle!
	short vWidth, vHeight;			//width and height of the resulting image in pixels
	precs vAngle;					//angle of view (max 180°) measured along the diagonal
	short vDivNo;					//number of current division, size defined in main.h (should be 10)
	
	//variables to help in calculations (not accessible with setters or getters, changed and accessed directly)
	short vDivMax;					//total number of divisions
	Coord vPos;						//current pixel for which the ray is generated
	Ray vRay;						//ray to the current pixel
	Vector vDiag;					//the rising diagonal within the view
	Vector vX, vY;					//the vectors along the sides (x goes to the right, y to the top)
	Vector vdX, vdY;				//fractions of vX and vY representing a move to the next x or y pixel respectively
	Vector vBL;						//vector to the bottom left of the view, starting point for rendering
	

public:
	Camera(Vector, Vector, precs, short, short, precs);

	void setCamera(Vector, Vector, precs, bool);
	void setView(short, short, precs, bool);
	void setVPos(Coord);
	void resetVPos();
	void setVDivNo(short);
	
	Vector getCPos();
	Vector getCDir();
	precs getCAngle();
	short getVWidth();
	short getVHeight();
	precs getVAngle();
	Coord getVPos();
	Coord getVDivNo();
	
	bool nextPixel();				//viewPos is moved to the next pixel within div. Returns true when it is at the end
	bool nextDiv();					//sets the viewPos to the first pixel of the next division
	bool applyVC();					//calculates basic variables after changes of view and/or camera, returns true if successful
	void generateRay(bool);			//generates a ray according to the vPos. Sets ray.pos only if argument is true
	PixelMap * render(short);		//generates rays for every pixel of the view, the rays calculate their interceptions with polygons and return the pointer to those polygons. Then the color is checked, associated with the specific pixel of the view and returned as a pointer to a PixelMap. If a factor is passed, it will be rendered antialiased.
	void print(string);
};