#include <iostream>
#include <math.h>
#include <string.h>

class Matrix {
private:
    precs ele[3][3];    //Elements of the matrix, element in row i and column j is ele[i][j]; i,j = 0,1,2
	
public:
    
    //constructors / destructors
    Matrix ();
    Matrix (precs temp[3][3]);
    Matrix (precs, precs, precs);	//diagonal matrix
    Matrix (precs, precs, precs, precs, precs, precs, precs, precs, precs);
	Matrix (Vector, Vector, Vector);//vectors as columns
	Matrix (Vector, precs);			//rotation matrix around Vector with angle precs
    
    ~Matrix () {};
    
    //Operators
    Matrix operator+ (Matrix);
    Matrix operator- (Matrix);
    Matrix operator* (Matrix);
	
    Vector operator* (Vector);
    Matrix operator* (precs);
    Matrix operator*= (precs);
    
    //Functions
	precs getEle(short, short);
    Vector getRow(short);
    Vector getColumn(short);
	
	void setEle(short, short, precs);
	
    precs getDet();
    precs getTrace();
	
    void transpose();
    void adjugate();
    void invert();
	
    
    void print(string = "");
};