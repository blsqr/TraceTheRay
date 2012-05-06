//Constructor
Camera::Camera (Vector t_cPos		= origin, 
				Vector t_cDir		= baseVx, 
				precs t_cAngle		= 0, 
				short t_vWidth		= 800, 
				short t_vHeight		= 600, 
				precs t_vAngle		= 46.8) {
	
	this->setCamera(t_cPos, t_cDir, t_cAngle, false);
	this->setView(t_vWidth, t_vHeight, t_vAngle, false);
	
	this->applyVC();
}



//Setters
void Camera::setCamera(Vector cP, Vector cD, precs cA, bool apply = true) {
	this->cPos		= cP;
	this->cDir		= cD;
	this->cAngle	= cA;
	
	this->cDir.normalize();
	
	if (apply) this->applyVC();
}
void Camera::setView(short vW, short vH, precs vA, bool apply = true) {
	this->vWidth	= vW;
	this->vHeight	= vH;
	this->vAngle	= vA;
	
	if (apply) this->applyVC();
}
void Camera::setVPos(Coord vP) {
	this->vPos	= vP;
}
void Camera::resetVPos() {
	this->vPos	= 0;
}
void Camera::setVDivNo(short no) {
	this->vDivNo	= no;
}



//Getters
Vector Camera::getCPos() {
	return this->cPos;
}
Vector Camera::getCDir() {
	return this->cDir;
}
precs Camera::getCAngle() {
	return this->cAngle;
}
short Camera::getVWidth() {
	return this->vWidth;
}
short Camera::getVHeight() {
	return this->vHeight;
}
precs Camera::getVAngle() {
	return this->vAngle;
}
Coord Camera::getVPos() {
	return this->vPos;
}
short Camera::getVDivNo() {
	return this->vDivNo;
}


//General methods
bool Camera::nextPixel() {
	Coord temp	= getVPos();
	
	//increment coordinates
	if ((temp.x + 1) == this->vWidth) {
		//cout << "Finished line " << temp.y << endl;
		temp.y = (temp.y + 1) % (this->vHeight);	//next y-pixel
	}
	temp.x = (temp.x + 1) % (this->vWidth);			//next x-pixel
	
	this->setVPos(temp);
	
	if (temp == nullCoord) return true;		//is at (0,0) again
	else return false;						//is not at the end yet
}

/*//move within divisons and return true, if at the end (top right) of one division)
bool Camera::nextPixel() {
	Coord temp		= getVPos();
	short divNo		= getVDivNo();
	
	short xDivs		= ceil(this->vWidth/divSize);			//divs in one row
	short yDivs		= ceil(this->vHeight/divSize);			//divs in one column
	
	//increment coordinates
	if ((temp.x + 1) == xDivEnd || (temp.x + 1) == this->vWidth) {
		//cout << "Finished line/div " << temp.y << endl;
		temp.y = (temp.y + 1) % (this->vHeight);	//next y-pixel
	}
	
	
	temp.x++;		//increment x
	if (temp.x % divSize == 0 || temp.x == this->vWidth) {
		temp.x = divNo*divSize;					//set x to beginning of div
		temp.y = (temp.y + 1) % this->vHeight;	//increment y
	}
	
	if (temp.x == divNO*divSize;
	this->setVPos(temp);
	
	if (temp == nullCoord) return true;		//is at (0,0) again
	else return false;						//is not at the end yet
}*/

bool Camera::nextDiv() {
	this->setVDivNo((this->getVDivNo() + 1)%divSize);
	return (this->getVDivNo() == 0);		//returns true, if at the end
}





