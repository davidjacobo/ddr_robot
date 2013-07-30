
#include "world.h"

world::world()
{
}



void world::initObjects(void)
{
     //obj_bunny = obj.load("bunny_low.obj");
     //obj_table = obj.load("table.obj");

     //obj_object = obj.load("object.obj");
     obj_world = obj.load("room2.obj");
}



void world::drawWorld(void)
{

//    //floor
//	glDisable(GL_LIGHTING);
//	glBegin(GL_QUADS);
//        glColor3f(0.8,0.49,0.19);
//		glVertex3f( 350, -.55,  200);
//		glVertex3f(-350, -.55,  200);
//		glVertex3f(-350, -.55, -200);
//		glVertex3f( 350, -.55, -200);
//	glEnd();
//	glEnable(GL_LIGHTING);


//    //Table
//    glPushMatrix();
//        glTranslatef(0.0f,-.2f,0.0f);
//        glCallList(obj_table);
//    glPopMatrix();

//    //Bunny
//    glCallList(obj_bunny);

    //World
    glCallList(obj_world);

    //Cylinder
    //glCallList(obj_object);
}
