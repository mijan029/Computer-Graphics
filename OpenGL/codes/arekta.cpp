#include <bits/stdc++.h>
using namespace std;

#include<stdio.h>
#include<stdlib.h>
#include<math.h>


#include <GL/glut.h>


#define pi (2*acos(0.0))


double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double initialrotation;
double maxlength;
double length;
double sradius;
double maxsradius;


struct point
{
    double x,y,z;
    point() {


    }
    point(double x, double y, double z) : x(x), y(y), z(z) {


    }
    point operator +(point a)  {
        point c;
        c.x=x+a.x;
        c.y=y+a.y;
        c.z=z+a.z;
        return c;
    }


    point operator -(point a)  {
        point c;
        c.x=x-a.x;
        c.y=y-a.y;
        c.z=z-a.z;
        return c;
    }


    point operator *(double a)  {
        point c;
        c.x=x*a;
        c.y=y*a;
        c.z=z*a;
        return c;
    }


    point operator /(double a)  {
        point c;
        c.x=x/a;
        c.y=y/a;
        c.z=z/a;
        return c;
    }


   
};
point cam(10, 10, 0), l(-10,-10,0),r(0,0,-1), u(0,1,0);


point cross(point a, point b)  {
    point c;
    c.x=a.y*b.z-a.z*b.y;
    c.y=a.z*b.x-a.x*b.z;
    c.z=a.x*b.y-a.y-b.x;
    return c;
}

void print(struct point p){
    cout<<p.x<<' '<<p.y<<' '<<p.z<<endl;
}

double dot(point a, point b) {
    double c;
    c=a.x * b.x + a.y * b.y + a.z*b.z;
    return c;
}
double modu(point a) {
    double c;
    c=a.x*a.x+a.y*a.y+a.z*a.z;
    c=sqrt(c);
    return c;
}
point unit(point a){
    point c;
    double mod=modu(a);
    c.x=a.x/mod;
    c.y=a.y/mod;
    c.z=a.z/mod;
    return c;
}




void drawAxes()
{
    if(drawaxes==1)
    {
       
        glBegin(GL_LINES);{
            glColor3f(1.0, 0.0, 0.0);
            glVertex3f( 100,0,0);
            glVertex3f(-100,0,0);


            glColor3f(0.0, 1.0, 0.0);
            glVertex3f(0,-100,0);
            glVertex3f(0, 100,0);


            glColor3f(0.0, 0.0, 1.0);
            glVertex3f(0,0, 100);
            glVertex3f(0,0,-100);
        }glEnd();
    }
}




void drawGrid()
{
    int i;
    if(drawgrid==1)
    {
        glColor3f(0.6, 0.6, 0.6);   //grey
        glBegin(GL_LINES);{
            for(i=-8;i<=8;i++){


                if(i==0)
                    continue;   //SKIP the MAIN axes


                //lines parallel to Y-axis
                glVertex3f(i*10, 0, -90);
                glVertex3f(i*10, 0,  90);


                //lines parallel to X-axis
                glVertex3f(-90, 0, i*10);
                glVertex3f( 90, 0, i*10);
            }
        }glEnd();
    }
}


void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);{
        glVertex3f( a, a,2);
        glVertex3f( a,-a,2);
        glVertex3f(-a,-a,2);
        glVertex3f(-a, a,2);
    }glEnd();
}




void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //gene.05 points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using gene.05d points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //gene.05 points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using gene.05d points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);


        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}
/*


void drawSphere(double radius,int slices,int stacks)
{
    struct point points[100][100];
    int i,j;
    double h,r;
    //gene.05 points
    for(i=0;i<=stacks;i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0;j<=slices;j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
            points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
            points[i][j].z=h;
        }
    }
    glColor3f(0,1,1);
    //draw quads using gene.05d points
    for(i=0;i<stacks;i++)
    {
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        for(j=0;j<slices;j++)
        {
            glBegin(GL_QUADS);{
                //upper hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
            }glEnd();
        }
    }
}
*/


void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);


    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);


    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();


    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}


