#include <chrono>
#include <graphics.h>
#include "iostream"
#include "conio.h"
#include "thread"

struct Vec2{
   int x;
   int y;
   // Vec2(float a,float b):x{a},y{b}{}
   void operator+(int x){
      this->x+=x;
      this->y+=x;
   }
   void operator-(int x){
      this->x-=x;
      this->y-=x;
   }
};

void rect(const Vec2& p1,const Vec2& p2,const bool& filled=true){
   if(filled){
      bar(p1.x, p1.y,p2.x, p2.y);
   }else{
      rectangle(p1.x, p1.y,p2.x, p2.y);
   }
}

struct Point{
   Vec2 p;
   void render(){
      bar(p.x,p.y,p.x+20,p.y+20);
   }
};


void b_line(Vec2& p1,Vec2& p2){
   Point pt;
   pt.p=p1;


   int dx=abs(p1.x-p2.x);
   int dy=abs(p1.y-p2.y);
   int e=(2*dy)-dx;
   int i=1;
   do{
      pt.render();
      while(e>=0){
         pt.p.y+=20;
         e+=2*dx;
         pt.render();
         std::this_thread::sleep_for(std::chrono::milliseconds(500));
      } 
      pt.p.x+=20;
      e+=2*(dy-dx);
      i++;
   }while(i<=dx);
}

void bresenham(int x1, int y1, int x2, int y2)
{
   int m_new = 2 * (y2 - y1);
   int slope_error_new = m_new - (x2 - x1);
   for (int x = x1, y = y1; x <= x2; x++)
   {
      std::cout << "(" << x << "," << y << ")\n";
   
      // Add slope to increment angle formed
      slope_error_new += m_new;
   
      // Slope error reached limit, time to
      // increment y and update slope error.
      if (slope_error_new >= 0)
      {
         y++;
         slope_error_new  -= 2 * (x2 - x1);
      }
   }
}

void plotPixel(int x1, int y1, int x2, int y2, int decide)
{
    //pk is initial decision making parameter
    //Note:x1&y1,x2&y2, dx&dy values are interchanged
    //and passed in plotPixel function so
    //it can handle both cases when m>1 & m<1
      int dx=abs(x2-x1);
      int dy=abs(y2-y1);
    int pk = 2 * dy - dx;
    for (int i = 0; i <= dx; i++)
    {
        std::cout << x1 << "," << y1 << std::endl;
        //checking either to decrement or increment the value
        //if we have to plot from (0,100) to (100,0)
        x1 < x2 ? x1++ : x1--;
        if (pk < 0)
        {
            //decision value will decide to plot
            //either  x1 or y1 in x's position
            if (decide == 0)
            {
               putpixel(x1, y1, RED);
            }
            else
            {
                //(y1,x1) is passed in xt
               putpixel(y1, x1, YELLOW);
                pk = pk + 2 * dy;
            }
        }
        else
        {
            y1 < y2 ? y1++ : y1--;
            if (decide == 0)
            {
 
                putpixel(x1, y1, RED);
            }
            else
            {
               putpixel(y1, x1, YELLOW);
            }
            pk = pk + 2 * dy - 2 * dx;
        }
    }
}

void b_circle(Vec2& p1,int r){
   Point pt;
   pt.p.x=0;
   pt.p.y=p1.x+(20*r);

   float dp=3-(2*r); 
   while(pt.p.x <= pt.p.y){

      if(dp<=0){
         dp+=(4*pt.p.x)+6;
      }
   }
}

int main()
{
   // int gd = DETECT,gm;
   // initgraph(&gd,&gm,NULL);

   int WIDTH=400;
   int HEIGHT=400;
   int win1=initwindow(WIDTH, HEIGHT,"Grid",200,200);
   setcurrentwindow(win1);
   int n=20;
   int w=(WIDTH/n),h=HEIGHT/n;
   Vec2 grid[n][n];
   int lineColor=150;
   setcolor(COLOR(lineColor,lineColor,lineColor));
   setbkcolor(COLOR(30,30,30));

   for(int i=0;i<n;i++){
      for(int j=0;j<n;j++){
         Vec2 v;
         v.x=i*w;
         v.y=i*h;
         grid[i][j]=v;
      }
   }

   for(int i=1;i<n;i++){
      line(0,i*h,WIDTH,i*h);
   }
   for(int i=1;i<n;i++){
      line(i*w,0,i*w,HEIGHT);
   }

   // rect(grid[5][5],grid[6][6]);
   // b_line(grid[1][1],grid[10][20]);
   plotPixel(10,10,500,200,1);


   getch();
   // closegraph();
   return 0;
}