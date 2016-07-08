//Constructor
Camera::Camera (Vector t_cPos		= origin, 
				Vector t_cDir		= baseVx, 
				precs t_cAngle		= 0, 
				short t_vWidth		= 800, 
				short t_vHeight		= 600, 
				precs t_vAngle		= 46.8,
				short t_vDivSize	= 1) {
	
	this->setCamera(t_cPos, t_cDir, t_cAngle, false);
	this->setView(t_vWidth, t_vHeight, t_vAngle, false);
	this->setDivSize(t_vDivSize);
	this->setDivNo(0);
	this->checkDiv = true;
	
	//check divisions
	if (t_vDivSize != 1 && (t_vWidth % t_vDivSize != 0 || t_vHeight % t_vDivSize != 0)) {
		cout << "Width or height don't match divSize! There might be errors in rendering. divSize = " << t_vDivSize << endl;
	}
	
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
void Camera::setDivSize(short size) {
	this->vDivSize	= size;
}
void Camera::setDivNo(unsigned int no) {
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
short Camera::getDivSize() {
	return this->vDivSize;
}
short Camera::getDivNo() {
	return this->vDivNo;
}

//General methods

//move within divisons and return true, if at the end
bool Camera::nextPixel() {
	Coord temp			= this->getVPos();
	short tWidth		= this->getVWidth();
	short tHeight		= this->getVHeight();
	short tDivSize		= this->getDivSize();
	
	if (this->vDivSize == 1) {
		//normal method, moving from pixel to pixel
		
		//increment coordinates
		if ((temp.x + 1) == this->getVWidth()) {
			//cout << "Finished line " << temp.y << endl;
			temp.y = (temp.y + 1) % (this->getVHeight());	//next y-pixel
		}
		temp.x = (temp.x + 1) % (this->getVWidth());			//next x-pixel
	}
	else {
		//using divisions, moving within divisions
		
		//move to next pixel normally
		temp.x++;
		
		//if not within the same division, move back divSize pixel and up 1 pixel
		if (temp.x % tDivSize == 0) {
			temp.x -= tDivSize;
			
			if ((temp.y + 1) % tDivSize == 0) {
				//next divison
				this->nextDiv();
				
				//calculate bottom left of new division
				temp.x	= this->getDivNo() % ((unsigned int) ceil(tWidth/tDivSize)) * tDivSize;
				temp.y	= floor(this->getDivNo() / ceil(tWidth/tDivSize)) * tDivSize;
			}
			else temp.y++;
		}
		//if at the end of the line, move back width % divSize pixel and up 1 pixel
		else if (temp.x == tWidth) {
			temp.x -= tWidth % tDivSize;
			
			if ((temp.y + 1) % tDivSize == 0) {
				//next divison
				this->nextDiv();
				
				//calculate bottom left of new division
				temp.x	= this->getDivNo() % ((unsigned int) ceil(tWidth/tDivSize)) * tDivSize;
				temp.y	= floor(this->getDivNo() / ceil(tWidth/tDivSize)) * tDivSize;
			}
			else if ((temp.y == tHeight)) {
				//arrived at top right
				temp = nullCoord;
			}
			else temp.y++;
		}
		//make sure it does not get out of vPos
		temp.x = temp.x % tWidth;
		temp.y = temp.y % tHeight;
	}
	//apply
	this->setVPos(temp);
	
	if (temp == nullCoord) return true;		//is at (0,0) again
	else return false;						//is not at the end yet	
}

void Camera::nextDiv() {
	this->setDivNo((this->getDivNo() + 1));
	this->checkDiv = true;
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
	precs tVA		= this->getVAngle() * 2*M_PI/360;
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
	
	return true;
}




void Camera::generateRay(bool first = false) {
	if (first) this->vRay.setPos(this->cPos);
	this->vRay.setDir(this->vBL + this->vdX * this->vPos.x + this->vdY * this->vPos.y);
}



void Camera::checkDivPgs() {
	Polygon * tempPg;
	Ray tempRay (this->getCPos(), baseVx);
	short pgIndex;
	short tX, tY;
	
	//create vectors to check, whether the points of the polygons are within the cone created by those vectors
	Vector a1, a2, a3, a4;		//bottom left, bottom right, top right, top left (ccw)
	a1	= this->vBL + this->vdX * this->vPos.x + this->vdY * this->vPos.y;
	a2	= this->vBL + this->vdX * (this->vPos.x + this->getDivSize()) + this->vdY * this->vPos.y;
	a3	= this->vBL + this->vdX * (this->vPos.x + this->getDivSize()) + this->vdY * (this->vPos.y + this->getDivSize());
	a4	= this->vBL + this->vdX * this->vPos.x + this->vdY * (this->vPos.y + this->getDivSize());
	
	/*a1.print("a1");
	a2.print("a2");
	a3.print("a3");
	a4.print("a4");*/
	
	//empty divPolygons
	this->inDivPolygons.clear();
	this->outDivPolygons	= allPolygons;
	
	//loop through allPolygons
	for (int i=0; i < allPolygons.size(); i++) {
		tempPg			= allPolygons[i];
		
		if (tempPg->isInCone(this->getCPos(), a4, a3, a2, a1)) {
			//add to divPolygons
			this->inDivPolygons.push_back(tempPg);
			
			//remove from outDivPolygons
			this->outDivPolygons.erase(find(outDivPolygons.begin(), outDivPolygons.end(), tempPg));
		}
	}
	
	//generate rays along the sides of the cone and trace them
	for (int j=0; j < (4 * this->getDivSize() - 3); j++) {
		
		//tracedPg	= this->vRay.trace(&(this->inDivPolygons));
		
	}
	
	this->checkDiv	= false;
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
	
	//reset vPos in Camera
	resetVPos();
	
	//generate first ray
	generateRay(true);
	
	cout << "\nRendering initialized.\n\tResolution: \t" << tVW/aa << "x" << tVH/aa << " at " << aa << "xAA\n\tPolygons: \t" << allPolygons.size() << endl;
	
	while (true) {
		//generate ray for new vPos
		generateRay();
		
		//start of new position --> update vector inDivPolygons to pass to trace()
		if (this->checkDiv) this->checkDivPgs();
		
		//RayTracing. Returns pointer to the valid polygon from a given vector including pointers to polygons in that division
		if (this->getDivSize() != 1) {
			tracedPg	= this->vRay.trace(&(this->inDivPolygons));
		}
		else {
			tracedPg	= this->vRay.trace();	//use all polygons
		}
		
		//check for intersection. No intersection: set pixel to backgroundcolor
		if (tracedPg->isActive()) {
			tracedColor	= tracedPg->getColor();
		}
		else {
			tracedColor = black;
		}
		
		//set pixel in PixelMap to color of polygon and move to next pixel (2nd arg TRUE) in the PixelMap
		render->setPixel(tracedColor, this->getVPos());
		
		//move to the next pixel in vPos. End loop if vPos reached top right
		end		= nextPixel();
		if (end) break;
	}
	
	
	//apply AntiAliasing
	*render	= render->antiAliase(aa);
	
	cout << "Rendering finished.\n" << endl;
	
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