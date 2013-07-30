
#include "simulator.h"

using namespace std;
using namespace cv;
double minescale = 1000.0;

int W_width = 1920;
int W_height = 1080;
int width = 592;
int height = 392;
int length_x_camera = 592;//400;
int length_y_camera = 392;//380;
int offset = 0;
bool first_run;
SDL_Surface* display;
double altura_camara = 0.5;
double altura_espejo = 2.0;
bool MONOCULAR;

enum textquality {solid, shaded, blended};

char* format(int n){
n--;
    char arr[12];
    arr[0] = 's';
    arr[1] = 'i';
    arr[2] = 'm';
    arr[3] = '_';
    arr[4] = arr[5] = arr[6] = '0';
    if(n>99){
        arr[4] = n/100+'0';
        n = n - (n/100)*100;
    }
    if(n>9){
        arr[5] = n/10+'0';
        n = n - (n/10)*10;
    }
    arr[6] = n+'0';
    arr[7] =  '.';
    arr[8] = 't';
    arr[9] = 'g';
    arr[10] = 'a';
    arr[11] = '\0';
    return arr;
}

simulator::simulator()
{
    putenv("SDL_VIDEO_CENTERED=1");
    SDL_Init(SDL_INIT_EVERYTHING);
    display = SDL_SetVideoMode(W_width,W_height,32,SDL_DOUBLEBUF|SDL_SWSURFACE|SDL_OPENGL|SDL_FULLSCREEN);

    glClearColor(1.0,1.0,1.0,1.0);
	glClearDepth(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,800.0/600.0,0.1,800.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);

    MONOCULAR = true;
    E.initEvader();
    E1.initEvader();
    E2.initEvader();

    R.initRobot(MONOCULAR,altura_camara);
    C.initRobot(MONOCULAR,altura_camara);

    W.initObjects();

    int argc = 1;
    char *argv[1] = {(char *)"Something"};
    glutInit(&argc, argv);
}

simulator::~simulator()
{
    SDL_Quit();
}

void output(int x, int y, float r, float g, float b, char *string,int offset)
{
  glColor3f( r, g, b );
  glRasterPos2f(x + offset, y + 8);
  int len, i;
  len = (int)strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24 , string[i]);
  }
}

void printText(){
    char *temp = "Image 1";
    output(0,360,0,0,0,temp,5);
    temp = "Image 2";
    output(480,360,0,0,0,temp,5);
    temp = "Current view";
    output(960,360,0,0,0,temp,5);
    temp = "Points' trajectories";
    output(1440,360,0,0,0,temp,5);
    temp = "Reduced Space";
    output(0,1040,0,0,0,temp,5);
    temp = "Realistic Space";
    output(640,1040,0,0,0,temp,5);
    temp = "3D View";
    output(1280,1040,0,0,0,temp,5);
}

void simulator::windows(void)
{
    glLineWidth(1.0);

    glBegin(GL_LINES);
    glColor3f(0.0,0.0,0.0);

    glVertex2f(0 , 0);
    glVertex2f(1920,0);

    glVertex2f(1920, 0);
    glVertex2f(1920,1080);

    glVertex2f(1920,1080);
    glVertex2f(0,1080);

    glVertex2f(0,1080);
    glVertex2f(0,0);

    //C1,C2,C3,Historia

    glVertex2f(480,0);
    glVertex2f(480,400);

    glVertex2f(960,0);
    glVertex2f(960,400);

    glVertex2f(1440,0);
    glVertex2f(1440,400);

    //Reducido, realista, 3d
    glVertex2f(640,400);
    glVertex2f(640,1080);

    glVertex2f(1280,400);
    glVertex2f(1280,1080);

    //horizontal
    glVertex2f(0,400);
    glVertex2f(1920,400);

    glEnd();
}

void simulator::show()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
     glShadeModel(GL_SMOOTH);

     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     glEnable(GL_LIGHT1);
     glEnable(GL_LIGHT2);
     glEnable(GL_LIGHT3);

     float pos2[]={1.5,0.5,1.5,0.0};
     float pos[]={-1.5,0.5,-1.5,0.0};
     float pos3[]={1.5,0.5,-1.5,0.0};
     float pos4[]={-1.5,0.5,1.5,0.0};
     float ambient[] = {0.0,0.0,0.0,1.0};

     GLfloat lightColor1[] = {1.0f, 1.0f, 1.0f, 1.0f};

	 //glLightfv(GL_LIGHT0,GL_DIFFUSE,dif);
	 glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
	 glLightfv(GL_LIGHT0,GL_POSITION,pos2);
	 glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);

     glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
     glLightfv(GL_LIGHT1, GL_POSITION, pos);

     glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor1);
     glLightfv(GL_LIGHT2, GL_POSITION, pos3);

     glLightfv(GL_LIGHT3, GL_DIFFUSE, lightColor1);
     glLightfv(GL_LIGHT3, GL_POSITION, pos4);

     glViewport(1280,400,640,640);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluPerspective(65.0,640/480,1,1000);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();


     cam.control();
     cam.update();

    W.drawWorld();
    E.drawEvader();
    R.drawRobot();

    //Vista C3 realista
    glViewport(960,0,480,360);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(R.sensor.angle, R.sensor.ratio, R.sensor.nearD, R.sensor.farD);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    R.sensor.update();
    W.drawWorld();
    E.drawEvader();

    //Vista C1 realista
    glViewport(0,0,480,360);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(C.sensor.angle, C.sensor.ratio, C.sensor.nearD, C.sensor.farD);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    C.sensor.setAltura(altura_camara);
    C.sensor.update();
    //W.drawWorld();
    E1.drawEvader();

    //Vista C2 realista
    glViewport(480,0,480,360);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(C.sensor.angle, C.sensor.ratio, C.sensor.nearD, C.sensor.farD);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    C.sensor.update();
    //W.drawWorld();
    E2.drawEvader();

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT0);

    //----Division for views
    glViewport(0, 0, W_width, W_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, W_width, 0, W_height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    windows();
}

void Screendump(char *destFile, short x0, short y0,short x1,short y1) {
    FILE   *out = fopen(destFile, "w");
    char   pixel_data[3*(y1-y0)*(x1-x0)];
    short  TGAhead[] = {0, 2, 0, 0, 0, 0, (x1-x0), (y1-y0), 24};

    glReadBuffer(GL_FRONT);
    glReadPixels(x0, y0, x1-x0, y1-y0, GL_BGR, GL_UNSIGNED_BYTE, pixel_data);
    fwrite(&TGAhead, sizeof(TGAhead), 1, out);
    fwrite(pixel_data, 3*(y1-y0)*(x1-x0), 1, out);
    fclose(out);
 }

void Screendump(char *destFile, short W, short H) {
    FILE   *out = fopen(destFile, "w");
    char   pixel_data[3*W*H];
    short  TGAhead[] = {0, 2, 0, 0, 0, 0, W, H, 24};
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, W, H, GL_BGR, GL_UNSIGNED_BYTE, pixel_data);
    fwrite(&TGAhead, sizeof(TGAhead), 1, out);
    fwrite(pixel_data, 3*W*H, 1, out);
    fclose(out);
 }

void glCircle(GLint x, GLint y, GLint radius,double r,double g,double b) {
    float angle;

    glColor3f(r,g,b);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < 100; i++) {
        angle = i*2*M_PI/100;
        glVertex2f(x + (cos(angle) * radius), y + (sin(angle) * radius));
    }
    glEnd();
}

void glFilledCircle(GLint x, GLint y, GLint radius,double r,double g,double b) {
    float angle;

    glColor3f(r,g,b);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for(int i = 0; i < 100; i++) {
        angle = i*2*M_PI/100;
        glVertex2f(x,y);
        glVertex2f(x + (cos(angle) * radius), y + (sin(angle) * radius));
    }
    glEnd();
}

//Funciones de navegacion
void cambiar_marco(Mat &points,Mat &p,int n){
    int i;
    double tempy,tempz;

    for(i=0;i<n;i++){
        tempy = points.at<double>(i,1);
        tempz = points.at<double>(i,2);

        p.at<double>(i,0) = -points.at<double>(i,0);
        p.at<double>(i,1) = tempz;
        p.at<double>(i,2) = tempy;
    }
}

