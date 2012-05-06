//Constructors
Vector::Vector() {
	ele[0] = 0;
	ele[1] = 0;
	ele[2] = 0;
}
Vector::Vector(precs x, precs y, precs z) {
	this->setEle(x,y,z);
}
Vector::Vector(precs temp [3]) {
	this->setEle(temp);
}


//Operators
Vector Vector::operator+ (Vector param) {
	Vector temp;
	for (int i = 0; i <= 2; i++) {
		temp.ele[i] = ele[i] + param.ele[i];
	}
    return (temp);
};
Vector Vector::operator- (Vector param) {
	Vector temp;
	for (int i = 0; i <= 2; i++) {
		temp.ele[i] = ele[i] - param.ele[i];
	}
    return (temp);
};
precs Vector::operator* (Vector param) {
    return scalarprod(*this, param);
}
Vector Vector::operator* (precs param) {
	Vector temp;
	for (int i = 0; i <= 2; i++) {
		temp.ele[i] = ele[i]*param;
	}
    return (temp);
}
Vector Vector::operator/ (Vector param) {
    Vector temp = crossprod(*this, param);
    return (temp);
};
bool Vector::operator== (Vector param) {
	for (int i = 0; i <= 2; i++) {
		if (ele[i] != param.ele[i]) return false;
	}
	return true;
};




//Set elements
void Vector::setEle(precs x, precs y, precs z) {
	this->ele[0] = x;
	this->ele[1] = y;
	this->ele[2] = z;
}
void Vector::setEle(precs temp [3]) {
	for (int i = 0; i <= 2; i++) {
		this->ele[i] = temp[i];
	}
}
void Vector::setEle(short coor, precs value) {
	this->ele[coor] = value;
}

//Normalize
void Vector::normalize() {
	*this = (*this)*(pow(this->getLength(),-1));
}

//Rotations
void Vector::rotate(Matrix rotation) {
	precs len1, len2;
	len1 = this->getLength();
	
	//Apply rotation matrix
	*this = rotation*(*this);
	
	//If necessary, adjust length to compensate the errors through bad precision
	len2 = this->getLength();
	if (fabs((len1/len2)-1) > 0.0000000000001) {
		*this = (*this)*(len1/len2);
		//cout << "corrected length" << len1/len2 << endl;
	}
}
void Vector::rotate(Vector axis, precs angle) {
	Matrix rotation(axis, angle);
	this->rotate(rotation);
}
void Vector::rotX(precs angle) {
	Vector axis(1,0,0);
	Matrix rotation(axis, angle);
	this->rotate(rotation);
}
void Vector::rotY(precs angle) {
	Vector axis(0,1,0);
	Matrix rotation(axis, angle);
	this->rotate(rotation);
	
}
void Vector::rotZ(precs angle) {
	Vector axis(0,0,1);
	Matrix rotation(axis, angle);
	this->rotate(rotation);
}



//General functions
precs Vector::scalarprod(Vector a, Vector b) {
	precs temp = a.ele[0]*b.ele[0] + a.ele[1]*b.ele[1] + a.ele[2]*b.ele[2];
    return (temp);
};

Vector Vector::crossprod(Vector a, Vector b) {
    Vector temp (a.ele[1]*b.ele[2] - a.ele[2]*b.ele[1],
				 a.ele[2]*b.ele[0] - a.ele[0]*b.ele[2],
				 a.ele[0]*b.ele[1] - a.ele[1]*b.ele[0]);
    return temp;
}

precs Vector::angleTo(Vector a) {
	return acos(((*this)*a)/(this->getLength()*a.getLength()));
}

//checks, whether two vectors p1, p2 are at the same side of a line between vectors a and b
bool Vector::isSameSide(Vector p1, Vector p2, Vector a, Vector b) {
	Vector cp1 = crossprod(b-a, p1-a);
	Vector cp2 = crossprod(b-a, p2-a);
	if ((cp1*cp2) >= 0) return true;
	else return false;
}

//checks, whether a point p lies within a polygon pg
bool Vector::isInPolygon(Polygon pg) {
	Vector a = pg.getEle(0);
	Vector b = pg.getEle(1);
	Vector c = pg.getEle(2);
	Vector p = *this;
	
	if (isSameSide(p, a, b, c) && isSameSide(p, b, a, c) && isSameSide(p, c, a, b)) return true;
	else return false;
}


//check, whether a point p lies within a body created by four rays a1,...,a4 (starting from pos)
//IMPORTANT: a1, a2, a3, a4 must be given in ccw direction 
bool Vector::isInCone(Vector pos, Vector a1, Vector a2, Vector a3, Vector a4) {
	Vector ray[4] = {a1, a2, a3, a4};
	for (int i=0; i<4; i++) {
		if ((crossprod(ray[(i+1)%4],ray[i])).angleTo(*this-pos) > M_PI/2) return false;
	}
	return true;
}




//Get Length
precs Vector::getLength() {
    return sqrt(scalarprod(*this, *this));
};


//Get elements
precs Vector::getEle(short coor) {
    return this->ele[coor];
};
precs Vector::getX() {
	return this->getEle(0);
}
precs Vector::getY() {
	return this->getEle(1);
}
precs Vector::getZ() {
	return this->getEle(2);
}



void Vector::print(string text) {
	if (text != "") {
		cout << endl << text << ": \t";
	}
	
	for (int i = 0; i <= 2; i++) {
		cout << ele[i];
		if (i < 2) {
			cout << ", \t";
		}
	}
    cout << endl;
}


