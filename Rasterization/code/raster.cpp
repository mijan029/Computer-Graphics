#include <bits/stdc++.h>
#include "bitmap_image.hpp"

using namespace std;

// Line 421, 458, 467 cganged something

class pt{
    public:
    double x, y, z, w;
    pt(){
        this->x = 0;
        this->y = 0;
        this->z = 0;
        this->w = 1;
    }
    pt(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = 1;
    }
    pt(float x, float y, float z, float w){
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
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
        double l = len();
        return pt(x/l, y/l, z/l);
    }
    float dot(pt p){
        return x*p.x + y*p.y + z*p.z;
    }

    pt cross(pt p){
        return pt(y*p.z-z*p.y, z*p.x-x*p.z, x*p.y-y*p.x);
    }

    friend istream& operator>>(istream& is, pt& p){
        is >> p.x >> p.y >> p.z;
        p.w = 1.0;
        return is;
    }

    friend ofstream& operator<<(ofstream& os, pt& p){
        os << fixed << setprecision(7);
        os << p.x << " " << p.y << " " << p.z << endl;
        return os;
    }

    void normal(){
        x/=w; y/=w; z/=w; w=1;
    }

    pt matixMul(double mat[4][4]){
        pt p(
            mat[0][0]*x+mat[0][1]*y+mat[0][2]*z+mat[0][3]*w,
            mat[1][0]*x+mat[1][1]*y+mat[1][2]*z+mat[1][3]*w,
            mat[2][0]*x+mat[2][1]*y+mat[2][2]*z+mat[2][3]*w,
            mat[3][0]*x+mat[3][1]*y+mat[3][2]*z+mat[3][3]*w

        );
        if(p.w!=0){
            p.x/=p.w;
            p.y/=p.w;
            p.z/=p.w;
            p.w = 1;
        }

        return p;
    }

    void print(){
        cout<<x<<' '<<y<<' '<<z<<endl;
    }
};

static unsigned long int g_seed = 1;
inline int randomm()
{
g_seed = (214013 * g_seed + 2531011);
return (g_seed >> 16) & 0x7FFF;
}


class Color{
    public:

    float r, g, b;
    Color(){
        r=g=b=0;
    }
    void set(){
        r = randomm();
        g = randomm();
        b = randomm();
    }
};

class triangle{
    public:
    pt a, b, c;
    Color color;
    triangle(){
        c = pt();
        b = pt();
        a = pt();
    }
    triangle(pt a, pt b, pt c){
        this->a = a;
        this->b = b;
        this->c = c;
    }
    void coloring(){
        color.set();
    }
    void OrderByX(){
        if(a.x>b.x) swap(a,b);
        if(c.x<a.x) swap(a,c);
        if(c.x<b.x) swap(b,c);
    }

    void OrderByZ(){
        if(a.z>b.z) swap(a,b);
        if(c.z<a.z) swap(a,c);
        if(c.z<b.z) swap(b,c);
    }

    friend istream& operator>>(istream& is, triangle& p){
        is >> p.a >> p.b >> p.c;
        return is;
    }

    friend ofstream& operator<<(ofstream& os, triangle& p){
        os << p.a << p.b << p.c << endl;
        return os;
    }


};