void rotacion_z(Mat &points,Mat &p,double gamma,int n){
    Mat R(3,3,CV_64FC1);
    Mat X(3,1,CV_64FC1);
    Mat Y(3,1,CV_64FC1);

    int i;

    //Rotacion en Z
    R.at<double>(0,0) = cos(gamma);
    R.at<double>(0,1) = -sin(gamma);
    R.at<double>(0,2) = 0.0;
    R.at<double>(1,0) = sin(gamma);
    R.at<double>(1,1) = cos(gamma);
    R.at<double>(1,2) = 0.0;
    R.at<double>(2,0) = 0.0;
    R.at<double>(2,1) = 0.0;
    R.at<double>(2,2) = 1.0;

    for(i=0;i<n;i++){
        X.at<double>(0,0) = points.at<double>(i,0);
        X.at<double>(1,0) = points.at<double>(i,1);
        X.at<double>(2,0) = points.at<double>(i,2);
        Y = R*X;

        p.at<double>(i,0) = Y.at<double>(0,0);
        p.at<double>(i,1) = Y.at<double>(1,0);
        p.at<double>(i,2) = Y.at<double>(2,0);
    }
}

void correct_heading_angle(Mat X){
    //Deja el angulo entre -PI a PI
    double phi = X.at<double>(2,0);
    if(phi > 3.14159265358979)
	phi -= 6.28318530717958;
    if(phi < -3.14159265358979)
	phi += 6.28318530717958;

    X.at<double>(2,0) = phi;
}

void correct_heading_angle(double *X){
    //Deja el angulo entre -PI a PI
    double phi = *X;
    if(phi > PI)
        phi -= 2.0*PI;
    if(phi < -PI)
        phi += 2.0*PI;

    *X = phi;
}

void set_Mat3C(Mat src,uchar a){
    //Inicia los valores de una matriz a un valor unico dado
    //La matriz es de 3 canales
    int i,j,M = src.rows, N = src.cols;

    for(i=0;i<M;i++)
        for(j=0;j<N;j++){
            src.at<Vec3b>(i,j)[0] = a;
            src.at<Vec3b>(i,j)[1] = a;
            src.at<Vec3b>(i,j)[2] = a;
        }
}

void set_Mat(Mat src,uchar a){
    //Inicia los valores de una matriz a un valor unico dado
    int i,j,M = src.rows, N = src.cols;

    for(i=0;i<M;i++)
        for(j=0;j<N;j++)
            src.at<uchar>(i,j) = a;
}

void copy_Mat(Mat src,Mat des){
    //Copiamos una matriz src a des, de tipo double
    int i,j,M = src.rows,N = src.cols;

    for(i=0;i<M;i++)
        for(j=0;j<N;j++)
            des.at<double>(i,j) = src.at<double>(i,j);
}

void build_K(Mat K,double n,double ax,double ay,double s,double x0,double y0){
    //Construyo la matriz de parametros intrinsecos de la camara perspectiva
    K.at<double>(0,0) = ax*n;
    K.at<double>(0,1) = ax*n*s;
    K.at<double>(0,2) = x0;
    K.at<double>(1,0) = 0.0;
    K.at<double>(1,1) = ay*n;
    K.at<double>(1,2) = y0;
    K.at<double>(2,0) = 0.0;
    K.at<double>(2,1) = 0.0;
    K.at<double>(2,2) = 1.0;
}

void read_input(Mat K,Mat points,double *Psi){
    //Leo los parametros de camara asi como los puntos 3D
    //Tambien leo la configuracion inicial del robot
    ifstream in;
    in.open("input_camera");
    int t,i;

   in >> t;
   if(MONOCULAR){
   for(i=0;i<t;i++){
        in >> points.at<double>(i,0);
        in >> points.at<double>(i,1);
        in >> points.at<double>(i,2);
        points.at<double>(i,2) -= altura_camara;
   }
   }
   else{
        for(i=0;i<t;i++){
            in >> points.at<double>(i,0);
            in >> points.at<double>(i,1);
            in >> points.at<double>(i,2);
            points.at<double>(i,2) = altura_espejo - points.at<double>(i,2);
        }
   }

    double tCCDx,tCCDy,N,S,x0,y0,f;
    double resolucionX,resolucionY;
    in >> tCCDx;
    in >> tCCDy;
    in >> f;
    in >> N;
    in >> *Psi;
    in >> S;
    in >> x0;
    in >> y0;
    in >> resolucionX;
    in >> resolucionY;
    double alphax, alphay;
    alphax = 1000*f*resolucionX/tCCDx;
    alphay = 1000*f*resolucionY/tCCDy;

    build_K(K,N,alphax,alphay,S,x0,y0);
    in.close();
}

void build_RT(Mat RT,double X,double Y,double Theta){
    //Mediante este metodo calculo la tranformacion (Rotacion )
    //desde la configuracion (X,Y,Theta) hasta (0,0,0)
    Theta *= -1;

    RT.at<double>(0,0) = cos(Theta);
    RT.at<double>(0,1) = -sin(Theta);
    RT.at<double>(0,2) = 0.0;

    RT.at<double>(1,0) = sin(Theta);
    RT.at<double>(1,1) = cos(Theta);
    RT.at<double>(1,2) = 0.0;

    RT.at<double>(2,0) = 0.0;
    RT.at<double>(2,1) = 0.0;
    RT.at<double>(2,2) = 1.0;
}

void apply_RT(Mat points,Mat points2,double X,double Y,double Theta){
    //Esta funcion aplica RT a todos los puntos, osea pasar del marco
    //de referencia mundo, al marco del robot
    Mat RT,input,output;
    RT.create(3,3,CV_64FC1);
    build_RT(RT,X,Y,Theta);

    points2.create(points.rows,points.cols,CV_64FC1);
    input.create(3,1,CV_64FC1);
    output.create(3,1,CV_64FC1);

    int i,size = points.rows;
    for(i=0;i<size;i++){
        //Ordeno los puntos a convertir en un array columna input
        //APlico la transformacion RT
        //Escribo el punto transformado en points2
        input.at<double>(0,0) = points.at<double>(i,0) - X;
        input.at<double>(1,0) = points.at<double>(i,1) - Y;
        input.at<double>(2,0) = points.at<double>(i,2);

        output = RT*input;

        points2.at<double>(i,0) = output.at<double>(0,0);
        points2.at<double>(i,1) = output.at<double>(1,0);
        points2.at<double>(i,2) = output.at<double>(2,0);
    }
}

void points_to_image(Mat points,Mat backup,Mat K,double basex,double basey,int points_to_trace,int n){
    //Leo la lista de puntos y los escalo para mostrar en la camara
    //No se hace trampa, solo se reescalan para mejor visualizacion
    int addX[] = {1,2,-1,-2, 1, 2,-1,-2,3,-3, 3,-3, 0,0,1,-1};
    int addY[] = {1,2,-1,-2,-1,-2, 1, 2,3,-3,-3, 3,-1,1,0,0};
    int i,x,y,k,a,b;
    int M,N,size = points.rows;
    double maxX,maxY;
    M = 768;
    N = 1024;
    double reduc = 480.0/N;

    maxX = maxY = 0.0;
    for(i=0;i<size;i++){
        x = points.at<double>(i,0)-K.at<double>(0,2);
        y = points.at<double>(i,1)-K.at<double>(1,2);
        if(fabs(x) > maxX)
            maxX = fabs(x);
        if(fabs(y) > maxY)
            maxY = fabs(y);
    }

    maxX *= 1.1;
    maxY *= 1.1;

    double escalaX = N/(2*maxX);
    double escalaY = M/(2*maxY);
    escalaX = escalaY = 1.0;

    if(!MONOCULAR){
        glCircle(N/2*reduc+basex,M/2*reduc+basey,M/2*reduc,0.0,0.0,0.0);
        glFilledCircle(N/2*reduc+basex,M/2*reduc+basey,10,0.0,0.0,0.0);
    }

    if(escalaX > escalaY)
        escalaX = escalaY;
    else
        escalaY = escalaX;

    escalaX = escalaY*N/M*0.8;
    if(escalaY < minescale)
        minescale = escalaY;

    for(i=0;i<size;i++){
        x = (points.at<double>(i,0)-K.at<double>(0,2))*escalaX+K.at<double>(0,2);
        if(MONOCULAR){
                x = -(points.at<double>(i,0)-K.at<double>(0,2))*escalaX+K.at<double>(0,2);
        }
        y = (points.at<double>(i,1)-K.at<double>(1,2))*escalaY+K.at<double>(1,2);
        //glFilledCircle(x*reduc+basex,y*reduc+basey,3,0.0,0.0,0.0);

        if(i<points_to_trace){
            backup.at<double>(n,i*2)   = x*reduc;
            backup.at<double>(n,i*2+1) = y*reduc;
        }
    }
}

