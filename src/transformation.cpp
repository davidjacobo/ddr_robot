#include "transformation.h"

transformation::transformation()
{
    rows = 4;
    cols = 4;
    Initialize();
}

void transformation::Initialize(void)
{

    Identity(matrix);

}

//   | 1 0 0 0 |
//   | 0 1 0 0 |
//   | 0 0 1 0 |
//   | 0 0 0 1 |
void transformation::Identity(double matrix[4][4])
{

    for(int r=0; r<4; ++r)
        for(int c=0; c<4; ++c)
            matrix[r][c] = ( c == r) ? 1.0 : 0.0;

}

// Multiplies matrices in the given order (AA = BB * CC)
void transformation::Multiply( double AA[][4], double BB[][4], double CC[][4] )
{
    double TT[4][4];
    for( int r = 0; r < 4; r++ )
        for( int c = 0; c < 4; c++ )
        {
            double sum = 0.0;
            for( int i = 0; i < 4; i++ )
                sum+= BB[i][r] * CC[c][i];
            TT[c][r] = sum;
        }
    for( int r = 0; r < 4; r++ )
        for( int c = 0; c < 4; c++ )
            AA[c][r] = TT[c][r];
}


//   |        a                  b                 c        d |
//   |        e                  f                 g        h |
//   |        i                  j                 k        l |
//   | m + transform.x   n + transform.y   ñ + transform.z  o |
void transformation::Translate(vector3d traslation)
{


     matrix[3][0] += traslation.x;
     matrix[3][1] += traslation.y;
     matrix[3][2] += traslation.z;

}

// Rotates around Y by the given angle
void transformation::RotateY( double radAngle, bool stdOrder)
{
    // Construct rotation matrix
    double rotMatrix[4][4];
    Identity( rotMatrix );
    rotMatrix[0][0] = cos( radAngle );
    rotMatrix[2][2] = rotMatrix[0][0];
    rotMatrix[2][0] = sin( radAngle );
    rotMatrix[0][2] = - rotMatrix[2][0];
    // Multiply
    // Multiply( this->matrix, rotMatrix, this->matrix );
    if( stdOrder )
        Multiply( matrix, rotMatrix, matrix );
    else
        Multiply( matrix, matrix, rotMatrix );
}
