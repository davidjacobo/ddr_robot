
#include "camera.h"

camera::camera()
{
    //loc.x=3.5;
    loc.y=2.5;
    //loc.z=1.0;
    loc.x = -2.5;
    loc.z = -1.0;

    camYaw=240.0;
    camPitch=-20.0;
    speed=0.02;
    mousespeed=0.1;
    mi=false;
}
camera::camera(const vector3d& l)
{
    loc=l;
    camYaw=0.0;
    camPitch=0.0;
    speed=0.02;
    mousespeed=0.1;
    mi=false;
}
camera::camera(const vector3d& l,const float& yaw,const float& pitch)
{
    loc=l;
    camYaw=yaw;
    camPitch=pitch;
    speed=0.02;
    mousespeed=0.1;
    mi=false;
}
camera::camera(const vector3d& l,const float& yaw,const float& pitch,const float& sp,const float& ms)
{
    loc=l;
    camYaw=yaw;
    camPitch=pitch;
    speed=sp;
    mousespeed=ms;
    mi=false;
}


void camera::lockCamera()
{
	if(camPitch>90)
		camPitch=90;
	if(camPitch<-90)
		camPitch=-90;
	if(camYaw<0.0)
		camYaw+=360.0;
	if(camYaw>360.0)
		camYaw-=360;
}

void camera::moveCamera(const float &dir)
{
	float rad=(camYaw+dir)*M_PI/180.0;
	loc.x-=sin(rad)*speed;
	loc.z-=cos(rad)*speed;
}

void camera::moveCameraUp(const float& dir)
{
	float rad=(camPitch+dir)*M_PI/180.0;
	loc.y+=sin(rad)*speed;
}

void camera::control()
{
	if(mi)
	{

		int MidX=400;
		int MidY=300;
		SDL_ShowCursor(SDL_DISABLE);
		int tmpx,tmpy;
		SDL_GetMouseState(&tmpx,&tmpy);
		camYaw+=mousespeed*(MidX-tmpx);
		camPitch+=mousespeed*(MidY-tmpy);
		lockCamera();
		SDL_WarpMouse(MidX,MidY);
		Uint8* state=SDL_GetKeyState(NULL);
		if(state[SDLK_w])
		{
				//moveCamera(0.0);

            if(camPitch!=90 && camPitch!=-90)       //if we are facing directly up or down, we don't go forward, it will be commented out, when there will be gravity
                moveCamera(0.0);        //move forward
            moveCameraUp(0.0);      //move up/down

		}else if(state[SDLK_s])
		{
				//moveCamera(180.0);
            if(camPitch!=90 && camPitch!=-90)
                moveCamera(180.0);
            moveCameraUp(180.0);

		}
		if(state[SDLK_a])
			moveCamera(90.0);
		else if(state[SDLK_d])
			moveCamera(270);
	}

	glRotatef(-camPitch,1.0,0.0,0.0);
	glRotatef(-camYaw,0.0,1.0,0.0);
}

void camera::update()
{
	glTranslatef(-loc.x,-loc.y,-loc.z);
}

vector3d camera::getLocation()
{
	return loc;
}

float camera::getYaw()
{
	return camYaw;
}
float camera::getPitch()
{
	return camPitch;
}

void camera::setLocation(const vector3d& newcoord)
{
	loc=newcoord;
}

void camera::lookAt(const float& y, const float& p)
{
	camYaw=y;
	camPitch=p;
}

void camera::mouseIn()
{
	mi=true;
}

void camera::mouseOut()
{
	mi=false;
}
//change thespherical coordiante system to cartesian
vector3d camera::getVector()
{
//	std::cout << camYaw << " " << camPitch << std::endl;
	return (vector3d(-cos(camPitch*M_PI/180.0)*sin(camYaw*M_PI/180.0),sin(camPitch*M_PI/180.0),-cos(camPitch*M_PI/180.0)*cos(camYaw*M_PI/180.0)));
}

bool camera::IsMouseIn()
{
	return mi;
}


void camera::setSpeed(float sp,float ms)
{
	speed=sp;
	mousespeed=ms;
}
