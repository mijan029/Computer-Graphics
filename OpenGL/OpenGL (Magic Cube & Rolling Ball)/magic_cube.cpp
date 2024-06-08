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
float scale = 0;
float precn = 1e-8;
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

pt camera(3, 3,2), look(-3,-3,-2), up(0,1,0);

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

void drawTriangle(pt newx, pt newy, pt newz){
    glBegin(GL_TRIANGLES);{
		glVertex3f(newx.x,newx.y,newx.z);
		glVertex3f(newy.x,newy.y,newy.z);
		glVertex3f(newz.x,newz.y,newz.z);
	}glEnd();
}

void triangles(float m1){
    float m2 = sqrt(2.0/3.0);
    m1*=m2;
    pt x(1,0,0), y(0,1,0), z(0,0,1);
    pt com(1.0/3,1.0/3,1.0/3);
    pt newx = x + (com-x)*(m1/(m2));
    pt newy = y + (com-y)*(m1/(m2));
    pt newz = z + (com-z)*(m1/(m2));


    for(int i=0; i<4; i++){
        glColor3f(i%2, (i+1)%2, 1);
        glPushMatrix();
        glRotatef(i*90,0,1,0);
        drawTriangle(newx, newy, newz);
        glPopMatrix();

        glColor3f((i+1)%2, i%2, 1);
        glPushMatrix();
        glRotatef(i*90,0,1,0);
        glRotatef(180,1,0,1);
        drawTriangle(newx, newy, newz);
        glPopMatrix();
    }

}
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
void sphere(pt p, float radius){

    glPushMatrix();
    glTranslatef(p.x,p.y,p.z);
    gluSphere(gluNewQuadric(),radius,1000,1000);
    //drawSphere(radius,100,100);
    glPopMatrix();

}

void spheres(float scale){
    if(scale>=1) scale = .99;
    float radius = scale / sqrt(3);
    float down = scale;

    glColor3f(1,0,0);
    sphere(pt(0,1-down,0),radius);
    sphere(pt(0,-1+down,0),radius);

    glColor3f(0,1,0);
    sphere(pt(0,0,1-down),radius);
    sphere(pt(0,0,-1+down),radius);

    glColor3f(0,0,1);
    sphere(pt(1-down,0,0),radius);
    sphere(pt(-1+down,0,0),radius);

}

void cylinder(float scale){
    float radius = scale/sqrt(3.0);
    glPushMatrix();
    glTranslatef(0,0,1);
    glRotatef(45,1,0,0);
    glTranslatef(0,0,-1);
    glTranslatef(0,-scale/sqrt(2.0),1-sqrt(2.0)+scale/sqrt(2.0));
    glScalef(1,1,1-scale);
    gluCylinder(gluNewQuadric(),radius,radius,sqrt(2.0),1000,1000);
    glPopMatrix();
}

void cylinders(float scale){
    glColor3f(1,1,0);

    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            glPushMatrix();
            glRotatef(j*90,i%2,0,(i+1)%2);
            glRotatef(i*90,0,1,0);
            cylinder(scale);
            glPopMatrix();
        }
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
  
    triangles(scale);
    cylinders(scale);
    spheres(scale);
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
 

    glutSwapBuffers();
}

void init(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 1000);
    glEnable(GL_DEPTH_TEST);
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
        case 'd':
            angle--;
            break;
         case ',':
            scale=max(scale-0.025,0.0);
            break;
        case '.':
            scale=min(1.0,scale+0.025);
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