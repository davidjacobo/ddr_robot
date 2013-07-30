
#include "robot.h"


//Class Robot
robot::robot()
{
    obj_model = NULL;
    transConfig.Initialize();
}


void robot::initRobot(bool _MONOCULAR,double alt_cam)
{
    //Initial configuration
    q.pos.x = 0.0;
    q.pos.y = 0.0;
    q.pos.z = 0.0;
    q.th = 0.0;
    MONOCULAR = _MONOCULAR;

    transConfig.Translate(q.pos);

    obj_model=obj.load("perseguidor_perspectivo.obj");


    float nearP = 0.05, farP = 10.0;
    float angle = 110.0, ratio=1.0;
    altura_camara = alt_cam;

    sensor.initSensor(angle,ratio,nearP,farP,q.pos,q.th,MONOCULAR);

}

void robot::setRobot(double x,double y,double phi3)
{
    q.pos.x = x-q.pos.x;
    q.pos.z = y-q.pos.z;
    transConfig.Translate(q.pos);
    q.pos.x = x;
    q.pos.z = y;

    double torot = phi3 - q.th;
    rotConfig.RotateY(torot);
    q.th = phi3;
}

void robot::drawRobot(void)
{
    glPushMatrix();

     glMultMatrixd( (double *)transConfig.matrix );   //IMPORTANTE: Aqui se aplica el estado del perseguidor
     rotConfig.RotateY(-M_PI/2);
     glMultMatrixd( (double *)rotConfig.matrix );
     rotConfig.RotateY(M_PI/2);


     glPushMatrix();
     glRotatef(90,0,1,0);
         glCallList(obj_model);
     glPopMatrix();

    if(MONOCULAR)
        sensor.draw();

    glPopMatrix();

    double state[3];
    GetState(state);

    if(MONOCULAR)
        state[1] += altura_camara;

    sensor.setCamPos(state);

    q.th += 3*M_PI/2;
    sensor.setCamAngle(q.th*180/M_PI);
    q.th -= 3*M_PI/2;

}

void robot::TranslateRobot(void)
{
	const double kDeltaPos = 0.01;
	vector3d dX( kDeltaPos, 0.0, 0.0 );
	vector3d dY( 0.0, kDeltaPos, 0.0 );
	vector3d dZ( 0.0, 0.0, kDeltaPos );

     Uint8* state=SDL_GetKeyState(NULL);

     if(state[SDLK_j])
          transConfig.Translate(dX*(-1.0));

     if(state[SDLK_l])
          transConfig.Translate(dX);

     if(state[SDLK_i])
          transConfig.Translate(dZ*(-1.0));
     if(state[SDLK_k])
          transConfig.Translate(dZ);

}

float robot::SaturateAngle(float angle)
{
    float retAngle = angle;
    // Negative limit
    if( retAngle < 0 )
        retAngle += ( 2.0 * M_PI );
    // Positive limit
    else if( retAngle >= ( 2.0 * M_PI ) )
        retAngle -= ( 2.0 * M_PI );
    return retAngle;

}

void robot::RotateRobot(void)
{
     const double kDeltaAng = M_PI / 32.0;

     Uint8* state=SDL_GetKeyState(NULL);

     //Rotate Base
     if(state[SDLK_u])
     {
          rotConfig.RotateY(kDeltaAng);
          q.th = SaturateAngle(q.th + kDeltaAng);
     }

     if(state[SDLK_o])
     {
          rotConfig.RotateY(-kDeltaAng);
          q.th = SaturateAngle(q.th - kDeltaAng);
     }
}

// Get the current state of the robot, just (x,y,z). Check notes on angles
void robot::GetState( double *state )
{
    int idx = 0;
    // Translation
    state[idx++] = this->transConfig.matrix[3][0];
    state[idx++] = this->transConfig.matrix[3][1];
    state[idx++] = this->transConfig.matrix[3][2];
}
