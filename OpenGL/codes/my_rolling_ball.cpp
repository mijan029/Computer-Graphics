#include <bits/stdc++.h>
using namespace std;
#define pi acos(-1.0)

#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif

int counter = 0;
float theta = pi/180; 
float angle=0;
float radius = 0.3;
float velocity = 0.6;
float dt = 0.1;
float Dt=0.1;
int agao=0, changeDirection=0;
int animate=0;


class pt{
    public:
    float x, y, z;
    pt(){

    }
    pt(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
    pt operator+(const pt& p){
        return pt(p.x+x, p.y+y, p.z+z);
    }
    pt operator-(const pt& p){
        return pt(-p.x+x, -p.y+y, -p.z+z);
    }

    pt operator*(float p){
        return pt(p*x, p*y, p*z);
    }
    pt operator/(float p){
        return pt(x/p, y/p, z/p);
    }
    float len(){
        return sqrt(x*x+y*y+z*z);
    }
    pt unit(){
        return pt(x/len(), y/len(), z/len());
    }
    float dot(pt p){
        return x*p.x + y*p.y + z*p.z;
    }

    pt cross(pt p){
        return pt(y*p.z-z*p.y, z*p.x-x*p.z, x*p.y-y*p.x);
    }

    pt matixMul(float** mat){
        return pt(
            mat[0][0]*x+mat[0][1]*y+mat[0][2]*z,
            mat[1][0]*x+mat[1][1]*y+mat[1][2]*z,
            mat[2][0]*x+mat[2][1]*y+mat[2][2]*z
        );
    }

    void print(){
        cout<<x<<' '<<y<<' '<<z<<endl;
    }
};

pt direction(1/sqrt(2.0),0,1/sqrt(2.0)), position(0,radius,0), rotatingAxis, track(0,1,0);
pt camera(4, 4, 4), look(-4,-4,-4), up(0,1,0);



float** rotation(pt a, float theta){
    float** mat = new float*[3];
    mat[0] = new float[3];
    mat[1] = new float[3];
    mat[2] = new float[3];
    mat[0][0] = cos(theta) + (1-cos(theta))*a.x*a.x ;
    mat[0][1] =  (1-cos(theta))*a.x*a.y + sin(theta)*(-a.z);
    mat[0][2] =  (1-cos(theta))*a.x*a.z + sin(theta)*a.y;
    mat[1][0] =  (1-cos(theta))*a.x*a.y + sin(theta)*a.z;
    mat[1][1] = cos(theta) + (1-cos(theta))*a.y*a.y ;
    mat[1][2] =  (1-cos(theta))*a.y*a.z + sin(theta)*(-a.x);
    mat[2][0] =  (1-cos(theta))*a.x*a.z + sin(theta)*(-a.y);
    mat[2][1] =  (1-cos(theta))*a.z*a.y + sin(theta)*a.x;
    mat[2][2] = cos(theta) + (1-cos(theta))*a.z*a.z ;



    return mat;
}


void drawxz(int a){

    for(int i=-a; i<=a; i++){
        
        if(i!=0){
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);{
                glVertex3f(-a,0,i);
                glVertex3f(a,0,i);
        
                glVertex3f(i,0,a);
                glVertex3f(i,0,-a);

            }glEnd();
        }
    }

}

void axes()
{
        int a = 20;
        drawxz(a);
        glLineWidth(3);
		glBegin(GL_LINES);{

            glColor3f(1.0, 0.0, 0.0);
			glVertex3f( -a,0,0);
			glVertex3f(a,0,0);

            glColor3f(0.0, 1.0, 0.0);
			glVertex3f(0,a,0);
			glVertex3f(0,-a,0);

           
            glColor3f(0.0, 0.0, 1.0);
			glVertex3f(0,0,a);
			glVertex3f(0,0,-a);
		
		}glEnd();

        
    
}

void chessboard(){
    int a = 20;
    for(int i=-a; i<a; i++){
        for(int j=-a; j<a; j++){
            if((i+j)%2){
                glColor3f(0,0,0);
            }else{
                glColor3f(1,1,1);
            }
            glBegin(GL_QUADS);{
                glVertex3f( i,0,j);
                glVertex3f(i+1,0,j);
                glVertex3f(i+1,0,j+1);
                glVertex3f(i,0,j+1);
            }glEnd();
        }
    }
    
}

void square(float a){
    glBegin(GL_QUADS);
    {

        glVertex3f(a, a, 0);
        glVertex3f(a, -a, 0);
        glVertex3f(-a, -a, 0);
        glVertex3f(-a, a, 0);

    }glEnd();
}

