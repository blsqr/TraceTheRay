Ray::Ray(Vector a=origin, Vector b=baseVx) {
	pos = a;
	dir = b;
}

void Ray::setPos(Vector temp) {
	this->pos = temp;
}
void Ray::setDir(Vector temp) {
	this->dir = temp;
}

Vector Ray::getPos() {
	return this->pos;
}
Vector Ray::getDir() {
	return this->dir;
}

Polygon * Ray::trace(vector<Polygon*> * thePolygons = &allPolygons) {
	Polygon * nearestPg = new Polygon();	//create nullPolygon (inactive)
	Polygon * tempPg;
	
	Matrix * paramMatrix;		//matrix from the dir-vector and two sides of the polygon creating its plane
	
	Vector params;				//parameters for the line equation (alpha) and the plane equation
	Vector intersection;		//the actual intersection
	
	precs shortestDis	= 0;
	precs tempDis		= 0;
	
	//loop through allPolygons
	for (int i=0; i < thePolygons->size(); i++) {
		//set pointer tempPg to the current polygon
		tempPg			= (*thePolygons)[i];
		
		//create matrix (dir, a-b, a-c)
		paramMatrix		= new Matrix (this->dir, tempPg->getEle(0) - tempPg->getEle(1), tempPg->getEle(0) - tempPg->getEle(2));
		
		//calculate parameters: (alpha, beta, gamma) = paramMatrix^-1 * (a - pos)
		paramMatrix->invert();
		params			= (*paramMatrix) * (tempPg->getEle(0) - this->pos);
		
		//check for intersection
		if (params.getEle(0) != 0) {			
			//Perform several checks:
			//check interception with plane. If it is behind the camera, alpha is smaller or equal to zero
			if (params.getEle(0) > 0) {
				//check whether the intersection is within the area of the polygon using the calculated parameters for the plane equation - that is faster than the method Vector::isInPolygon(Polygon) uses.
				if (params.getEle(1) > 0 && params.getEle(2) > 0 && (params.getEle(1) + params.getEle(2)) <= 1) {
					//now calculate the intersection using the ray equation and the ray parameter
					intersection	= (this->pos) + (this->dir) * params.getEle(0);
					//check distance to pos
					tempDis	= (intersection - (this->pos)).getLength();
					
					if (tempDis < shortestDis || shortestDis == 0) {
						shortestDis	= tempDis;
						
						//All checks successful. Set nearestPg to this polygon
						nearestPg	= tempPg;
						
						//intersection.print("New nearest intersection:");
						//cout << "Polygon " << i << ": now new nearest polygon with distance = " << shortestDis << endl;
					}
					//else cout << "Polygon " << i << ": " << (tempDis-shortestDis) << " farther away than nearest." << endl;
				}
				//else cout << "Polygon " << i << ": not within polygon. Plane parameters: (" << params.getEle(1) << ", " << params.getEle(2) << ")" << endl;
			}
			//else cout << "Polygon " << i << ": behind camera. Ray parameter: " << params.getEle(0) << endl;
		}
		//else cout << "Polygon " << i << ": no intersection." << endl;
	}
	return nearestPg;
}


//Similar to above function, but only returning boolean value for one polygon
bool Ray::isIntersecting(Polygon * tempPg) {
	Matrix * paramMatrix;
	
	Vector params;
	Vector intersection;
		
	//create matrix
	paramMatrix		= new Matrix (this->dir, tempPg->getEle(0) - tempPg->getEle(1), tempPg->getEle(0) - tempPg->getEle(2));
	
	//calculate parameters
	paramMatrix->invert();
	params			= (*paramMatrix) * (tempPg->getEle(0) - this->pos);
	
	//check for intersection
	if (params.getEle(0) != 0) {	
		//check whether it is behind the camera
		if (params.getEle(0) > 0) {
			//check whether the intersection is within the area of the polygon
			if (params.getEle(1) > 0 && params.getEle(2) > 0 && (params.getEle(1) + params.getEle(2)) <= 1) {
				return true;
			}
			//else not within polygon.
		}
		//else behind camera.
	}
	//else no intersection
	
	return false;
}



void Ray::print(string text="") {
	cout << "Ray";
	if (text != "") {
		cout << " " << text << ":" << endl;
	}
	this->pos.print("Pos");
	this->dir.print("Dir");
	
    cout << endl;
}