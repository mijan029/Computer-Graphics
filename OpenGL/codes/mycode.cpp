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

pt camera(10, 10, 5), look(-10,-10,-5), up(0,1,0);

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

    axes();
  

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
        case 'a':
            angle++;
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