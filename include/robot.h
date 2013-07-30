
#ifndef ROBOT_H
#define ROBOT_H

#include <GL/gl.h>
#include <GL/glu.h>

#include "vector3D.h"
#include "transformation.h"
#include "objloader.h"

#include "sensorCamera.h"


#ifndef M_PI
#define M_PI 3.1415926535
#endif
typedef struct
{
    vector3d pos;
    double th;
}configuration;




class robot{

    
    public:

        configuration q;
        int obj_model;
        objloader obj;
	bool MONOCULAR;
	double altura_camara;
              


        transformation transConfig;
        transformation rotConfig;

        sensorCamera sensor;
    
    private:

        float SaturateAngle(float angle);

    public:
        robot();

        void initRobot(bool,double);
        void drawRobot(void);
	void setRobot(double,double,double);

        void TranslateRobot(void);
        void RotateRobot(void);


        void GetState( double *state );
};

#endif
