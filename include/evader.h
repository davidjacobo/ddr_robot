
#ifndef EVADER_H
#define EVADER_H

#include <GL/gl.h>
#include <GL/glu.h>

#include "vector3D.h"
#include "transformation.h"
#include "objloader.h"

//#include "sensorCamera.h"


#ifndef M_PI
#define M_PI 3.1415926535
#endif
typedef struct
{
    vector3d pos;
    double th;
}configurationE;




class evader{

    
    public:

        configurationE q;
        int obj_model;
        objloader obj;
              


        transformation transConfigE;
        //transformation rotConfig;

        //sensorCamera sensor;
    
    private:

        //float SaturateAngle(float angle);

    public:
        evader();

        void initEvader(void);
        void drawEvader(void);
	void setEvader(double,double);

        void TranslateEvader(void);
        //void RotateEvader(void);


        //void GetState( double *state );
};

#endif