int cnt=0;
void drawSphere(double radius,int slices,int stacks)
{
	pt points[105][105];
	int i,j;
	double h,r;
	//generate points
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
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices/8;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                // glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				// glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				// glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				// glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void alignBall(float radius){
    glPushMatrix();
    glRotatef(-90,0,1,0);
    glRotatef(-90,1,0,0);
    drawSphere(radius,88,88);
    glPopMatrix();
}

void ball(float radius){

    for(int i=0; i<4; i++){
        glPushMatrix();
        glColor3f(1,0,0);
        glRotatef(90*i,0,1,0);
        alignBall(radius);
        glPopMatrix();
    }

    glPushMatrix();
    glRotatef(45,0,1,0);
    glRotatef(180,1,0,0);
    glRotatef(45,0,1,0);
        for(int i=0; i<4; i++){
            glPushMatrix();
            glColor3f(1,0,0);
            glRotatef(90*i,0,1,0);
            alignBall(radius);
            glPopMatrix();
        }
    glPopMatrix();




    for(int i=0; i<4; i++){
        glPushMatrix();
        glColor3f(0,1,0);
        glRotatef(90*i+45,0,1,0);
        alignBall(radius);
        glPopMatrix();
    }

    glPushMatrix();
    glRotatef(45,0,1,0);
    glRotatef(180,1,0,0);
    glRotatef(45,0,1,0);
        for(int i=0; i<4; i++){
            glPushMatrix();
            glColor3f(0,1,0);
            glRotatef(90*i+45,0,1,0);
            alignBall(radius);
            glPopMatrix();
        }
    glPopMatrix();

}

void drawBall(float radius){
    //glPushMatrix();
    //glTranslatef(0,radius,0);
    ball(radius);
    //glPopMatrix();
}


void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    pt points[105];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(0,0,1);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawArrow(){

    glPushMatrix();
    glTranslatef(position.x,position.y,position.z);
    pt uolmbo = direction.cross(pt(0,1,0));
    glRotatef(-90,uolmbo.x, uolmbo.y, uolmbo.z);
    glRotatef(-90,1,0,0);

    glColor3f(0,0,1);
    glLineWidth(5);
    glBegin(GL_LINES);
    {
        glVertex3f(0,0,0);
        glVertex3f(0,0,2*radius);
    }glEnd();

    glPushMatrix();
    glColor3f(0,0,1);
    glTranslatef(0,0,2*radius);
    drawCone(.04,.2,100);
    glPopMatrix();
    glPopMatrix();
}

void animateBall(){
    glPushMatrix();
    glTranslatef(position.x,position.y,position.z);
   // glRotatef((velocity*180*dt)/(pi*radius),rotatingAxis.x, rotatingAxis.y, rotatingAxis.z);
    float theta = acos(track.dot(up))*(180/pi);
    pt upore(0,1,0);
    pt lombo = upore.cross(track);
    upore = upore.matixMul(rotation(lombo.unit(),theta));
    if(abs(upore.dot(track)-1)>.0001){
        theta = -theta;
    }
    cout<<theta<<endl;
    glRotatef(theta,lombo.x,lombo.y,lombo.z);
    drawBall(radius);
    glPopMatrix();
}
void wall(float x){
    for(int i=0; i<4; i++){
        glPushMatrix();
        glRotatef(i*90,0,1,0);
        glColor3f(1,0,0);
        glBegin(GL_QUADS);
        {
            glVertex3f(-x,0,-x);
            glVertex3f(-x,0.5,-x);
            glVertex3f(x,0.5,-x);
            glVertex3f(x,0,-x);
        }
        glEnd();
        glPopMatrix();
    }
}
void display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        camera.x, camera.y, camera.z, 
        camera.x + look.x, camera.y + look.y, camera.z + look.z,
        up.x, up.y, up.z
        );

    glRotatef(angle,0,1,0);

    //axes();
    chessboard();
    wall(3);


    if(changeDirection){
        changeDirection=1-changeDirection;
        dt=Dt;
        if(abs(abs(position.x+radius)-3)<0.001 || abs(abs(position.x-radius)-3)<0.001) direction.x=-direction.x;
        if(abs(abs(position.z+radius)-3)<0.001 || abs(abs(position.z-radius)-3)<0.001) direction.z=-direction.z;
        direction.print();
    }

    for(float tt=0.001; tt<=dt; tt+=0.001){
       pt pp(0,0,0);
       if(agao==1)
            pp = position+direction*(velocity*tt);
       if(agao==-1)
            pp = position-direction*(velocity*tt);

       if(pp.x+radius>3 || pp.x-radius<-3 || pp.z+radius>3||pp.z-radius<-3){
        changeDirection=1;
        dt = tt-0.001;
       }
    }


    if(agao){
        
        if(agao==1){
            rotatingAxis = up.cross(direction);
            position = position+direction*(velocity*dt);
        }
        else{
            rotatingAxis = direction.cross(up);
            position = position-direction*(velocity*dt);
        }
        track = track.matixMul(rotation(rotatingAxis.unit(),(velocity*180*dt)/(pi*radius)));
        agao=0;
    }
    drawArrow();
    animateBall();
    

   //for(int i = 0; i < 4; i++){

       // glPushMatrix();

        // glPushMatrix();
        // glRotatef(1*cnt,0,0,1);
        // glTranslatef(.5,.5,0);

        // square(2);
        // glPopMatrix(); 

       // glPopMatrix();  

    //}
   
    //glFlush();

    // glPushMatrix();
    // glTranslatef(0,0,5);
    // glRotatef(45,1,0,0);
    // glTranslatef(0,0,-5);
    // glScalef(1,1,0.5);
    // gluCylinder(gluNewQuadric(),0.3,0.3,10,1000,1000);
    // glPopMatrix();


    // glPushMatrix();
    // glTranslatef(10,0, 10);
    // glColor3f(1,1,0);
    // gluSphere(gluNewQuadric(),
 	// 2,
 	// 100,
 	// 10);
    // glPopMatrix();

    glutSwapBuffers();
}

