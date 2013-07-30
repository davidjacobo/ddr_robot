
#ifndef SIMULATOR_H
#define SIMULATOR_H


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <cmath>
#define PI 3.14159265
#define EPS 0.0001
#define Xres 640
#define Yres 640
#define size_x 1024
#define size_y 768


#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

#include "camera.h"
#include "vector3D.h"
#include "world.h"
#include "robot.h"
#include "evader.h"

class simulator
{
    public:
    bool running;
    camera cam;
    world W;
    robot R,C;
    evader E,E1,E2;

	static const int FPS=20;

    public:
        simulator();
        ~simulator();

        void start();
        void show();
	void printer(cv::Mat&,int,int,int,int);

	void windows(void);

};

#endif