void points_to_image(Mat points,Mat K,double basex,double basey){
    //Leo la lista de puntos y los escalo para mostrar en la camara
    //No se hace trampa, solo se reescalan para mejor visualizacion
    int addX[] = {1,2,-1,-2, 1, 2,-1,-2,3,-3, 3,-3, 0,0,1,-1};
    int addY[] = {1,2,-1,-2,-1,-2, 1, 2,3,-3,-3, 3,-1,1,0,0};
    int i,x,y,k,a,b;
    int M,N,size = points.rows;
    double maxX,maxY;
    M = 768;
    N = 1024;
    double reduc = 480.0/N;

    maxX = maxY = 0.0;
    for(i=0;i<size;i++){
        x = points.at<double>(i,0)-K.at<double>(0,2);
        y = points.at<double>(i,1)-K.at<double>(1,2);
        if(fabs(x) > maxX)
            maxX = fabs(x);
        if(fabs(y) > maxY)
            maxY = fabs(y);
    }

    maxX *= 1.1;
    maxY *= 1.1;

    double escalaX = N/(2*maxX);
    double escalaY = M/(2*maxY);
    escalaX = escalaY = 1.0;

    if(!MONOCULAR){
        glCircle(N/2*reduc+basex,M/2*reduc+basey,M/2*reduc,0.0,0.0,0.0);
        glFilledCircle(N/2*reduc+basex,M/2*reduc+basey,10,0.0,0.0,0.0);
    }

    if(escalaX > escalaY)
        escalaX = escalaY;
    else
        escalaY = escalaX;

    //cout<<escalaX<<" "<<escalaY<<endl;
    //escalaY = 0.19;
    escalaX = escalaY*N/M*0.8;
    if(escalaY < minescale)
        minescale = escalaY;

    for(i=0;i<size;i++){
        x = (points.at<double>(i,0)-K.at<double>(0,2))*escalaX+K.at<double>(0,2);
        if(MONOCULAR){
                x = -(points.at<double>(i,0)-K.at<double>(0,2))*escalaX+K.at<double>(0,2);
        }
        y = (points.at<double>(i,1)-K.at<double>(1,2))*escalaY+K.at<double>(1,2);
        glFilledCircle(x*reduc+basex,y*reduc+basey,3,0.0,0.0,0.0);
    }
}

void convert3D_2D(Mat points,Mat K,double Psi){
    //Proyectamos los puntos 3D en el sensor de la camara
    int i,size = points.rows;
    double magnitud,x,y,z;

    Mat input(3,1,CV_64FC1);
    Mat output(3,1,CV_64FC1);
    for(i=0;i<size;i++){
        x = points.at<double>(i,0);
        y = points.at<double>(i,1);
        z = points.at<double>(i,2);
        magnitud = sqrt(pow(x,2) + pow(y,2) + pow(z,2));

        input.at<double>(0,0) = x/(z+Psi*magnitud);
        input.at<double>(1,0) = y/(z+Psi*magnitud);
        input.at<double>(2,0) = 1.0;

        output = K*input;

        points.at<double>(i,0) = output.at<double>(0,0);
        points.at<double>(i,1) = output.at<double>(1,0);
        points.at<double>(i,2) = output.at<double>(2,0);
    }
}

void convert3D_2D_sin_espejo(Mat points,Mat K,double Psi){
    //Proyectamos los puntos 3D en el sensor de la camara
    int i,size = points.rows;
    double x,y,z;

    Mat input(3,1,CV_64FC1);
    Mat output(3,1,CV_64FC1);
    for(i=0;i<size;i++){
        x = points.at<double>(i,0);
        y = points.at<double>(i,1);
        z = points.at<double>(i,2);

        input.at<double>(0,0) = x;
        input.at<double>(1,0) = y;
        input.at<double>(2,0) = z;

        output = K*input;

        points.at<double>(i,0) = output.at<double>(0,0)/output.at<double>(2,0);
        points.at<double>(i,1) = output.at<double>(1,0)/output.at<double>(2,0);
        points.at<double>(i,2) = 1.0;
    }
}

void convert_points_to_1D(Mat points,Mat output,Mat K){
    //Representamos los puntos por el seno y coseno del angulo formado
    int i,size = points.rows;
    double x;

    for(i=0;i<size;i++){
        //Calculo el angulo y lo paso al marco del robot
        x = points.at<double>(i,0);


        output.at<double>(i,0) = (x - K.at<double>(0,2))/K.at<double>(0,0);
        output.at<double>(i,1) = 1;
    }
}

void convert_points_to_sincos(Mat points,Mat output,Mat K){
    //Representamos los puntos por el seno y coseno del angulo formado
    int i,size = points.rows;
    double angle,x,y;

    for(i=0;i<size;i++){
        //Calculo el angulo y lo paso al marco del robot
        x = points.at<double>(i,0);
        y = points.at<double>(i,1);
        angle = atan2(y-K.at<double>(1,2),x-K.at<double>(0,2));
        angle += PI/2;

        correct_heading_angle(&angle);
        //Mysterius Pi sum detected
        output.at<double>(i,0) = sin(angle);
        output.at<double>(i,1) = cos(angle);
    }
}

void normalize_tensor(Mat tensor){
    //Normalizamos el tensor dividiendo por uno de sus elementos
    int i;
    double div = tensor.at<double>(2,0);

    for(i=0;i<8;i++)
	tensor.at<double>(i,0) = tensor.at<double>(i,0) / div;
}

void calculate_tensor_values_SVD(Mat C1,Mat C2,Mat C3,Mat tensor,int n){
    //Calculamos el tensor trifocal, utilizando las correspondencias de 3 imagenes
    //Y calculando el eigenvector correspondiente al menor eigenvalor.
    int i,size = n;
    double X1,X2,Y1,Y2,Z1,Z2;
    Mat temp(size,6,CV_64FC1);
    Mat trans(6,size,CV_64FC1);

    for(i=0;i<size;i++){
        X1 = C1.at<double>(i,0);
        X2 = C1.at<double>(i,1);

        Y1 = C2.at<double>(i,0);
        Y2 = C2.at<double>(i,1);

        Z1 = C3.at<double>(i,0);
        Z2 = C3.at<double>(i,1);

        temp.at<double>(i,0) = X1*Y1*Z1+X2*Y2*Z1; //T111
        temp.at<double>(i,1) = X1*Y1*Z2+X2*Y2*Z2; //T112
        temp.at<double>(i,2) = X1*Y2*Z1+X2*Y2*Z2; //T121
        temp.at<double>(i,3) = X1*Y2*Z2-X2*Y2*Z1; //T122
        temp.at<double>(i,4) = X2*Y1*Z1+X2*Y2*Z2; //T211
        temp.at<double>(i,5) = X2*Y1*Z2-X2*Y2*Z1; //T212
    }

    Mat val,vec,rig;
    transpose(temp,trans);
    SVD::compute(temp,val,rig,vec);

    for(i=0;i<6;i++)
        tensor.at<double>(i,0) = vec.at<double>(vec.rows-1,i);

    tensor.at<double>(6,0) = tensor.at<double>(0,0)-tensor.at<double>(3,0)-tensor.at<double>(5,0);
    tensor.at<double>(7,0) = tensor.at<double>(1,0)+tensor.at<double>(2,0)+tensor.at<double>(4,0);
}