class matrix{
    public:
    double arr[4][4];
    matrix(){
        memset(arr,0,sizeof(arr));
    }
    matrix operator*(matrix m){
        matrix ret;
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                double sum=0;
                for(int k=0; k<4; k++){
                    sum+=arr[i][k]*m.arr[k][j];
                }
                ret.arr[i][j]=sum;
            }
        }
        return ret;
    }
    void createScaleMat(pt a){
        arr[0][0] = a.x;
        arr[1][1] = a.y;
        arr[2][2] = a.z;
        arr[3][3] = 1;
    }

    void createTransMat(pt a){
        arr[0][0] = 1;
        arr[1][1] = 1;
        arr[2][2] = 1;
        arr[3][3] = 1;
        arr[0][3] = a.x;
        arr[1][3] = a.y;
        arr[2][3] = a.z;
    }

    pt R(pt x, pt a, double theta){
        //𝑐𝑜𝑠𝜃 𝑥⃗ + (1 − 𝑐𝑜𝑠𝜃) (𝑎⃗̇. 𝑥⃗)𝑎⃗ + 𝑠𝑖𝑛𝜃 (𝑎⃗ × 𝑥⃗)
        theta = theta*acos(-1.0)/180;
        return x*cos(theta) + a*((1-cos(theta))*(a.dot(x))) + a.cross(x)*sin(theta);
    }

    void createRotationMat(pt a, double theta){
        // arr[0][0] = cos(theta) + (1-cos(theta))*a.x*a.x ;
        // arr[0][1] =  (1-cos(theta))*a.x*a.y + sin(theta)*(-a.z);
        // arr[0][2] =  (1-cos(theta))*a.x*a.z + sin(theta)*a.y;
        // arr[1][0] =  (1-cos(theta))*a.x*a.y + sin(theta)*a.z;
        // arr[1][1] = cos(theta) + (1-cos(theta))*a.y*a.y ;
        // arr[1][2] =  (1-cos(theta))*a.y*a.z + sin(theta)*(-a.x);
        // arr[2][0] =  (1-cos(theta))*a.x*a.z + sin(theta)*(-a.y);
        // arr[2][1] =  (1-cos(theta))*a.z*a.y + sin(theta)*a.x;
        // arr[2][2] = cos(theta) + (1-cos(theta))*a.z*a.z ;
        // arr[3][3] = 1;

        a.unit();
        pt c1=R(pt(1,0,0),a,theta);
        pt c2=R(pt(0,1,0),a,theta);
        pt c3=R(pt(0,0,1),a,theta);

        //c1.print(); c2.print(); c3.print();

        arr[0][0] =  c1.x;
        arr[0][1] =  c2.x;
        arr[0][2] =  c3.x;
        arr[1][0] =  c1.y;
        arr[1][1] =  c2.y;
        arr[1][2] =  c3.y;
        arr[2][0] =  c1.z;
        arr[2][1] =  c2.z;
        arr[2][2] =  c3.z;
        arr[3][3] = 1;
    }

    void createIdentity(){
        arr[0][0]=arr[1][1]=arr[2][2]=arr[3][3]=1;
    }

    void print(){
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                cout<<arr[i][j]<<' ';
            }
            cout<<endl;
        }
    }
    
};

matrix viewTransform(pt eye, pt look, pt up){
    pt l = look-eye;
    l = l.unit();
    pt r = l.cross(up);
    r = r.unit();
    pt u = r.cross(l);

    matrix eyepos, mat;
    eyepos.createTransMat(pt(-eye.x,-eye.y,-eye.z));
    mat.arr[0][0] = r.x;
    mat.arr[0][1] = r.y;
    mat.arr[0][2] = r.z;
    mat.arr[1][0] = u.x;
    mat.arr[1][1] = u.y;
    mat.arr[1][2] = u.z;
    mat.arr[2][0] = -l.x;
    mat.arr[2][1] = -l.y;
    mat.arr[2][2] = -l.z;
    mat.arr[3][3] = 1;

    mat = mat*eyepos;

    return mat;
}

matrix projectTransformation(double fovY, double aspectRatio, double near, double far){
    double fovX = fovY * aspectRatio;
    fovX = (fovX*acos(-1.0))/180;
    fovY = (fovY*acos(-1.0))/180;
    double t = near * tan(fovY/2);
    double r = near * tan(fovX/2);

    matrix mat;
    mat.arr[0][0]=near/r;
    mat.arr[1][1] = near/t;
    mat.arr[2][2] = -(far+near)/(far-near);
    mat.arr[3][2] = -1;
    mat.arr[2][3] = -(2*far*near)/(far-near);

    return mat;
}

