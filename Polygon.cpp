//Constructor / destructor
Polygon::Polygon () {
	transparency	= 0;
	status			= 0;
	
	//No pointer is added to the list of polygons!
};

Polygon::Polygon (Vector ele0, Vector ele1, Vector ele2, Color color = black) {
	transparency = 0;
	
	if (!(ele0==ele1 && ele0==ele2 && ele1==ele2)) {
		this->setEle(ele0, ele1, ele2);
		this->setColor(color);
		status = 1;
		
		//Add pointer to this polygon to the dynamic array of pointers to all Polygons
		allPolygons.push_back(this);
	}
	else {
		status = 0;
		cout << "Polygon constructor: two or more equal vectors! Construction aborted" << endl;
	}
}

Polygon::~Polygon () {};

//Setters
bool Polygon::activate() {
	if (!this->status) {
		this->status = 1;
		
		//Add pointer to this polygon to the dynamic array of pointers to all Polygons
		allPolygons.push_back(this);
		
		return true;
	}
	else return false;
}

void Polygon::setEle(Vector ele0, Vector ele1, Vector ele2) {
	transparency = 0;
	
	this->ele[0] = ele0;
	this->ele[1] = ele1;
	this->ele[2] = ele2;
	
	//Calculate center
	this->center = (ele0+ele1+ele2)*(pow(3, -1));
	
	//Calculate normal and area (crossprod (/) of the sides touching vector 1)
	this->normal = (ele1-ele0)/(ele2-ele0);
	this->area = 0.5*(this->normal.getLength());	
	this->normal.normalize();
}

void Polygon::setColor(Color color) {
	this->color = color;
}


//Getters
Vector Polygon::getEle(int coor) {
	return this->ele[coor];
}
Color Polygon::getColor() {
	return this->color;
}
Vector Polygon::getCenter() {
	return this->center;
}
Vector Polygon::getNormal() {
	return this->normal;
}
precs Polygon::getArea() {
	return this->area;
}
bool Polygon::isActive() {
	return this->status;
}


//Translations
void Polygon::moveBy(Vector param) {
	this->setEle(
				 this->ele[0]+param,
				 this->ele[1]+param,
				 this->ele[2]+param);
}

void Polygon::moveTo(Vector param) {
	param = param-(this->center);
	this->moveBy(param);
}

//Rotations
void Polygon::rotateAbs(Vector axis, precs angle) {
	Vector temp[3];
	for (int i = 0; i <= 2; i++) {
		temp[i]	= this->ele[i];
		temp[i].rotate(axis, angle);
	}
	this->setEle(temp[0], temp[1], temp[2]);
}

void Polygon::rotateRel(Vector axis, precs angle) {
	Vector oldPosition	= this->getCenter();
	
	this->moveTo(origin);
	this->rotateAbs(axis, angle);
	this->moveTo(oldPosition);
}

//Scaling
void Polygon::scaleAbs(precs factor) {
	this->setEle(
				 this->ele[0]*factor,
				 this->ele[1]*factor,
				 this->ele[2]*factor);
}

void Polygon::scaleRel(precs factor) {
	Vector oldPosition	= this->getCenter();
	
	this->moveTo(origin);
	this->scaleAbs(factor);
	this->moveTo(oldPosition);
}



//Check whether the polygon is in a cone given by four rays emerging from pos
bool Polygon::isInCone(Vector pos, Vector a1, Vector a2, Vector a3, Vector a4) {
	//check points
	for (int i=0; i<3; i++) {
		if (this->getEle(i).isInCone(pos, a1, a2, a3, a4)) return true;
	}
	
	Vector * coneRays[4]	= {&a1, &a2, &a3, &a4};
	Ray temp (pos, a1);
	
	//check intersections
	for (int j=0; j<4; j++) {
		temp.setDir(*(coneRays[j]));
		if (temp.isIntersecting(this)) return true;
	}
	temp.setDir(a1+a2+a3+a4);
	if (temp.isIntersecting(this)) return true;
	
	return false;
}







void Polygon::print(string text) {
	cout << endl;
	
	if (text != "") {
		cout << "Polygon " << text;
	}
	else cout << "Polygon:";
	
	
	if (this->isActive()) {
		cout << endl;
		cout << "\tP1:\t"; this->getEle(0).print();
		cout << "\tP2:\t"; this->getEle(1).print();
		cout << "\tP3:\t"; this->getEle(2).print();
		cout << "\tCenter:\t"; this->getCenter().print();
		cout << "\tNormal:\t"; this->getNormal().print();
		cout << "\tArea:\t" << this->getArea() << endl;
		cout << endl;
	}
	else cout << " is inactive!" << endl;
}