void calculate_tensor_values_SVD8(Mat C1,Mat C2,Mat C3,Mat tensor,int n){
    //Calculamos el tensor trifocal, utilizando las correspondencias de 3 imagenes
    //Y calculando el eigenvector correspondiente al menor eigenvalor.
    int i,size = n;
    double X1,X2,Y1,Y2,Z1,Z2;
    Mat temp(size,8,CV_64FC1);
    Mat trans(8,size,CV_64FC1);

    for(i=0;i<size;i++){
        X1 = C1.at<double>(i,0);
        X2 = C1.at<double>(i,1);

        Y1 = C2.at<double>(i,0);
        Y2 = C2.at<double>(i,1);

        Z1 = C3.at<double>(i,0);
        Z2 = C3.at<double>(i,1);

        temp.at<double>(i,0) = X1*Y1*Z1; //T111
        temp.at<double>(i,1) = X1*Y1*Z2; //T112
        temp.at<double>(i,2) = X1*Y2*Z1; //T121
        temp.at<double>(i,3) = X1*Y2*Z2; //T122
        temp.at<double>(i,4) = X2*Y1*Z1; //T211
        temp.at<double>(i,5) = X2*Y1*Z2; //T212
        temp.at<double>(i,6) = X2*Y2*Z1; //T221
        temp.at<double>(i,7) = X2*Y2*Z2; //T222
    }

    Mat val,vec,rig;
    transpose(temp,trans);
    SVD::compute(temp,val,rig,vec);

    for(i=0;i<8;i++)
        tensor.at<double>(i,0) = vec.at<double>(vec.rows-1,i);
}

void calculate_tensor_at_hand(double tx2,double ty2,double theta2,double tx3,double ty3,double theta3,Mat tensor){
    tensor.at<double>(0,0) = ty3*sin(theta2) - ty2*sin(theta3);
    tensor.at<double>(1,0) = -tx3*sin(theta2) - ty2*cos(theta3);
    tensor.at<double>(2,0) = tx2*sin(theta3) + ty3*cos(theta2);
    tensor.at<double>(3,0) = tx2*cos(theta3) - tx3*cos(theta2);
    tensor.at<double>(4,0) = ty2*cos(theta3) - ty3*cos(theta2);
    tensor.at<double>(5,0) = tx3*cos(theta2) - ty2*sin(theta3);
    tensor.at<double>(6,0) = ty3*sin(theta2) - tx2*cos(theta3);
    tensor.at<double>(7,0) = tx2*sin(theta3) - tx3*sin(theta2);
}

void calculate_tensor_at_hand_C2(double tx1,double ty1,double theta1,double tx3,double ty3,double theta3,Mat tensor){
    tensor.at<double>(0,0) = -ty3*sin(theta1) + ty1*sin(theta3);
    tensor.at<double>(1,0) = tx3*sin(theta1) + ty1*cos(theta3);
    tensor.at<double>(2,0) = ty3*cos(theta1) - ty1*cos(theta3);
    tensor.at<double>(3,0) = -tx3*cos(theta1) + ty1*sin(theta3);
    tensor.at<double>(4,0) = -ty3*cos(theta1) - tx1*sin(theta3);
    tensor.at<double>(5,0) = tx3*cos(theta1) - tx1*cos(theta3);
    tensor.at<double>(6,0) = -ty3*sin(theta1) + tx1*cos(theta3);
    tensor.at<double>(7,0) = tx3*sin(theta1) - tx1*sin(theta3);
}

void calculate_tensor_at_hand_C3(double tx1,double ty1,double theta1,double tx2,double ty2,double theta2,Mat tensor){
    tensor.at<double>(0,0) = ty2*sin(theta1) - ty1*sin(theta2);
    tensor.at<double>(1,0) = -ty2*cos(theta1) + ty1*cos(theta2);
    tensor.at<double>(2,0) = -tx2*sin(theta1) - ty1*cos(theta2);
    tensor.at<double>(3,0) = tx2*cos(theta1) - ty1*sin(theta2);
    tensor.at<double>(4,0) = ty2*cos(theta1) + tx1*sin(theta2);
    tensor.at<double>(5,0) = ty2*sin(theta1) - tx1*cos(theta2);
    tensor.at<double>(6,0) = -tx2*cos(theta1) + tx1*cos(theta2);
    tensor.at<double>(7,0) = -tx2*sin(theta1) + tx1*sin(theta2);
}

void init_X2_reduc(Mat estado,Mat tensor,double dini){
    double escala;
    double tx,ty,phi;
    double a,b;

    tx = tensor.at<double>(6,0);
    ty = -tensor.at<double>(1,0);

    phi = 0.0;
    //Calculo de phi3 a partir de elementos del tensor
    a = (-tensor.at<double>(1,0) - tensor.at<double>(4,0))/dini;    //-T211 -  T121 / dini
    b = (tensor.at<double>(3,0) + tensor.at<double>(6,0))/dini;     //T212 + T122 / dini

    phi = atan2(b,a);
    //Termina calculo de phi3

    escala = dini*cos(phi)/tensor.at<double>(2,0);
    estado.at<double>(0,0) = tx*escala;
    estado.at<double>(1,0) = ty*escala;
    estado.at<double>(2,0) = phi;
    //cout<<"pose C2 wrt C3: "<<estado<<endl;
}

void init_X2(Mat estado,Mat tensor,double dini){
    Mat A(4,4,CV_64FC1);
    Mat X(4,1,CV_64FC1);
    Mat Y(4,1,CV_64FC1);

    A.at<double>(0,0) = 0.0;
    A.at<double>(0,1) = 0.0;
    A.at<double>(0,2) = 0.0;
    A.at<double>(0,3) = -1.0;

    A.at<double>(1,0) = 0.0;
    A.at<double>(1,1) = 1.0;
    A.at<double>(1,2) = 0.0;
    A.at<double>(1,3) = 0.0;

    A.at<double>(2,0) = 0.0;
    A.at<double>(2,1) = 0.0;
    A.at<double>(2,2) = -1.0/dini;
    A.at<double>(2,3) = -1.0/dini;

    A.at<double>(3,0) = 1.0/dini;
    A.at<double>(3,1) = 1.0/dini;
    A.at<double>(3,2) = 0.0;
    A.at<double>(3,3) = 0.0;

    //Necesito formar el vector X
    X.at<double>(0,0) = tensor.at<double>(1,0);
    X.at<double>(1,0) = tensor.at<double>(2,0);
    X.at<double>(2,0) = tensor.at<double>(5,0);
    X.at<double>(3,0) = tensor.at<double>(6,0);

    Y = A*X;

    double x,y,phi,esc;
    phi = atan(Y.at<double>(2,0)/Y.at<double>(3,0));
    esc = -(dini*cos(phi))/tensor.at<double>(4,0);

    x = Y.at<double>(0,0)*esc;
    y = Y.at<double>(1,0)*esc;

    estado.at<double>(0,0) = x;
    estado.at<double>(1,0) = y;
    estado.at<double>(2,0) = -phi;
    //cout<<"pose X2: ("<<x<<" , "<<y<<" , "<<phi<<")"<<endl;
}

void estimar_pose_evasor(Mat &C1,Mat &C2,Mat &C3,Mat &reducido,Mat &tensor,double dini,double R){
    //Este metodo llama al calculo del tensor para regresar la pose del evasor en el
    //espacio reducido

    calculate_tensor_values_SVD(C2,C3,C1,tensor,C1.rows);
    normalize_tensor(tensor);

    init_X2_reduc(reducido,tensor,dini);

    double tx,ty,phi;
    double x1,x2,y1,y2;

    tx = reducido.at<double>(0,0);
    ty = reducido.at<double>(1,0);
    phi = reducido.at<double>(2,0);

    x1 = (-tx)*cos(phi) - (-ty)*sin(phi);
    y1 = (-tx)*sin(phi) + (-ty)*cos(phi);

    x2 = (-x1)*cos(phi) + (R-y1)*sin(phi);
    y2 = -(-x1)*sin(phi) + (R-y1)*cos(phi);

    reducido.at<double>(0,0) = x2;
    reducido.at<double>(1,0) = y2;
}
//Terminan metodos de navegacion

void cartesian_to_polar(double x,double y,double *r,double *tetha){
    //Obtenemos la representacion en coordenadas polares del estado del evasor (X,Y)
    *r = sqrt(pow(x,2) + pow(y,2));
    *tetha = atan2(x,y);
}

