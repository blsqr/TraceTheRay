//Constructors
Matrix::Matrix () {
	for (int i = 0; i <= 2; i++) {
		for (int j = 0; j <= 2; j++) {
			ele[i][j] = 0;
		}
	}
};
Matrix::Matrix (precs temp[3][3]) {
	for (int i = 0; i <= 2; i++) {
		for (int j = 0; j <= 2; j++) {
			ele[i][j] = temp[i][j];
		}
	}
};
Matrix::Matrix (precs temp_aa, precs temp_bb, precs temp_cc) {
	precs temp[3] = {temp_aa, temp_bb, temp_cc};
	
	for (int i = 0; i <= 2; i++) {
		for (int j = 0; j <= 2; j++) {
			if (i == j)
				ele[i][j] = temp[i];
			else
				ele[i][j] = 0;
		}
	}
}
Matrix::Matrix (precs aa, precs ab, precs ac, precs ba, precs bb, precs bc, precs ca, precs cb, precs cc) {
	precs temp[9] = {aa, ab, ac, ba, bb, bc, ca, cb, cc};
	
	for (int i = 0; i <= 2; i++) {
		for (int j = 0; j <= 2; j++) {
			ele[i][j] = temp[(3*i)+j];
		}
	}
}
Matrix::Matrix (Vector a, Vector b, Vector c) {
	Vector temp[3] = {a, b, c};
	
	for (int i = 0; i <= 2; i++) {
		for (int j = 0; j <= 2; j++) {
			ele[i][j] = temp[j].getEle(i);
		}
	}
}
Matrix::Matrix (Vector axis, precs angle) {
	//Rotation matrix
	
	//Normalize Vector first
	if (axis.getLength() != 1) axis.normalize();
	
	//convert angle to radiants
	angle *= 2*M_PI/360;
	
	//create elements of matrix
	ele[0][0]	= cos(angle) + pow(axis.getX(), 2)*(1-cos(angle));
	ele[0][1]	= axis.getX()*axis.getY()*(1-cos(angle))-axis.getZ()*sin(angle);
	ele[0][2]	= axis.getX()*axis.getZ()*(1-cos(angle))+axis.getY()*sin(angle);
	ele[1][0]	= axis.getX()*axis.getY()*(1-cos(angle))+axis.getZ()*sin(angle);
	ele[1][1]	= cos(angle) + pow(axis.getY(), 2)*(1-cos(angle));
	ele[1][2]	= axis.getY()*axis.getZ()*(1-cos(angle))-axis.getX()*sin(angle);
	ele[2][0]	= axis.getX()*axis.getZ()*(1-cos(angle))-axis.getY()*sin(angle);
	ele[2][1]	= axis.getY()*axis.getZ()*(1-cos(angle))+axis.getX()*sin(angle);
	ele[2][2]	= cos(angle) + pow(axis.getZ(), 2)*(1-cos(angle));
}


//Operators
Matrix Matrix::operator+ (Matrix param) {
	for (int i = 0; i <= 2; i++) {
		for (int j = 0; j <= 2; j++) {
			param.ele[i][j] += ele[i][j];
		}
	}
    return (param);
};
Matrix Matrix::operator- (Matrix param) {
	for (int i = 0; i <= 2; i++) {
		for (int j = 0; j <= 2; j++) {
			param.ele[i][j] -= ele[i][j];
		}
	}
    return (param);
};
Matrix Matrix::operator* (Matrix param) {
	Matrix temp;
	for (int i = 0; i <= 2; i++) {
		for (int j = 0; j <= 2; j++) {
			temp.ele[i][j] = (this->getRow(i))*(param.getColumn(j));
		}
	}
    return (temp);
};

Vector Matrix::operator* (Vector param) {
	Vector temp;
	for (int i = 0; i <= 2; i++) {
		temp.setEle(i, (this->getRow(i))*(param));
	}
	return (temp);
};

Matrix Matrix::operator* (precs param) {
	Matrix temp;
	for (int i = 0; i <= 2; i++) {
		for (int j = 0; j <= 2; j++) {
			temp.ele[i][j] = this->ele[i][j]*param;
		}
	}
    return (temp);
};
Matrix Matrix::operator*= (precs param) {
	Matrix temp;
	temp = temp*param;
    return (temp);
};





//Getters
precs Matrix::getEle(short row, short column) {
	return this->ele[row][column];
}
Vector Matrix::getRow(short row) {
	Vector temp(this->ele[row][0], this->ele[row][1], this->ele[row][2]);
	return (temp);
}
Vector Matrix::getColumn(short column) {
	Vector temp(this->ele[0][column], this->ele[1][column], this->ele[2][column]);
	return (temp);
}

precs Matrix::getDet() {
	precs temp;
	temp	+= this->ele[0][0]*this->ele[1][1]*this->ele[2][2];
	temp	+= this->ele[0][1]*this->ele[1][2]*this->ele[2][0];
	temp	+= this->ele[0][2]*this->ele[1][0]*this->ele[2][1];
	temp	-= this->ele[2][0]*this->ele[1][1]*this->ele[0][2];
	temp	-= this->ele[2][1]*this->ele[1][2]*this->ele[0][0];
	temp	-= this->ele[2][2]*this->ele[1][0]*this->ele[0][1];
	
	return temp;
}

precs Matrix::getTrace() {
	precs temp;
	for (int i = 0; i <= 2; i++) {
		temp += this->ele[i][i];
	}
	return (temp);
}

//setters
void Matrix::setEle(short row, short column, precs value) {
	this->ele[row][column] = value;
}


//operations on the matrix
void Matrix::transpose() {
	Matrix temp = *this;
	
	//Build temporary matrix
	for (int i = 0; i <= 2; i++) {
		for (int j = 0; j <= 2; j++) {
			temp.ele[i][j] = this->ele[j][i];
		}
	}
	
	*this = temp;
}

void Matrix::adjugate() {
	Matrix temp;
	int factor;			//factor (-1) or (1)
	precs subDet;		//determinant of submatrix;
	precs subMat[4];	//Array holding the submatrix
	int m;				//Counting how many values were entered into the submatrix
	
	//Build temporary matrix
	for (int i = 0; i <= 2; i++) {
		for (int j = 0; j <= 2; j++) {
			factor	= pow((-1),(i+j));
			m		= 0;
			
			//Get submatrix
			for (int k = 0; k <= 2; k++) {
				for (int l = 0; l <= 2; l++) {
					if ((k != i) && (l != j)) {
						subMat[m] = this->ele[k][l];
						m++;
					}
				}
			}
			
			subDet			= subMat[0]*subMat[3]-subMat[2]*subMat[1];
			temp.ele[j][i]	= factor*subDet;
		}
	}
	
	*this = temp;
}

void Matrix::invert() {
	precs det = this->getDet();
	Matrix temp;
	
	if (det != 0) {
		temp = *this;
		temp.adjugate();
		temp = temp * (1/det);
	}
	
	*this = temp;
}



void Matrix::print(string text) {
	if (text != "") {
		cout << endl << "Matrix " << text << ":" << endl;
	}
	
	for (int i=0; i<=2; i++) {
		for (int j=0; j<=2; j++) {
			if (ele[i][j] != -0) {
				cout << ele[i][j] << "\t\t";
			}
			else {
				cout << 0 << "\t\t";
			}
			
			if (j == 2) { cout << endl; } //Next row
		}
	}
	cout << endl;
};