#include "sensorCamera.h"

sensorCamera::sensorCamera()
{

    speed = 0.01;
}

void sensorCamera::initSensor(float _angle, float _ratio, float _nearD, float _farD, vector3d pos, float th,bool MONO){
    ratio = _ratio;
    angle = _angle;
    nearD = _nearD;
    farD  = _farD;
    MONOCULAR = MONO;

    //for camerea view
    camYaw = th;
    camPos = pos;

    float tang;

    tang = (float)tan(angle* ANG2RAD * 0.5) ;
    nh = nearD * tang;
    nw = nh * ratio;
    fh = farD  * tang;
    fw = fh * ratio;


}

void sensorCamera::draw(void)
{

    //sensor position
    vector3d p(0.0,1.0,0.0);

    vector3d l(0.0,1.0,-1.0);

    //sensor up vector
    vector3d u(0,1,0);

    //sensor lookat
    if(!MONOCULAR){
        l.x = 0.0;
        l.y = 3.0;
        l.z = 0.0;

        u.z = 1;
        u.y = 0;
    }

    vector3d dir, nc, fc, X,Y,Z;

    // compute the Z axis of sensor
    // this axis points in the opposite direction from
    // the looking direction
    Z = p - l;
    Z.normalize();

    // X axis of sensor with given "up" vector and Z axis
    X = u.crossProduct(Z);
    X.normalize();

    // the real "up" vector is the cross product of Z and X
    Y = Z.crossProduct(X);


    // compute the centers of the near and far planes
    nc = p - Z * nearD;
    fc = p - Z * farD;

    // compute the 4 corners of the frustum on the near plane
    ntl = nc + Y * nh - X * nw;
    ntr = nc + Y * nh + X * nw;
    nbl = nc - Y * nh - X * nw;
    nbr = nc - Y * nh + X * nw;

    // compute the 4 corners of the frustum on the far plane
    ftl = fc + Y * fh - X * fw;
    ftr = fc + Y * fh + X * fw;
    fbl = fc - Y * fh - X * fw;
    fbr = fc - Y * fh + X * fw;

    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
        glColor3f(1.0,1.0,1.0);

        glVertex3f(ntl.x,ntl.y,ntl.z);
        glVertex3f(ntr.x,ntr.y,ntr.z);

        glVertex3f(ntr.x,ntr.y,ntr.z);
        glVertex3f(nbr.x,nbr.y,nbr.z);

        glVertex3f(nbr.x,nbr.y,nbr.z);
        glVertex3f(nbl.x,nbl.y,nbl.z);

        glVertex3f(nbl.x,nbl.y,nbl.z);
        glVertex3f(ntl.x,ntl.y,ntl.z);
    glEnd();

    glBegin(GL_LINES);
        glColor3f(1.0,1.0,1.0);

        glVertex3f(ntl.x,ntl.y,ntl.z);
        glVertex3f(ftl.x,ftl.y,ftl.z);

        glVertex3f(ntr.x,ntr.y,ntr.z);
        glVertex3f(ftr.x,ftr.y,ftr.z);

        glVertex3f(nbl.x,nbl.y,nbl.z);
        glVertex3f(fbl.x,fbl.y,fbl.z);

        glVertex3f(nbr.x,nbr.y,nbr.z);
        glVertex3f(fbr.x,fbr.y,fbr.z);

    glEnd();



    glBegin(GL_QUADS);
        glColor3f(0.5,0.59,0.59);
        glVertex3f(ftl.x,ftl.y,ftl.z);
        glVertex3f(ftr.x,ftr.y,ftr.z);
        glVertex3f(fbr.x,fbr.y,fbr.z);
        glVertex3f(fbl.x,fbl.y,fbl.z);

    glEnd();

    glEnable(GL_LIGHTING);


}

//For camera view
void sensorCamera::setCamPos(double *p)
{
    camPos.x = (float)p[0];
    camPos.y = (float)p[1];
    camPos.z = (float)p[2];
}

void sensorCamera::setAltura(double alt)
{
    camPos.y = alt;
}

void sensorCamera::setCamAngle(float angle)
{
    camYaw = angle;
}

void sensorCamera::update()
{
    glRotatef(-camYaw,0.0,1.0,0.0);

    glTranslatef(-camPos.x,-camPos.y,-camPos.z);

}