void drawtriangle(point a,point b,point c){
    glBegin(GL_TRIANGLES);{
        glVertex3f(a.x,a.y,a.z);
        glVertex3f(b.x,b.y,b.z);
        glVertex3f(c.x,c.y,c.z);
    }glEnd();
}
void scaleandtranslation(){
    double difference= (maxlength-length)/3;
    glTranslatef(difference, difference, difference);
    glScaled(length, length, length);
}


void drawtriangles(){
    for(int i=0;i<8;i++){


        glPushMatrix();{
            if(i<4){
                if((i%2)==0) glColor3f(1, 0, 1.0f);
                else glColor3f(0, 1, 1.0f);
            }
            else{
                if((i%2)==0) glColor3f(0, 1, 1.0f);
                else glColor3f(1, 0, 1.0f);
            }
            glRotatef(90*i,0,1,0);
            if(i>=4) glRotatef(180,1,0,1);
            scaleandtranslation();
            point a(1,0,0), b(0,1,0), c(0,0,1);
            drawtriangle(a,b,c);
        }glPopMatrix();


    }




}


void drawspheres(){


    for(int i=0;i<6;i++){
       
        glPushMatrix();{
            if(i<4){
                if((i%2)==0)glColor3f(0, 0, 1);
                else glColor3f(0, 1, 0);
            }
            else glColor3f(1.0f, 0.0f, 0.0f);
            if (i<4) glRotatef(90*i,0,1,0);
            else glRotatef(90+180*i,1,0,0);
            glTranslatef(0,0,length);
            gluSphere(gluNewQuadric(),sradius,1000,1000);


        }glPopMatrix();


    }


   


}


void drawcylinders(){


    glColor3f(1.0f, 1.0f, 0.0f);
    for(int i=0;i<12;i++){
        glPushMatrix();{
        glRotatef(-90,0,1,0);
        glTranslatef(1/(sqrt(2))-sqrt(2.0)*1,0,-1/(sqrt(2)));
        if(i<4){}
        else if(i<8) glRotatef(90,1,0,0);
        else glRotatef(90,0,0,1);
        glRotatef(45+i*90,0,1,0);
        glTranslatef(length/sqrt(2),0,0);
        gluCylinder(gluNewQuadric(),sradius,sradius,length*sqrt(2.0),1000,1000);
        }glPopMatrix();
    }


}


void rotateleft(){
    r = r*cos(.05)+l*sin(.05);
    l = l*cos(.05)-r*sin(.05);
}


void rotateright(){
    r = r*cos(-.05) + l*sin(-.05);
    l = l*cos(-.05)-r*sin(-.05);
}


void lookup(){
    l = l*cos(.05)+u*sin(.05);
    u = u*cos(.05)-l*sin(.05);
}


void lookdown(){
    l = l*cos(-.05)+u*sin(-.05);
    u = u*cos(-.05)-l*sin(-.05);
}
void tiltcounterclock(){
    u = u*cos(.05)+r*sin(.05);
    r = r*cos(.05)-u*sin(.05);
}


void tiltclock(){
    u = u*cos(-.05)+r*sin(-.05);
    r = r*cos(-.05)-u*sin(-.05);
}


void keyboardListener(unsigned char key, int x,int y){
    switch(key){
        case 'a':
            initialrotation=initialrotation+5.0;
            break;
        case 'd':
            initialrotation=initialrotation-5.0;
            break;
        case '1':
            rotateleft();
            break;
        case '2':
            rotateright();
            break;
        case '3':
            lookup();
            break;
        case '4':
            lookdown();
            break;
        case '5':
            tiltcounterclock();
            break;
        case '6':
            tiltclock();
            break;
        case ',':
            length=length-0.1;
            //double radiusinc=maxsradius/14;
            sradius=sradius+maxsradius/16;
            if( length<0 ) {
                length = 0;
                sradius = maxsradius;
            }
            break;
        case '.':
            length=length+0.1;
            //double radiusinc=maxsradius/14;
            sradius=sradius-maxsradius/16;
            if( length>maxlength ) {
                length = maxlength;
                sradius = 0;
            }
            break;


       
        default:
            break;
    }
    glutPostRedisplay();
}
void moveforward(){
    cam=cam+l*0.2;
}
void movebackward(){
    cam=cam-l*0.2;
}
void moveright(){
    cam=cam+r;
}
void moveleft(){
    cam=cam-r;
}
void moveup(){
    cam=cam+u;
}
void movedown(){
    cam=cam-u;
}




