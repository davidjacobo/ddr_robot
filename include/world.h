
#ifndef WORLD_H
#define WORLD_H

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <string>

#include "vector3D.h"
#include "objloader.h"

class world{


    public:
        objloader obj;
        int obj_bunny;
        int obj_table;

        int obj_object;
        int obj_world;

    public:
        world();

        void initObjects(void);

        void drawWorld();

};

#endif

