
#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <iostream>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "vector3D.h"

class camera
{
    
    vector3d loc;
    float camYaw;
    float camPitch;
    float speed;
    float mousespeed;
    bool mi;                    //indicates if the mouse is in the simulation screen

    void lockCamera();
    void moveCamera(const float& dir);
    void moveCameraUp(const float& dir);

    public:
        camera();
        camera(const vector3d& loc);
        camera(const vector3d& loc,const float& yaw,const float& pitch);
        camera(const vector3d& loc,const float& yaw,const float& pitch, const float& sp, const float& ms);
        void control();
        void update();
        vector3d getVector();
        vector3d getLocation();
        float getYaw();
        float getPitch();

        void setLocation(const vector3d& newcoord);
        void lookAt(const float& y, const float& p);
        void mouseIn();
        void mouseOut();
        bool IsMouseIn();
        void setSpeed(float sp, float ms);


};

#endif