void specialKeyListener(int key, int x,int y){
    switch(key){
        case GLUT_KEY_UP:
            moveforward();
            break;
        case GLUT_KEY_DOWN:    
            movebackward();
            break;


        case GLUT_KEY_RIGHT:
            moveright();
            break;
        case GLUT_KEY_LEFT :
            moveleft();
            break;


        case GLUT_KEY_PAGE_UP:
            moveup();
            break;
        case GLUT_KEY_PAGE_DOWN:
            movedown();
            break;
        case GLUT_KEY_INSERT:
            break;


        case GLUT_KEY_HOME:
            break;
        case GLUT_KEY_END:
            break;


        default:
            break;
    }
    glutPostRedisplay();
}




void mouseListener(int button, int state, int x, int y){    //x, y is the x-y of the screen (2D)
    switch(button){
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN){     // 2 times?? in ONE click? -- solution is checking DOWN or UP
                drawaxes=1-drawaxes;
            }
            break;


        case GLUT_RIGHT_BUTTON:
            //........
            break;


        case GLUT_MIDDLE_BUTTON:
            //........
            break;


        default:
            break;
    }
    glutPostRedisplay();
}






void display(){


    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClearColor(0,0,0,0);    //color black
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    /********************
    / set-up camera here
    ********************/
    //load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);


    //initialize the matrix
    glLoadIdentity();


    //now give three info
    //1. where is the camera (viewer)?
    //2. where is the camera looking?
    //3. Which direction is the camera's UP direction?


    //gluLookAt(100,100,100,    0,0,0,  0,0,1);
    //gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,       0,0,0,      0,0,1);
    gluLookAt(cam.x,cam.y,cam.z,cam.x+l.x,cam.y+l.y,cam.z+l.z,u.x,u.y,u.z);




    //again select MODEL-VIEW
    //glMatrixMode(GL_MODELVIEW);




    /****************************
    / Add your objects from here
    ****************************/
    //add objects


    glRotatef(initialrotation, 0, 1 ,0);
    cout<<initialrotation<<endl;
    print(cam); print(l); print(r); print(u);
    drawAxes();
    drawGrid();
    //drawSphere(1,95,95);
    drawtriangles();
    drawspheres();
    drawcylinders();


    //glColor3f(1,0,0);
    //drawSquare(10);


   // drawSS();


    //drawCircle(30,24);


    //drawCone(20,50,24);


    //drawSphere(30,24,20);








    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}




void animate(){
    angle+=0.05;
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}


void init(){
    //codes for initialization
    drawgrid=1;
    drawaxes=1;
    cameraHeight=150.0;
    cameraAngle=1.0;
    angle=0;
    initialrotation=45.0;
    maxlength=1.6;
    length=1.6;
    maxsradius = 1.6 / sqrt(3.0);
    sradius = 0.0;
    cout<<"dekhi"<<endl;


    //clear the screen
    //glClearColor(0,0,0,0);


    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);


    //initialize the matrix
    glLoadIdentity();


    //give PERSPECTIVE parameters
    gluPerspective(60,  1,  1,  1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}


int main(int argc, char **argv){
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(200, 10);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);   //Depth, Double buffer, RGB color


    glutCreateWindow("My OpenGL Program");


    init();


    glEnable(GL_DEPTH_TEST);    //enable Depth Testing


    glutDisplayFunc(display);   //display callback function
    //glutIdleFunc(animate);        //what you want to do in the idle time (when no drawing is occuring)


    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    //glutMouseFunc(mouseListener);


    glutMainLoop();     //The main loop of OpenGL


    return 0;
}