int identificar_cuadrante(Mat &reducido){
    //determinamos a que cuadrante corresponden las coordenadas del evasor
    double r,tetha;
    double x = reducido.at<double>(0,0);
    double y = reducido.at<double>(1,0);

    if(y<fabs(0.000001))
        reducido.at<double>(1,0) = y = 0.0001;

    cartesian_to_polar(x,y,&r,&tetha);

    if(tetha>=0 && tetha <= PI/2.0) return 1;
    if(tetha>0) return 4;
    if(tetha<0 && tetha >= -PI/2.0) return 2;
    if(tetha <0) return 3;
    return -1;
}

double distancia_a_rIII(double Ve,double Vp,double b,double tetha,double l){
    //Calculamos la distancia desde el origen (Perseguidor) hasta la curva critica
    //que divide las regiones II y III
    return (l*Vp/Ve)-(Ve*tetha*b)/Vp;
}

double orientacion_BS(double Ve,double Vp){
    //Buscanos el angulo donde comienza la recta de transicion TS
    return acos(Ve/Vp);
}

double orientacion_TS(double Ve,double Vp,double b,double l){
    double S = acos(Ve/Vp);
    double x = -(b*Ve*cos(S))/Vp + l*sin(S);
    double y = -(b*Ve*pow(cos(S),2))/(Vp*sin(S)) + (b*cos(S))/sin(S) + l*cos(S);
    return atan2(x,y);
}

double distancia_a_barrera(double l,double Ve,double Vp,double tetha){
    //Calculamos las distancia a la barrera
    double S = acos(Ve/Vp);
    return (Vp*l*sin(S))/(Vp*sin(tetha)-Ve*sin(tetha-S));
}

double eval_ecuacion_675(double r,double tetha,double b,double l,double Ve,double Vp){
    //Evaluamos la ecuacion 6.75 que nos dice si estamos por encima
    //de la curva de transicion
    double seno = sin(tetha);
    double coseno = cos(tetha);

    return pow(r,4)*pow(seno,2)*pow(coseno,2) + pow(r,4)*pow(seno,4) + 2*b*pow(r,3)*pow(seno,3) +
    pow(b,2)*pow(r,2)*pow(seno,2) - (pow(Ve,2)*pow(b,2)*pow(r,2)*pow(coseno,2))/pow(Vp,2) + (2.0*Ve*b*l*pow(r,2)*seno*coseno)/Vp +
    (2.0*Ve*pow(b,2)*l*r*coseno)/Vp - pow(l,2)*pow(r,2)*pow(seno,2) - 2*b*pow(l,2)*r*seno - pow(l,2)*pow(b,2);
}

void realista_a_reducido(Mat &realista,Mat &reducido){
    //Calculamos la representacion del robot en el espacio reducido, esto es con respecto
    //al perseguidor
    double xe,ye,xp,yp,tetha;
    xp = realista.at<double>(0,0);
    yp = realista.at<double>(1,0);
    tetha = realista.at<double>(2,0);
    xe = realista.at<double>(3,0);
    ye = realista.at<double>(4,0);

    reducido.at<double>(0,0) = (xe-xp)*sin(tetha) - (ye-yp)*cos(tetha);
    reducido.at<double>(1,0) = (xe-xp)*cos(tetha) + (ye-yp)*sin(tetha);
}

void reducido_a_realista(Mat &realista,Mat &reducido){
    //Calculamos la representacion del robot en el espacio realista (en realidad podemos elegir
    //entre una infinidad de mapeos) a pardir de su representacion en el espacio reducido
    realista.at<double>(0,0) = 0.0;
    realista.at<double>(1,0) = 0.0;
    realista.at<double>(2,0) = 0.0;
    realista.at<double>(3,0) = reducido.at<double>(1,0);
    realista.at<double>(4,0) = -reducido.at<double>(0,0);
}

int identificar_region(Mat &realista,Mat &reducido,double b,double l,double Ve,double Vp){
    //Mediante la evaluacion de 4 condiciones, deducimos en que region se encuentra el evasor
    //con respecto al perseguidor
    double x,y;
    double r,tetha;
    double rIII,tethaBS,rB,tethaTS,eq675;

    //Todas las coordenadas las paso al primer cuadrante
    x = fabs(reducido.at<double>(0,0));
    y = fabs(reducido.at<double>(1,0));

    cartesian_to_polar(x,y,&r,&tetha);
    rIII = distancia_a_rIII(Ve,Vp,b,tetha,l);
    tethaBS = orientacion_BS(Ve,Vp);
    tethaTS = orientacion_TS(Ve,Vp,b,l);
    rB = distancia_a_barrera(l,Ve,Vp,tetha);
    eq675 = eval_ecuacion_675(r,tetha,b,l,Ve,Vp);

    if(fabs(x) < EPS && r > l) return 1;
    if(r > rIII) return 3;
    if((tetha >=0 && tetha < tethaTS) && eq675 > 0) return 2;
    if((tetha >= tethaTS && tetha <= tethaBS) && r > rB) return 2;
    if((tetha > tethaBS && tetha <= PI/2) && r > l) return 2;
    if(r > l) return 1;

    return -1;
}

void seleccionar_controles(Mat &controles,Mat &reducido,int region,double Vp,int cuad){
    //Decidimos el control a aplicar basados en la region actual del evasor
    if(region == 1){
        controles.at<double>(0,0) = Vp;
        controles.at<double>(1,0) = Vp;
    }
    else if(region == 2){
        controles.at<double>(0,0) = Vp;
        controles.at<double>(1,0) = -Vp;
    }
    else if(region == 3){
        controles.at<double>(0,0) = Vp;
        controles.at<double>(1,0) = -Vp;
    }
    else{
        cout<<"error: region indefinida, no es posible decidir control"<<endl;
    }

    //Modificamos los signos segun el cuandrante en el que se encuentre
    if(cuad == 1){
        //No necesita correccion
    }
    else if(cuad == 2){
        if(region == 2 || region == 3){
            controles.at<double>(0,0) *= -1;
            controles.at<double>(1,0) *= -1;
        }
    }
    else if(cuad == 3){
        if(region == 1){
            controles.at<double>(0,0) *= -1;
            controles.at<double>(1,0) *= -1;
        }
    }
    else if(cuad == 4){
        if(region == 1){
            controles.at<double>(0,0) *= -1;
            controles.at<double>(1,0) *= -1;
        }
        else if(region == 2 || region == 3){
            controles.at<double>(0,0) *= -1;
            controles.at<double>(1,0) *= -1;
        }
    }
    else{
        cout<<"error: cuadrante indefinido"<<endl;
    }
}

void f(Mat &realista,Mat &controles,double b,double deltaT){
    //Este metodo comprende las ecuaciones cinematicas que describen el avance de los jugadores
    //mediante sus ecuaciones de movimiento
    double tetha;
    tetha = realista.at<double>(2,0);

    double u1,u2,u3,u4;
    u1 = controles.at<double>(0,0);
    u2 = controles.at<double>(1,0);
    u3 = controles.at<double>(2,0);
    u4 = controles.at<double>(3,0);

    realista.at<double>(0,0) += ((u1+u2)/2)*cos(tetha)*deltaT;
    realista.at<double>(1,0) += ((u1+u2)/2)*sin(tetha)*deltaT;
    realista.at<double>(2,0) += (u2-u1)/(2*b)*deltaT;

    realista.at<double>(3,0) += u3*cos(u4)*deltaT;
    realista.at<double>(4,0) += u3*sin(u4)*deltaT;
}

void aplicar_control(Mat &realista,Mat &reducido,Mat &controles,double b,double deltaT){
    //aplicamos el control
    double u1,u2,u3,u4;
    double x,y;

    u1 = controles.at<double>(0,0);
    u2 = controles.at<double>(1,0);
    u3 = controles.at<double>(2,0);
    u4 = controles.at<double>(3,0);
    u4 = realista.at<double>(2,0) - u4;

    x = reducido.at<double>(0,0);
    y = reducido.at<double>(1,0);

    reducido.at<double>(0,0) += ((u2-u1)*y/(2*b) + u3*sin(u4))*deltaT;
    reducido.at<double>(1,0) += (-(u2-u1)*x/(2*b) - (u1+u2)/2 + u3*cos(u4))*deltaT;
}