int main(){
    ifstream finn("scene.txt");
    ofstream foutt("stage1.txt");

    pt eye, look, up;
    double fovY, aspectRatio, near, far;

    finn>>eye>>look>>up;
    finn>>fovY>>aspectRatio>>near>>far;

    matrix mat;
    mat.createIdentity();
    stack<matrix> st;

    string s;

    //stage 1
    while(finn>>s){
       pt p;
       matrix mm;
       if(s=="scale"){
            finn>>p;
            mm.createScaleMat(p);
            mat = mat*mm;
       }else if(s=="translate"){
            finn>>p;
            mm.createTransMat(p);
            mat = mat*mm;
            
       }else if(s=="rotate"){
            double theta;
            finn>>theta>>p;
            mm.createRotationMat(p,theta);
            //mm.print();
            mat = mat*mm;
            //mat.print();
       }else if(s=="triangle"){
            triangle t;
            finn>>t;
            t.a = t.a.matixMul(mat.arr);
            t.b = t.b.matixMul(mat.arr);
            t.c = t.c.matixMul(mat.arr);

            foutt<<t;

       }else if(s == "push"){
            st.push(mat);
       }else if(s == "pop"){
            mat = st.top();
            st.pop();
       }else if(s=="end"){
            break;
       }
    }


    finn.close();
    foutt.close();
    finn.open("stage1.txt");
    foutt.open("stage2.txt");
    //stage2
    triangle t;
    while (finn>>t)
    {
        t.a = t.a.matixMul(viewTransform(eye,look,up).arr);
        t.b = t.b.matixMul(viewTransform(eye,look,up).arr);
        t.c = t.c.matixMul(viewTransform(eye,look,up).arr);

        foutt<<t;

    }
    


    finn.close();
    foutt.close();
    finn.open("stage2.txt");
    foutt.open("stage3.txt");
    //stage3
    while (finn>>t)
    {
        t.a = t.a.matixMul(projectTransformation(fovY, aspectRatio, near, far).arr);
        t.b = t.b.matixMul(projectTransformation(fovY, aspectRatio, near, far).arr);
        t.c = t.c.matixMul(projectTransformation(fovY, aspectRatio, near, far).arr);
        t.a.normal(); t.b.normal(); t.c.normal();

        foutt<<t;

    }


    //stage4
    finn.close();
    foutt.close();
    ifstream finnn("config.txt");
    finn.open("stage3.txt");
    foutt.open("z-buffer.txt");

    double Screen_Height, Screen_Width;
    finnn>>Screen_Width>>Screen_Height;

    double left_limit=-1, right_limit=1, top_limit=1, bottom_limit=-1, z_min=-1, z_max=1;
    double dx = (right_limit - left_limit) / Screen_Width;
    double dy = (top_limit - bottom_limit) / Screen_Height;

    bitmap_image image(Screen_Width, Screen_Height);
    image.set_all_channels(0, 0, 0);

    double Top_Y = top_limit - dy/2;
    double Left_X = left_limit + dx/2;

    double** z_buffer;
    z_buffer = new double*[(int)Screen_Height];
    for(int i=0; i<Screen_Height; i++){
        z_buffer[i] = new double[(int)Screen_Width];
        for(int j=0; j<Screen_Width; j++){
            z_buffer[i][j] = z_max;
        }
    }

    int cnt=0;
    while (finn>>t)
    { cnt++;
        
       
        double y1 = min(Top_Y,max(t.a.y,max(t.b.y,t.c.y)));
        double y2 = max(bottom_limit,min(t.a.y,min(t.b.y,t.c.y)));

        t.coloring(); //cout<<cnt<<"y: "<<y1<<' '<<y2<<endl;
        
        for(double y = y1; y>=y2+dy/2; y-=dy){
            t.OrderByX(); 
            double x1=1, x2=-1;
            if(y<=max(t.a.y,t.b.y) && y>=min(t.a.y,t.b.y)){
                if(t.a.y != t.b.y) x1 = t.a.x + (y-t.a.y)*(t.a.x-t.b.x)/(t.a.y-t.b.y);
                if(y<=max(t.c.y,t.b.y) && y>=min(t.c.y,t.b.y)){
                    if(t.c.y != t.b.y) x2 = t.b.x + (y-t.b.y)*(t.b.x-t.c.x)/(t.b.y-t.c.y);
                }else{
                    if(t.a.y != t.c.y) x2 = t.a.x + (y-t.a.y)*(t.a.x-t.c.x)/(t.a.y-t.c.y);
                }
            }else{
                if(t.a.y != t.c.y) x1 = t.a.x + (y-t.a.y)*(t.a.x-t.c.x)/(t.a.y-t.c.y);
                if(y<=max(t.c.y,t.b.y) && y>=min(t.c.y,t.b.y)){
                    if(t.c.y != t.b.y) x2 = t.b.x + (y-t.b.y)*(t.b.x-t.c.x)/(t.b.y-t.c.y);
                }else{
                    if(t.a.y != t.b.y) x2 = t.a.x + (y-t.a.y)*(t.a.x-t.b.x)/(t.a.y-t.b.y);
                }
            }

            t.OrderByZ();
            double z1=1, z2=-1;
            if(y<=max(t.a.y,t.b.y) && y>=min(t.a.y,t.b.y)){
                if(t.a.y != t.b.y) z1 = t.a.z + (y-t.a.y)*(t.a.z-t.b.z)/(t.a.y-t.b.y);
                if(y<=max(t.c.y,t.b.y) && y>=min(t.c.y,t.b.y)){
                    if(t.c.y != t.b.y) z2 = t.b.z + (y-t.b.y)*(t.b.z-t.c.z)/(t.b.y-t.c.y);
                }else{
                    if(t.a.y != t.c.y) z2 = t.a.z + (y-t.a.y)*(t.a.z-t.c.z)/(t.a.y-t.c.y);
                }
            }else{
                if(t.a.y != t.c.y) z1 = t.a.z + (y-t.a.y)*(t.a.z-t.c.z)/(t.a.y-t.c.y);
                if(y<=max(t.c.y,t.b.y) && y>=min(t.c.y,t.b.y)){
                    if(t.c.y != t.b.y) z2 = t.b.z + (y-t.b.y)*(t.b.z-t.c.z)/(t.b.y-t.c.y);
                }else{
                    if(t.a.y != t.b.y) z2 = t.a.z + (y-t.a.y)*(t.a.z-t.b.z)/(t.a.y-t.b.y);
                }
            }

            x1 = max(x1,Left_X);
            x2 = min(x2,right_limit);

            pt pp(x1,y,z2);
            if((pp-t.a).cross(pp-t.b).dot(pp-t.c)<1e-6) swap(z1,z2);


            for(double x = x1; x<=x2-dx/2; x+=dx){
                int i = (Top_Y-y)/dy;
                int j = (x-Left_X)/dx;
                //if(cnt==1 && i<=120) cout<<cnt<<' '<<i<<' '<<j<<endl;
                if(x1==x2){ }
                else{
                    double z = z1 + (x-x1)*(z1-z2)/(x1-x2);
                    //if(cnt==1) cout<<z<<' '<<cnt<<endl;
                    if(z<z_buffer[i][j] && z>z_min){
                        z_buffer[i][j]=z;
                        image.set_pixel(j, i, t.color.r, t.color.g , t.color.b);

                    }
                }
            }
        }
    }
    image.save_image("out.bmp");

    for(int i=0; i<Screen_Height; i++){
        for(int j=0; j<Screen_Width; j++){
            if(z_buffer[i][j]<z_max){
                foutt<<setprecision(6)<<fixed<<z_buffer[i][j]<<"\t";
            }
        }
            foutt<<endl;
    }

    finn.close();
    finnn.close();
    foutt.close();
    for(int i=0; i<Screen_Height; i++){
        delete z_buffer[i];
    }
    delete z_buffer;

    return 0;
}


