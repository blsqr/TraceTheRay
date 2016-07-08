#include <iostream>

//Struct for the bitmap file
class PixelMap {	
	
	//header structures
	short type;
	struct Header {               
		int		size;                       
		short	reserved1, reserved2;
		int		offset; 
	};
	struct Info  {
		unsigned int	size;
		signed int		width, height;
		unsigned short	planes, colorDepth;
		unsigned int	compression, imageSize;
		signed int		horizontalRes, verticalRes;
		unsigned int	colors, importantColors;
	};
	
	//general properties
	unsigned short w, h;		//dimensions
	Coord pencilPos;			//coordinates of the pencil
	precs pencilAngle;			//angle of the pencil, the direction in which it is moved
	Color **pixel;				//pixelmap
	string name, filename;		//name and filename (filename includes .bmp)
	
	//regarding the pencil: it is only a temporary position, it does not draw when moved but only when a method is called which sets a pixel to a color.
	
	
public:
	PixelMap (string, short, short, bool);
	
	void setDim(short, short);					//set dimensions of the pixelmap
	
	void setPixel(Color, bool);					//pass color struct, use pencil position, move to next pixel, if bool true
	void setPixel(Color, Coord);				//pass color struct and position
	void setPixel(char, char, char, char);		//pass bgra values, use pencil position
	void setPixel(short, short, Color);			//pass color struct, use given position
	void setPixel(short, short, char, char, char, char);	//pass bgra values, a opt., pos given
	
	void setName(string);						//set the filename of the pixelmap
	
	Color getPixel(short, short, bool);			//return & print color of pixel; bool=0 -> no print
	Color getPixel(Coord, bool);				//return & print color of pixel; bool=0 -> no print

	void setPencilTo(Coord);					//set the pencil coordinates
	void setPencilToOrigin();
	
	Coord getPencilPos();						//get the pencil coordinates
	bool isOnCanvas(Coord);						//returns 1 if pencil position is on canvas
	
	void setPencilAngle(precs);
	precs getPencilAngle();
	
	void movePencilBy(precs);					//moves the pencil in direction of the angle
	void movePencil(precs, precs);				//moves the pencil relative to its current position
	
	void nextPixel();							//sets the pencil to the next pixel
	
	void noise(bool);							//create bw noise, set bool to 1 for color noise
	void drawLine(Coord, Coord, Color, float, precs);	//line from p1 to p2, color, step, width
	PixelMap antiAliase(short);					//Merges factor^2 pixels into one new pixel, returns
	
	void createFile();							//write the file
	//void loadFile(string f);					//load a file
};