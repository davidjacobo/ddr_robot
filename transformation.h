

#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "vector3D.h"
#include <cmath>

class transform{

     public:
          //double matrix[16]; //matrix 4x4
          double matrix[4][4]; //matrix 4x4
          int rows;
          int cols;

     private:
          //double rotMatrix[9]; //matrix 3x3
          double rotMatrix[3][3];

     public:
          transform();
          ~transform();

          void Initialize( void );
          void ConstructDH( double aLength, double alphaTwist,
                    double dOffset, double thetaAngle );

          void Translate( vector3d traslation );
          //void RotateX( double radAngle);
          //void RotateY( double radAngle);
          //void RotateZ( double radAngle);

          void RotateX( double radAngle, bool stdOrder = true );
          void RotateY( double radAngle, bool stdOrder = true );
          void RotateZ( double radAngle, bool stdOrder = true );

          void Multiply( double AA[][4], double BB[][4], double CC[][4] );
          void Multiply( double matrix[4][4] );

          //void Identity( double matrix[16] );
          void Identity( double matrix[4][4] );

};

#endif