void graficar_regionIII(Mat &output,int X,int Y,double Ve,double Vp,double b,double l,double dx,double basex,double basey){
    int centerX,centerY;
    int cuaX[] = {0,0, 0,1,1, 1,-1,-1,-1};
    int cuaY[] = {0,1,-1,1,0,-1,-1, 0, 1};
    centerX = X/2;
    centerY = Y/2;
    int x1,y1,i;

    double angle = 0.0;
    double delta = 0.001;
    double r,x,y;

    glBegin(GL_POINTS);
    glColor3f(0.0f,1.0f,0.0f);

    while(angle < 1.57){
        r = distancia_a_rIII(Ve,Vp,b,angle,l);
        x = r*sin(angle)*dx;
        y = r*cos(angle)*dx;

        for(i=0;i<9;i++){
            x1 = x + cuaX[i];
            y1 = y + cuaY[i];

            if(fabs(x1) < centerX && fabs(y1) < centerY){
                glVertex2f( x1+centerX+basex , y1+centerY+basey);
                glVertex2f( x1+centerX+basex ,-y1+centerY+basey);
                glVertex2f(-x1+centerX+basex , y1+centerY+basey);
                glVertex2f(-x1+centerX+basex ,-y1+centerY+basey);
            }
        }
        angle += delta;
    }
    glEnd();
}

void graficar_regionII(Mat &output,int X,int Y,double Ve,double Vp,double b,double l,double dx,double basex,double basey){
    int centerX,centerY;
    int cuaX[] = {0,0, 0,1,1, 1,-1,-1,-1};
    int cuaY[] = {0,1,-1,1,0,-1,-1, 0, 1};
    centerX = X/2;
    centerY = Y/2;
    int x1,y1,i;

    double angle = 0.0001;
    double delta = 0.001;
    double r,x,y;
    double bs,ts;
    double low, up, mid;
    double eval;
    int count;
    uchar red, green, blue;

    bs = orientacion_BS(Ve,Vp);
    ts = orientacion_TS(Ve,Vp,b,l);

    glBegin(GL_POINTS);
    while(angle < 1.57){
        if(angle < ts){
            low = l;
            up = distancia_a_rIII(Ve,Vp,b,angle,l);
            count = 0;

            while(count++<100 && low <= up){
                mid = (low + up)/2;
                eval = eval_ecuacion_675(mid,angle,b,l,Ve,Vp);

                if(fabs(eval) < EPS){
                    break;
                }

                else if(eval < 0){
                    low = mid;
                }
                else{
                    up = mid;
                }
            }

            r = mid;
            x = r*sin(angle)*dx;
            y  =r*cos(angle)*dx;
            red = 0;
            green = 0;
            blue = 255;
        }
        else if(angle < bs){
            r = distancia_a_barrera(l,Ve,Vp,angle);
            x = r*sin(angle)*dx;
            y = r*cos(angle)*dx;
            blue = green = 0;
            red = 255;
        }
        else{
            //Do nothing, arco del circulo de captura
        }

        glColor3f(red/255.0,green/255.0,blue/255.0);

        for(i=0;i<9;i++){
            x1 = x + cuaX[i];
            y1 = y + cuaY[i];
            if(fabs(x1) < centerX && fabs(y1) < centerY){
                glVertex2f( x1+centerX+basex, y1+centerY+basey);
                glVertex2f( x1+centerX+basex,-y1+centerY+basey);
                glVertex2f(-x1+centerX+basex, y1+centerY+basey);
                glVertex2f(-x1+centerX+basex,-y1+centerY+basey);
            }
        }
        angle += delta;
    }
    glEnd();
}

void draw_axis(Mat &output,double minX,double maxX,double minY,double maxY,double X,double Y,int div=-1,int mark=1,double basex=0,double basey=0){
    //IMprime ejes x y y graduados sobre una imagen
    double dx = (maxX - minX)/X;
    double dy = (maxY - minY)/Y;
    char cad[100];
    int separation = 35;
    int align;
    double add,midadd;
    add = 128.0;

    double range = min((maxX - minX) , (maxY - minY));
    if(div!=-1)
        add = range / div;
    else{
        while(range / add < 8)
            add= add/2.0;
    }
    midadd = add/2.0;

    glBegin(GL_LINES);
    glColor3f(0.0,0.0,0.0);
    //line(output,Point(separation,Y-separation), Point(X,Y-separation),Scalar(0,0,0));
    //line(output,Point(separation,Y-separation), Point(separation,0),Scalar(0,0,0));
    glVertex2f(separation+basex,separation+basey); glVertex2f(X+basex,separation+basey);
    glVertex2f(separation+basex,separation+basey); glVertex2f(separation+basex,Y+basey);
    glEnd();

    int i,j;
    double res;

    for(res = minX;res<maxX;res+=midadd){
        j = (res-minX)/dx;

        if(j >= separation){
            line(output,Point(j,Y-separation-2),Point(j,Y-separation+2),Scalar(0,0,0));

            if((int)res%mark==0){
                sprintf(cad,"%d",(int)res);

                align = 0;
                if((int)res < 0)
                    align = 12;

                putText(output, cad, Point(j-3-align,Y-10), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0,0,0));

                line(output,Point(j,Y-separation-5),Point(j,Y-separation+5),Scalar(0,0,0));
            }
        }
    }

     for(res = minY;res<maxY;res+=midadd){
        i = (res-minY)/dy;

        if(i >= separation){
            line(output,Point(separation-2,Y-i),Point(separation+2,Y-i),Scalar(0,0,0));

            if((int)res%mark==0){
                sprintf(cad,"%d",lround(res));
                putText(output, cad, Point(2,Y-i+3), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0,0,0));

                line(output,Point(separation-5,Y-i),Point(separation+5,Y-i),Scalar(0,0,0));
            }
        }
    }
}

void traceParticion(Mat &reducido,double l,int n,double Ve,double Vp,double b,double basex,double basey){
    int i,j;
    int X,Y,centerX,centerY,x1,y1;
    int addX[] = {-2,-1,1,2,-2,-1,1,2};
    int addY[] = {-2,-1,1,2,2,1,-1,-2};
    int cuaX[] = {0,0, 0,1,1, 1,-1,-1,-1};
    int cuaY[] = {0,1,-1,1,0,-1,-1, 0, 1};

    X = 641;
    Y = 641;
    centerX = X/2;
    centerY = Y/2;

    Mat output(Y,X,CV_8UC3);

    double MAX_DISTANCE = 4;//8.0;
    double dx = centerX/MAX_DISTANCE;
    double x,y;

    //Trazar circulo de radio l
    glCircle(centerX+basex,centerY+basey,l*dx,0.0,0.0,0.0);

    glBegin(GL_LINES);
    glColor3f(0.0,0.0,0.0);
    glVertex2f(centerX+basex,0+basey); glVertex2f(centerX+basex,Y+basey);
    glVertex2f(0+basex,centerY+basey); glVertex2f(X+basex,centerY+basey);
    glEnd();

    first_run = false;
    graficar_regionIII(output,X,Y,Ve,Vp,b,l,dx,basex,basey);
    graficar_regionII(output,X,Y,Ve,Vp,b,l,dx,basex,basey);

    glBegin(GL_POINTS);
    glColor3f(0.0,0.0,0.0);
    for(i=0;i<n;i++){
        x = reducido.at<double>(i,0)*dx;
        y = reducido.at<double>(i,1)*dx;

        //Pintar pasos previos del evasor
        for(j=0;j<9;j++){
            x1 = x + cuaX[j];
            y1 = y + cuaY[j];
            if(fabs(x1) < centerX && fabs(y1) < centerY){
                glVertex2f(x1+centerX+basex,y1+centerY+basey);
            }
        }
    }

    //solo el ultimo punto se pinta como cruz
    for(int k=0;k<8;k++){
                if(x+addX[k]+centerX >= 0 && x+addX[k]+centerX < X &&
                   y+addY[k]+centerY >= 0 && y+addY[k]+centerY < Y){
                    glVertex2f(x+addX[k]+centerX+basex,y+addY[k]+centerY+basey);
                }
    }
    glEnd();

    draw_axis(output,-MAX_DISTANCE,MAX_DISTANCE,-MAX_DISTANCE,MAX_DISTANCE,X,Y,4,1,basex,basey);
}

