#ifndef SENSORCAMERA_H
#define SENSORCAMERA_H

#include <SDL/SDL.h>


#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "vector3D.h"

#define ANG2RAD 3.14159265358979323846/180.0

class sensorCamera{

    //for shader
    unsigned int programID;


    float speed;

public:
    vector3d ntl,ntr,nbl,nbr,ftl,ftr,fbl,fbr;
    float nearD, farD, ratio, angle;
    float nw,nh,fw,fh;
    bool MONOCULAR;

    float camYaw;
    vector3d camPos;


public:
    sensorCamera();

    void initSensor(float _angle, float _ratio, float _nearD, float _farD, vector3d pos, float th,bool MONOCULAR);
    void draw(void);

    //For camera view

    void update();
    void setCamPos(double *p);
    void setAltura(double alt);
    void setCamAngle(float angle);


};

#endif // SENSORCAMERA_H