void init(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 1000);
    glEnable(GL_DEPTH_TEST);
    rotatingAxis = up.cross(direction);
}



// void reshapeListener(GLsizei width, GLsizei height) {  
//     if (height == 0) height = 1;                
//     GLfloat aspect = (GLfloat)width / (GLfloat)height;

//     glViewport(0, 0, width, height);
//     glMatrixMode(GL_PROJECTION);  
//     glLoadIdentity();             

//    gluPerspective(60,	1,	1,	100);
// }

void keyboard(unsigned char key, int x, int y) {

    switch (key) {
        case ' ':{
            animate=1-animate;
            break;
        }


        case 'i':{
            agao=1;
            break;
        }

        case 'k':{
            agao=-1;
            break;
        }
        case 'j':
            direction=direction.matixMul(rotation(pt(0,1,0),2*theta));
            break;

        case 'l':
            direction=direction.matixMul(rotation(pt(0,1,0),-2*theta));
            break;


        case 'a':
            angle++;
            break;

        case 'd':
            angle--;
            break;
         case '1':
            look = look.matixMul(rotation(up.unit(),theta));
			break;

        case '2':
            look = look.matixMul(rotation(up.unit(),-theta));
			break;

        case '3':{
			pt right;
            right = look.cross(up).unit();
            look = look.matixMul(rotation(right,theta));
            up = up.matixMul(rotation(right,theta));
			break;
        }

        case '4':{
			pt right;
            right = look.cross(up).unit();
            look = look.matixMul(rotation(right,-theta));
            up = up.matixMul(rotation(right,-theta));
			break;
        }

        case '5':
			up = up.matixMul(rotation(look.unit(),theta));
			break;

        case '6':
			up = up.matixMul(rotation(look.unit(),-theta));
			
			break;

        default:   
            break;
    }

    glutPostRedisplay();    
}
void specialKey(int key, int x,int y)
{
	switch(key){
		case GLUT_KEY_UP:		
			camera = camera+look.unit();
			break;
		case GLUT_KEY_DOWN:		
			camera = camera-look.unit();
			break;

		case GLUT_KEY_RIGHT:
			camera = camera+look.cross(up).unit();
			break;
		case GLUT_KEY_LEFT :
			camera = camera-look.cross(up).unit();
			break;
		case GLUT_KEY_PAGE_UP:
		    camera=camera+up;
			break;
		case GLUT_KEY_PAGE_DOWN:
            camera=camera-up;
			break;
		default:
			break;
	}
    
	glutPostRedisplay();
}


void Timer(int value){
    if(animate){
        agao=1;
        glutPostRedisplay();
    }
    glutTimerFunc(40, Timer, 0);
}

int main(int argc, char **argv)
{
    printf("Hello World\n");
    glutInit(&argc, argv);
    glutInitWindowPosition(200, 10);
    glutInitWindowSize(800, 800);
    glutCreateWindow("OpenGL Demo");
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    //glutReshapeFunc(reshapeListener);  
    init();
    glutDisplayFunc(display);
    glutSpecialFunc(specialKey);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(10, Timer, 0);
    //glutIdleFunc(display);
    glutMainLoop();

    // pt p(1,2,3), q(4,5,6);
    // p.unit().print();
    // cout<<p.len()<<' '<<p.dot(q)<<endl;
    // pt r = p+q;
    // r.print();

    //  r = p-q;
    // r.print();

    //  r = p*3;
    // r.print();

    //  r = p/3;
    // r.print();

    return 0;
}