void print_triangle(Mat &output,double x,double y,double theta,double basex,double basey){
    //Pintar el vector de movimiento
    double L = 5;
    double l = 12;
    double x2,y2,x3,y3,x4,y4;

    x2 = x + L*cos(theta);
    y2 = y + L*sin(theta);

    x3 = x2 + l*cos(theta + 3*PI/4);
    y3 = y2 + l*sin(theta + 3*PI/4);

    x4 = x2 + l*cos(theta + 5*PI/4);
    y4 = y2 + l*sin(theta + 5*PI/4);

    glBegin(GL_TRIANGLES);
    glVertex2f(x2+basex,y2+basey);
    glVertex2f(x3+basex,y3+basey);
    glVertex2f(x4+basex,y4+basey);
    glEnd();
}

void trace(Mat &path,int n,bool done,double l,double basex,double basey){
    int i,j,x1,y1;
    int cuaX[] = {0,0, 0,1,1, 1,-1,-1,-1};
    int cuaY[] = {0,1,-1,1,0,-1,-1, 0, 1};
    double xe,ye,xp,yp,tetha;
    double xmin,xmax,ymin,ymax;


    xmin = ymin = 2000000000.0;
    xmax = ymax = -2000000000.0;
    for(i=0;i<n;i++){
        xp = path.at<double>(i,0);
        yp = path.at<double>(i,1);

        xe = path.at<double>(i,3);
        ye = path.at<double>(i,4);

        xmin = min(xmin , min(xp,xe));
        xmax = max(xmax , max(xp,xe));
        ymin = min(ymin , min(yp,ye));
        ymax = max(ymax , max(yp,ye));
    }

     xmin = -1.5;
     ymin = -4.0;
     xmax =  4.5;
     ymax =  2.0;

    double rangeX = fabs(xmax - xmin);
    double rangeY = fabs(ymax - ymin);

    double dx = Xres/rangeX;
    double dy = Yres/rangeY;

    Mat output(Yres,Xres,CV_8UC3);

    draw_axis(output,xmin,xmax,ymin,ymax,Xres,Yres,3,1,basex,basey);

    int x,y;

    for(i=0;i<n;i++){
        xp = path.at<double>(i,0);
        yp = path.at<double>(i,1);
        tetha = path.at<double>(i,2);

        x = (xp - xmin)*dx;
        y = (yp - ymin)*dy;

        glBegin(GL_POINTS);
        glColor3f(1.0,0.0,0.0);
        for(j=0;j<9;j++){
            x1 = x + cuaX[j];
            y1 = y + cuaY[j];
            glVertex2f(x1+basex,y1+basey);
        }
        glEnd();

        if(i%50==0 || i+1==n)
            print_triangle(output,x,y,tetha,basex,basey);
    }

    double xdamn = x;
    double ydamn = y;
    if(done){
        glCircle(xdamn+basex,ydamn+basey,l*dx,0.0,0.0,0.0);
    }

    for(i=0;i<n;i++){
        xe = path.at<double>(i,3);
        ye = path.at<double>(i,4);
        x = (xe - xmin)*dx;
        y = (ye - ymin)*dy;
        if(i%50==0 || i+1==n)
            glFilledCircle(x+basex,y+basey,5,0.0,0.0,1.0);

        glBegin(GL_POINTS);
        glColor3f(0.0,0.0,1.0);
        for(j=0;j<9;j++){
            x1 = x + cuaX[j];
            y1 = y + cuaY[j];
            glVertex2f(x1+basex,y1+basey);
        }
        glEnd();
    }

    if(done){
        glCircle(xdamn+basex,ydamn+basey,l*dx,0.0,0.0,0.0);
    }
}

void trace_points_path(Mat &path,int n,int points_to_trace,double basex,double basey){
    int i,j,k,x1,y1,x0,y0;
    int cuaX[] = {0,1, 1,-1,-1,0, 0,1,-1};
    int cuaY[] = {0,1,-1,-1, 1,1,-1,0, 0};
    double x,y,xprev,yprev;
    double reduc = 480.0/1024;

    if(!MONOCULAR){
        int M,N;
        M = 768;
        N = 1024;
        glCircle(N/2*reduc+basex,M/2*reduc+basey,M/2*reduc,0.0,0.0,0.0);
        glFilledCircle(N/2*reduc+basex,M/2*reduc+basey,10,0.0,0.0,0.0);
    }

    for(i=2;i<n;i++){
        for(j=0;j<points_to_trace;j++){
            x = path.at<double>(i,j*2)+basex;
            y = path.at<double>(i,j*2+1)+basey;

            xprev = path.at<double>(i-1,j*2)+basex;
            yprev = path.at<double>(i-1,j*2+1)+basey;

            glBegin(GL_LINES);
            glVertex2f(x,y); glVertex2f(xprev,yprev);
            glEnd();

            for(k=0;k<5;k++){
                x = x + cuaX[k];
                y = y + cuaY[k];
                glBegin(GL_POINTS);
                glColor3f(0.0,0.0,0.0);
                glVertex2f(x+basex,y+basey);
                glEnd();
            }
        }
    }
}

double calcular_tiempo_necesario(double theta,double deltaT,double alpha){
    //Estimamos el tiempo necesario de giro, para un control de 2 fases
    if(PI-fabs(theta) < fabs(theta)){
        //Significa que se alinea en reversa
        return fabs(((PI-fabs(theta))*deltaT)/alpha);
    }
    else{
        return fabs((fabs(theta)*deltaT)/alpha);
    }
}

void desplaza_evasor(Mat points,Mat p,double x,double y,double theta,double dini,double R){
    apply_RT(points,p,0,dini+R,0);
    apply_RT(p,p,-x,-y,0);
}

