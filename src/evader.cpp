
#include "evader.h"


//Class Evader
evader::evader()
{
    obj_model = NULL;
    transConfigE.Initialize();
}


void evader::initEvader()
{

    //Initial configuration
    q.pos.x = 0.0;
    q.pos.y = 0.0;
    q.pos.z = 0.0;
    q.th = 0.0;

    transConfigE.Translate(q.pos);
    obj_model=obj.load("evasor_final.obj");
}

void evader::setEvader(double x,double y)
{
    q.pos.x = x-q.pos.x;
    q.pos.z = y-q.pos.z;

    transConfigE.Translate(q.pos);

    q.pos.x = x;
    q.pos.z = y;
}


void evader::drawEvader(void)
{
    glPushMatrix();
     TranslateEvader();

     glMultMatrixd( (double *)transConfigE.matrix );

     glPushMatrix();
     glRotatef(90,0,1,0);
         glCallList(obj_model);
     glPopMatrix();

    glPopMatrix();
}

void evader::TranslateEvader(void)
{
	const double kDeltaPos = 0.01;
	vector3d dX( kDeltaPos, 0.0, 0.0 );
	vector3d dY( 0.0, kDeltaPos, 0.0 );
	vector3d dZ( 0.0, 0.0, kDeltaPos );

     Uint8* state=SDL_GetKeyState(NULL);

     if(state[SDLK_j])
          transConfigE.Translate(dX*(-1.0));

     if(state[SDLK_l])
          transConfigE.Translate(dX);

     if(state[SDLK_i])
          transConfigE.Translate(dZ*(-1.0));
     if(state[SDLK_k])
          transConfigE.Translate(dZ);

}