bool Camera::applyVC() {
	/*
	 Vector vDiag;						//the rising diagonal within the view
	 Vector vX, vY;						//the vectors along the sides (x goes to the right, y to the top)
	 Vector vBL;						//vector to the bottom left, starting point
	*/
	
	//temporary variables
	Vector tCDir	= this->getCDir();
	short tVW		= this->getVWidth();
	short tVH		= this->getVHeight();
	precs tRatio	= (float) tVW/tVH;
	precs tVA		= this->getVAngle();
	Coord temp;
	
	//calculate vX: drop z-coordinate of cDir --> parallel to xy-plane. rotate 90° ccw around z-axis --> vX _|_ cDir
	this->vX		= tCDir;
	if (this->vX.getEle(0) != 0 || this->vX.getEle(1) != 0) {
		this->vX.setEle(2, 0);
		this->vX.rotZ(90);
	}
	else if (this->vX.getEle(2) != 0) {
		this->vX.rotX(90);
	}
	else {
		cout << "cDir not set!" << endl;
	}
	this->vX.rotate(tCDir, this->cAngle);
	
	//check vX * cDir = 0, rounded to 14 decimal places
	if (roundf((this->vX * this->getCDir())) != 0) {
		cout << "(vX * cDir) = " << (this->vX * this->getCDir()) << endl;
	}
	
	
	//calculate vY = cDir cross vX. ( / is crossprod operator!)
	this->vY		= tCDir / this->vX ;
	
	//calculate lengths of vX and vY: |vX| = 2tan(vAngle/2) / sqrt(1 + (ratio)^-2) ### |vY| = |vX|/ratio
	this->vX		= this->vX * (2*tan((float) tVA*0.5) * (1/sqrt(1 + 1/(tRatio*tRatio))));
	this->vY		= this->vY * (this->vX.getLength() / tRatio);
	
	this->vdX		= this->vX * ((float) 1/tVW);
	this->vdY		= this->vY * ((float) 1/tVH);
	
	//add vX to vY to get vDiag
	this->vDiag		= this->vX + this->vY;
	
	//calculate bottom left vector, the vector the rendering starts from, pointing to the center of pixel (0,0)
	this->vBL		= tCDir - (this->vDiag * 0.5) + (this->vdX * 0.5) + (this->vdY * 0.5);
	
	
	//calculate number of max divisions
	this->vDivMax	= ceil((tVW*tVH)/(divSize*divSize));
	this->vDivNo	= 0;
	
	return true;
}

void Camera::generateRay(bool first = false) {
	if (first) this->vRay.setPos(this->cPos);
	this->vRay.setDir(this->vBL + this->vdX * this->vPos.x + this->vdY * this->vPos.y);
}

PixelMap * Camera::render(short aa=1) {
	//local variables
	int tVW				= aa*this->getVWidth();
	int tVH				= aa*this->getVHeight();
	Polygon * tracedPg;
	Color tracedColor;
	bool end;
	
	//Initialize PixelMap with given dimensions
	PixelMap * render	= new PixelMap ("render", tVW, tVH);
	
	//apply the new vWidth and Height to Camera
	if (aa != 1) {
		this->setView(tVW, tVH, this->getVAngle(), true);
	}
	
	//reset vPos in Camera and pencilPos in PixelMap
	resetVPos();
	render->setPencilToOrigin();
	
	//generate first ray
	generateRay(true);
	
	while (true) {
		//generate ray for new vPos
		generateRay();
		
		//RayTracing. Returns pointer to the valid polygon
		tracedPg	= this->vRay.trace();
		
		//check for intersection. No intersection: set pixel to backgroundcolor
		if (tracedPg->isActive()) {
			tracedColor	= tracedPg->getColor();
		}
		else {
			tracedColor = black;
		}
		
		//set pixel in PixelMap to color of polygon and move to next pixel (2nd arg TRUE) in the PixelMap
		render->setPixel(tracedColor, true);
		
		//move to the next pixel in vPos. End loop if vPos reached top right
		end		= nextPixel();
		if (end) break;
	}
	
	
	//apply AntiAliasing
	*render	= render->antiAliase(aa);
	
	cout << "Finished rendering." << endl;
	
	//reset vWidth and Height to old values
	if (aa != 1) {
		this->setView(tVW/aa, tVH/aa, this->getVAngle(), true);
	}
	
	return render;
}



void Camera::print(string text="") {
	cout << endl;
	
	if (text != "") {
		cout << "Camera " << text;
	}
	else cout << "Camera:";
	
	
	cout << endl;
	cout << "\tcPos:  \t\t"; this->getCPos().print();
	cout << "\tcDir:  \t\t"; this->getCDir().print();
	cout << "\tcAngle:\t\t" << this->getCAngle() << endl;
	cout << "\tvW x vH:\t" << this->getVWidth() << " x " << this->getVHeight() << " \t(" << ((float) this->getVWidth()/this->getVHeight()) << " : 1)" << endl;
	cout << "\tvAngle:\t\t" << this->getVAngle() << endl;
	cout << "\tvX:    \t\t"; this->vX.print();
	cout << "\tvY:    \t\t"; this->vY.print();
	cout << "\tvDiag: \t\t"; this->vDiag.print();
	cout << "\tvBL:   \t\t"; this->vBL.print();
	cout << endl;
	
}