void simulator::start()
{
	SDL_Event event;
    Uint32 startTime;
    running = true;
    std::cout << "started the simulation" << std::endl;

    //INIT PART
    ifstream input("input");
    ifstream input2("input_camera");
    Mat realista(5,1,CV_64FC1);
    Mat reducido(3,1,CV_64FC1);
    Mat evasor(3,1,CV_64FC1);
    Mat controles(4,1,CV_64FC1);
    Mat path(500000,5,CV_64FC1);
    Mat reduc_path(500000,2,CV_64FC1);

    double Ve, Vp;
    double l,b,n,de,dini = 1.0;
    int points_to_trace = 12;

    Mat points_path(500000,points_to_trace*2,CV_64FC1);

    input2 >> n;
    input2.close();
    //n = 30;

    double Psi;
    Mat points(n,3,CV_64FC1);
    Mat K(3,3,CV_64FC1);
    Mat p1(n,3,CV_64FC1);
    Mat p2(n,3,CV_64FC1);
    Mat p3(n,3,CV_64FC1);
    Mat c1(n,2,CV_64FC1);
    Mat c2(n,2,CV_64FC1);
    Mat c3(n,2,CV_64FC1);
    Mat estado(3,1,CV_64FC1);
    Mat tensor(8,1,CV_64FC1);

    read_input(K,points,&Psi);

    apply_RT(points,p1,0,0,0);
    apply_RT(points,p2,0,dini,0);

    if(MONOCULAR){
        cambiar_marco(p1,p1,points.rows);
        cambiar_marco(p2,p2,points.rows);
        convert3D_2D_sin_espejo(p1,K,Psi);
        convert3D_2D_sin_espejo(p2,K,Psi);
        convert_points_to_1D(p1,c1,K);
        convert_points_to_1D(p2,c2,K);
    }
    else{
        convert3D_2D(p1,K,Psi);
        convert3D_2D(p2,K,Psi);
        convert_points_to_sincos(p1,c1,K);
        convert_points_to_sincos(p2,c2,K);
    }

    input >> Vp >> Ve >> l >> b >> de;
    for(int i=0;i<3;i++)
        input >> reducido.at<double>(i,0);

    controles.at<double>(2,0) = Ve;
    //controles.at<double>(3,0) = -PI/6;
    //controles.at<double>(3,0) = -PI/6.5;
    controles.at<double>(3,0) = 0.0;

    Mat reducido2(2,1,CV_64FC1);
    Mat controles2(4,1,CV_64FC1);
    bool fase2;
    double deltaT = 0.01;
    double remaining_time,alpha,necesary_time;
    double x,y,r,theta;
    double T = 0.0;
    bool mid = true;
    int i;
    int region;
    int counter = 0;
    n = 0;
    int cuad;
    SDL_Rect dest;

    fase2 = false;
    alpha = (Vp*deltaT)/b;

    reducido_a_realista(realista,reducido);
    first_run = true;

    //INIT END

    startTime = SDL_GetTicks();
	while(running)
	{
        if(!fase2){
            desplaza_evasor(points,p3,reducido.at<double>(0,0),reducido.at<double>(1,0),PI/4,dini,de);

            if(MONOCULAR){
                cambiar_marco(p3,p3,points.rows);
                convert3D_2D_sin_espejo(p3,K,Psi);
                convert_points_to_1D(p3,c3,K);
            }
            else{
                convert3D_2D(p3,K,Psi);
                convert_points_to_sincos(p3,c3,K);
            }

            estimar_pose_evasor(c1,c2,c3,evasor,tensor,dini,de);

            if(first_run){

                path.at<double>(n,0) = realista.at<double>(0,0);
                path.at<double>(n,1) = realista.at<double>(1,0);
                path.at<double>(n,2) = realista.at<double>(2,0);
                path.at<double>(n,3) = realista.at<double>(3,0);
                path.at<double>(n,4) = realista.at<double>(4,0);

                reduc_path.at<double>(n,0) = reducido.at<double>(0,0);
                reduc_path.at<double>(n,1) = reducido.at<double>(1,0);

                R.setRobot(realista.at<double>(0,0) , -realista.at<double>(1,0), realista.at<double>(2,0));
                E.setEvader(realista.at<double>(3,0), -realista.at<double>(4,0));
                //Establezco las posiciones para C1, C2
                C.setRobot(0.0, 0.0, 0.0);
                E1.setEvader(0.0,-dini-de);
                E2.setEvader(0.0,-de);

                show();

                trace(path,++n,false,l,640,400);
                traceParticion(reduc_path,l,n,Ve,Vp,b,0,400);
                printText();

                SDL_GL_SwapBuffers();
                Screendump("sim_inicio"  ,0   ,0  ,1920,1080);
                Screendump("sim_001.tga"  ,0   ,0  ,1920,1080);
                Screendump("inicio_imagen1"  ,0   ,0  ,480 ,400);
                Screendump("inicio_imagen2"  ,480 ,0  ,960 ,400);
                Screendump("inicio_current"  ,960 ,0  ,1440,400);
                Screendump("inicio_historial",1440,0  ,1920,400);
                Screendump("inicio_reducido" ,0   ,400,640 ,1080);
                Screendump("inicio_realista" ,640 ,400,1280,1080);
                Screendump("inicio_3d"       ,1280,400,1920,1080);
            }
        }

        reducido2.at<double>(0,0) = evasor.at<double>(0,0);
        reducido2.at<double>(1,0) = evasor.at<double>(1,0);
        cuad = identificar_cuadrante(evasor);

        region = identificar_region(realista,evasor,b,l,Ve,Vp);

        if(region == -1){
            cout<<"captura en t: "<<T<<endl;
            break;
        }

        seleccionar_controles(controles,evasor,region,Vp,cuad);

        controles2.at<double>(0,0) = controles.at<double>(0,0);
        controles2.at<double>(1,0) = controles.at<double>(1,0);
        controles2.at<double>(2,0) = 0.0;
        controles2.at<double>(3,0) = 0.0;

        aplicar_control(realista,reducido2,controles2,b,deltaT);

        if(fase2){
            fase2 = false;
            aplicar_control(realista,evasor,controles,b,remaining_time);
            aplicar_control(realista,reducido,controles,b,remaining_time);
            f(realista,controles,b,remaining_time);
            counter++;
        }
        else if(identificar_cuadrante(evasor)!=identificar_cuadrante(reducido2) && region!=1){
            x = evasor.at<double>(0,0);
            y = evasor.at<double>(1,0);
            cartesian_to_polar(x,y,&r,&theta);
            necesary_time = calcular_tiempo_necesario(theta,deltaT,alpha);

            controles2.at<double>(2,0) = 0.0;
            controles2.at<double>(3,0) = 0.0;

            aplicar_control(realista,evasor,controles2,b,necesary_time);
            aplicar_control(realista,reducido,controles2,b,necesary_time);
            f(realista,controles2,b,necesary_time);

            remaining_time = deltaT - necesary_time;
            fase2 = true;
        }
        else{
            necesary_time = remaining_time = 0.0;
            aplicar_control(realista,evasor,controles,b,deltaT);
            aplicar_control(realista,reducido,controles,b,deltaT);
            f(realista,controles,b,deltaT);
            counter++;
        }

        R.setRobot(realista.at<double>(0,0) , -realista.at<double>(1,0), realista.at<double>(2,0));
        E.setEvader(realista.at<double>(3,0), -realista.at<double>(4,0));

        //Establezco las posiciones para C1, C2
        C.setRobot(0.0, 0.0, 0.0);
        E1.setEvader(0.0,-dini-de);
        E2.setEvader(0.0,-de);

        if(counter%2==0 && !fase2){

            path.at<double>(n,0) = realista.at<double>(0,0);
            path.at<double>(n,1) = realista.at<double>(1,0);
            path.at<double>(n,2) = realista.at<double>(2,0);
            path.at<double>(n,3) = realista.at<double>(3,0);
            path.at<double>(n,4) = realista.at<double>(4,0);

            reduc_path.at<double>(n,0) = reducido.at<double>(0,0);
            reduc_path.at<double>(n,1) = reducido.at<double>(1,0);

            show();
            points_to_image(p3,points_path,K,960,0,points_to_trace,n);

            trace(path,++n,false,l,640,400);
            traceParticion(reduc_path,l,n,Ve,Vp,b,0,400);
            trace_points_path(points_path,n,points_to_trace,1440,0);
            printText();

            SDL_GL_SwapBuffers();
            Screendump(format(n)  ,0   ,0  ,1920,1080);
            if(1000/FPS>(SDL_GetTicks()-startTime))
                SDL_Delay(1000/FPS-(SDL_GetTicks()-startTime));
        }

        if(fase2)
            T+= 0.0;
        else
            T+= deltaT;

        if(T>1.0 && mid){
        //if(T>1.75 && mid){
            Screendump("sim_mitad",0,0,1920,1080);
            Screendump("mitad_imagen1"  ,0   ,0  ,480 ,400);
            Screendump("mitad_imagen2"  ,480 ,0  ,960 ,400);
            Screendump("mitad_current"  ,960 ,0  ,1440,400);
            Screendump("mitad_historial",1440,0  ,1920,400);
            Screendump("mitad_reducido" ,0   ,400,640 ,1080);
            Screendump("mitad_realista" ,640 ,400,1280,1080);
            Screendump("mitad_3d"       ,1280,400,1920,1080);

            //controles.at<double>(3,0) = -PI/6.5-PI/4;
            mid = false;
        }

        startTime = SDL_GetTicks();
    }

    n--;
    show();
    points_to_image(p3,points_path,K,960,0,points_to_trace,n);

    trace(path,++n,true,l,640,400);
    traceParticion(reduc_path,l,n,Ve,Vp,b,0,400);
    trace_points_path(points_path,n,points_to_trace,1440,0);
    printText();

    SDL_GL_SwapBuffers();
    Screendump(format(n+1)  ,0   ,0  ,1920,1080);
    Screendump(format(n+2)  ,0   ,0  ,1920,1080);
    Screendump("sim_fin"    ,0   ,0  ,1920,1080);
    Screendump("final_imagen1"  ,0   ,0  ,480 ,400);
    Screendump("final_imagen2"  ,480 ,0  ,960 ,400);
    Screendump("final_current"  ,960 ,0  ,1440,400);
    Screendump("final_historial",1440,0  ,1920,400);
    Screendump("final_reducido" ,0   ,400,640 ,1080);
    Screendump("final_realista" ,640 ,400,1280,1080);
    Screendump("final_3d"       ,1280,400,1920,1080);

    cout<<"Representacion en el espacio realista "<<realista<<endl;
    cout<<"Representacion en el estado reducido "<<reducido<<endl;
    cout<<"escala menor "<<minescale<<endl;
    input.close();
    SDL_Delay(3000);
}
