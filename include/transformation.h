#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "vector3D.h"
#include <cmath>

class transformation{


    unsigned int rows;
    unsigned int cols;

    double transVector[3];
    double rotMatrix[3][3];

public:
    double matrix[4][4];

public:
    transformation();
    void Initialize();
    void Identity( double matrix[4][4] );

    void Multiply( double AA[][4], double BB[][4], double CC[][4] );

    void Translate(vector3d traslation);

    void RotateY( double radAngle, bool stdOrder = true );


};

#endif // TRANSFORMATION_H
