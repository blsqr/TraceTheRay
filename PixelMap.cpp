//Constructors
PixelMap::PixelMap(string temp_name, short width=128, short height=128, bool create=false) {
	name		= temp_name;
	filename	= temp_name+".bmp";
	
	w			= width;
	h			= height;
	
	//create empty but correct size pixelmap
	pixel = new Color*[w];
	for (short i=0; i<w; i++) pixel[i] = new Color[h];
	
	if (create) this->createFile();
}

//Setters
void PixelMap::setDim(short width, short height) {
	this->w = width;
	this->h = height;
}

void PixelMap::setPixel(Color color, bool move=false) {
	if (isOnCanvas(this->pencilPos)) this->pixel[this->pencilPos.x][this->pencilPos.y]	= color;
	if (move) this->nextPixel();
}
void PixelMap::setPixel(char b, char g, char r, char a=0) {
	Color color (b, g, r, a);
	if (isOnCanvas(this->pencilPos)) this->pixel[this->pencilPos.x][this->pencilPos.y]	= color;
}
void PixelMap::setPixel(short x, short y, Color color) {
	if (isOnCanvas(this->pencilPos)) this->pixel[x][y]	= color;
}
void PixelMap::setPixel(short x, short y, char b, char g, char r, char a=0) {
	Color color (b, g, r, a);
	if (isOnCanvas(this->pencilPos)) this->pixel[x][y]	= color;
}

void PixelMap::setName(string name) {
	this->name	= name;
}

//Getters
Color PixelMap::getPixel(short x, short y, bool print=0) {
	Color temp;
	temp = this->pixel[x][y];
	if (print == 1) temp.print();
	
	return temp;
}
Color PixelMap::getPixel(Coord pos, bool print=0) {
	Color temp;
	temp = this->pixel[pos.x][pos.y];
	if (print == 1) temp.print();
	
	return temp;
}

//Pencil
void PixelMap::setPencilTo(Coord pos) {
	this->pencilPos = pos;
}
void PixelMap::setPencilToOrigin() {
	Coord temp;
	this->pencilPos = temp;
}

Coord PixelMap::getPencilPos() {
	return this->pencilPos;
}
bool PixelMap::isOnCanvas(Coord penCilPos) {
	if ((pencilPos.x < this->w) && (pencilPos.y < this->h) && (pencilPos.x >= 0) && (pencilPos.y >= 0)) {
		return 1;
	}
	else return 0;
}

void PixelMap::setPencilAngle(precs angle) {
	this->pencilAngle = angle;
}
precs PixelMap::getPencilAngle() {
	return this->pencilAngle;
}

void PixelMap::movePencilBy(precs length) {	
	this->pencilPos.xp += cos(this->pencilAngle)*length;
	this->pencilPos.yp += sin(this->pencilAngle)*length;
	
	this->pencilPos.apply();
}
void PixelMap::movePencil(precs x, precs y) {
	this->pencilPos.xp += x;
	this->pencilPos.yp += y;
	
	this->pencilPos.apply();
}

void PixelMap::nextPixel() {
	Coord temp	= getPencilPos();
	
	//increment coordinates
	if ((temp.x + 1) == this->w) temp.y = (temp.y + 1) % (this->h);
	temp.x = (temp.x + 1) % (this->w);
	
	this->setPencilTo(temp);
}

//Image manipulation
void PixelMap::noise(bool colorNoise = 0) {
	Color color;
	
	//Save current position
	Coord temp = this->getPencilPos();
	this->setPencilToOrigin();
	
	for (int i=0; i<((this->w) * (this->h)); i++) {		
		if (colorNoise == 1) {
			color.b	= rand()%255;
			color.g	= rand()%255;
			color.r	= rand()%255;
			color.a = 0;
		}
		else {
			int random = rand()%255;
			color.b	= random;
			color.g	= random;
			color.r	= random;
			color.a = 0;
		}
		
		this->setPixel(color);
		this->nextPixel();
	}
	
	//Reset position
	this->setPencilTo(temp);
}

