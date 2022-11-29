#include "iostream"
#include "graphics.h"
#include "math.h"

class CircumscribedCircle{
float a,b,c,r,cx,cy,cr;
public:
int x1, y1, x2, y2, x3, y3;
CircumscribedCircle(int x1,int y1,int x2,int y2,int x3,int y3){
this->x1=x1;
this->x2=x2;
this->x3=x3;
this->y1=y1;
this->y2=y2;
this->y3=y3;
}
void plot_symm(int x,int y,int xc, int yc){
putpixel(xc+x, yc+y, RED);
putpixel(xc-x, yc+y, RED);
putpixel(xc+x, yc-y, RED);
putpixel(xc-x, yc-y, RED);
putpixel(xc+y, yc+x, RED);
putpixel(xc-y, yc+x, RED);
putpixel(xc+y, yc-x, RED);
putpixel(xc-y, yc-x, RED);
}
void draw(){
triangle();
getsides();
getCenterCordinates();
getRadius();
getCurcumRadius();
circle(cx,cy,r);
circle(cx,cy,cr);
}

void line(int x0,int y0,int x1,int y1){
int dx = x1 - x0;
int dy = y1 - y0;
int step;
if (abs(dx) > abs(dy))
step = abs(dx);
else
step = abs(dy);
float x_incr = (float)dx / step;
float y_incr = (float)dy / step;
float x = x0;
float y = y0;

for (int i = 0; i < step; i++) {
putpixel(round(x), round(y), WHITE);
std::cout << round(x) << " " << round(y) << "\n";
x += x_incr;
y += y_incr;
}
}
float distance(int x1,int y1,int x2,int y2){
return sqrt(pow(x2-x1,2)+pow(y2-y1,2));
}
void triangle(){
line(x1,y1,x2,y2);
line(x2,y2,x3,y3);
line(x1,y1,x3,y3);
}
void getsides(){
a=distance(x1,y1,x2,y2);
b=distance(x2,y2,x3,y3);
c=distance(x1,y1,x3,y3);
}

void getCenterCordinates(){
cx=((a*x1)+(b*x2)+(c*x3))/(a+b+c);
cy=((a*y1)+(b*y2)+(c*y3))/(a+b+c);
}
void getRadius(){
int s=(a+b+c)/2;
int area=sqrt(s*(s-a)*(s-b)*(s-c));
r=area/s;
}
void getCurcumRadius(){
cr=distance(cx,cy,x1,y1);
}
void circle(int xx,int yy,int r){
int x=0,y=r;
int d=3-(2*r);
while(x<=y){
std::cout << "(" << xx+x << "," << yy+y << ")" << std::endl;
plot_symm(x,y,xx,yy);
if(d<0){
d+=4*x+6;
x+=1;
}else{
d+=(4*(x-y))+10;
x+=1;
y-=1;
}

}
}
};
int main(){
int x1,y1,x2,y2,x3,y3;
std::cout << "x1: ";
std::cin >> x1;
std::cout << "y1: ";
std::cin >> y1;
std::cout << "x2: ";
std::cin >> x2;
std::cout << "y2: ";
std::cin >> y2;
std::cout << "x3: ";
std::cin >> x3;
std::cout << "y3: ";
std::cin >> y3;
// CircumscribedCircle cc(200,20,100,180,300,180);
CircumscribedCircle cc(x1,y1,x2,y2,x3,y3);
int gd=DETECT,gm;
initgraph(&gd,&gm,NULL);
cc.draw();
getch();
closegraph();
return 0;
}