void PixelMap::drawLine(Coord p1, Coord p2, Color color, float width=1.0, precs lengthStep=1.0) {	
	//Set steps for width of line
	float widthStep	= 0.02;
	
	//Save current position and angle
	Coord tempPos	= this->getPencilPos();
	precs tempAngle	= this->getPencilAngle();
	
	//Calculate angle and length, set step
	precs length	= sqrt(pow((p2.x-p1.x), 2) + pow((p2.y-p1.y), 2));
	precs angle		= atan2((p2.y-p1.y),(p2.x-p1.x));
	angle			= fmod(2*M_PI + angle, 2*M_PI);
	precs angle2	= angle+(M_PI/2); //Perpendicular angle
	
	//draw lines. Move along the width first, then draw lines. Repeat
	for (precs widthProgress=0; widthProgress <= width; widthProgress += widthStep) {
		this->setPencilTo(p1); //reset to p1
		
		//Adjust position for parallel lines if width is given. Must be greater than 1!
		if (width > 1) {
			this->setPencilAngle(angle2); //move pencil perpendicular to line
			this->movePencilBy((widthProgress + widthStep) - (0.5*width));
		}
		
		this->setPencilAngle(angle); //reset angle -> move pencil along line
		
		//draw line
		for (precs lengthProgress=0; lengthProgress <= length; lengthProgress += lengthStep) {
			this->movePencilBy(lengthStep);
			this->setPixel(color);
		}
	}
	
	//Reset position and angle
	this->setPencilTo(tempPos);
	this->pencilPos.sync();
	this->setPencilAngle(tempAngle);
}

PixelMap PixelMap::antiAliase(short aa) {
	//check values
	if (aa != 1) {
		if ((this->w)%aa == 0 && (this->h)%aa == 0) {
			//temporary PixelMap with new values
			short waa = (this->w)/aa;
			short haa = (this->h)/aa;
			PixelMap aaImg((this->name)+"_aa", waa, haa);
			
			Color mergedColor;
			Color tempColor;
			Coord tempPos;
			short colorSumB, colorSumG, colorSumR, colorSumA;
			
			aaImg.setPencilToOrigin();
			
			for (int i=0; i<(waa*haa); i++) { //each pixel of aaImg
				
				//temporary position within the original document
				tempPos = aaImg.getPencilPos();
				tempPos.x *= aa;
				tempPos.y *= aa;
				
				//Get the average color of aa^2 pixels
				for (int j=0; j<aa; j++) { //rows of original
					for (int k=0; k<aa; k++) { //columns of originals
						tempColor = this->getPixel(tempPos);
						
						colorSumB += tempColor.b;
						colorSumG += tempColor.g;
						colorSumR += tempColor.r;
						colorSumA += tempColor.a;
						
						//Next column
						tempPos.x++;
					}
					//Next row
					tempPos.y++;
					
					//reset column
					tempPos.x -= aa; 
				}
				
				//Create the final color, average of each color value of corresponding original pixels
				mergedColor.b = (char) (colorSumB * pow(aa,-2));
				mergedColor.g = (char) (colorSumG * pow(aa,-2));
				mergedColor.r = (char) (colorSumR * pow(aa,-2));
				mergedColor.a = (char) (colorSumA * pow(aa,-2));
				
				//Apply
				aaImg.setPixel(mergedColor);
				
				//Reset and move on
				colorSumB = 0; colorSumG = 0;colorSumR = 0; colorSumA = 0;
				
				aaImg.nextPixel();
			}
			
			return aaImg;
		}
		else {
			//error
			cout << "Factor does not match dimensions. Abort antialiasing" << endl;
			PixelMap fail ((this->name)+"_aa(failed)", 1, 1);
			return fail;
		}
	}
	else {
		return *this;
	}
}

//File-handling
void PixelMap::createFile()  {
	short w = this->w;
	short h = this->h; 
	
	//header
	type			= 19778;
	Header header	= {sizeof(Color)*w*h+54, 0, 0, 54};
	Info   info		= {sizeof(Info), w, h, 1, sizeof(Color)*8, 0, (sizeof(Color)*(w*h)), 1, 1, 0, 0};
	
	//open binary filestream
	fstream pic;
	pic.open((this->filename).c_str(), ios::out|ios::binary);
	
	//write reinterpreted header and info data
	pic.write(reinterpret_cast<char*>(&type), sizeof(type));
	pic.write(reinterpret_cast<char*>(&header), sizeof(header));
	pic.write(reinterpret_cast<char*>(&info), sizeof(info));
	
	for (int i=0; i<h; i++) {		//rows
		for (int j=0; j<w; j++) {	//columns
			pic.write((char*) &(this->pixel[j][i]), sizeof(Color));
		}
	}
	
	cout << "Created " << this->filename << " (" << w << "x" << h << ")" << endl;
	
	pic.close();
}

/* //Needs corrections
void Bitmap::loadFile(string name)  {
	string filename = name+".bmp";
	pic.open(name.c_str(), ios::in|ios::binary);
	pic.read(reinterpret_cast<char*>(&type), sizeof(type));
	pic.read(reinterpret_cast<char*>(&header), sizeof(Bitmap::Header));
	pic.read(reinterpret_cast<char*>(&info), sizeof(Bitmap::Info));
	
	h = info.height;
	w = info.width; 
}*/