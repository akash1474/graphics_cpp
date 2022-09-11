#include <chrono>
#include <graphics.h>
#include <ratio>
#include <wingdi.h>
#include "iostream"
#include "conio.h"
#include "thread"
#include "g_helper.h"
#include "matrix.hpp"
#include "cmath"



int main()
{
   // int gd = DETECT,gm; initgraph(&gd,&gm,NULL);
   int WIDTH=400;
   int HEIGHT=400;
   int win1=initwindow(WIDTH, HEIGHT,"Grid",200,200);
   setcurrentwindow(win1);
   int n=20;
   int w=(WIDTH/n),h=HEIGHT/n;
   Vec2 grid[n][n];
   int lineColor=150;
   // setcolor(COLOR(lineColor,lineColor,lineColor));
   // setbkcolor(COLOR(30,30,30));
   float angle=0.0;

   Matrix cube(3,8,{
      {50,50,-50,-50,50,50,-50,-50},
      {50,-50,50,-50,50,-50,50,-50},
      {50,50,50,50,-50,-50,-50,-50}
   });





   double dis= 0.8;
   Matrix proj(2,3,{
      {dis,0,0},
      {0,dis,0}
   });

   while(true){
      Matrix rotateX(3,3,{
         {1,0,0},
         {0,cos(angle),-sin(angle)},
         {0,sin(angle),cos(angle)}
      });

      Matrix rotateY(3,3,{
         {cos(angle),0,sin(angle)},
         {0,1,0},
         {-sin(angle),0,cos(angle)}
      });

      Matrix rotateZ(3,3,{
         {cos(angle),-sin(angle),0},
         {sin(angle),cos(angle),0},
         {0,0,1},
      });
      // break;
      Matrix rotate=Matrix::dot(rotateX,cube); //3x8
      rotate=Matrix::dot(rotateY,rotate); //3x8
      rotate=Matrix::dot(rotateZ,rotate); //3x8
      Matrix projected=Matrix::dot(proj,rotate); //2x8
      projected+200;
      for(int i=0;i<projected.cols;i++){
         point(projected.data[0][i],projected.data[1][i]);
      }

      // int p=1;
      for(int i=0;i<7;i++){
         if(i%2==0) line(projected.data[0][i],projected.data[1][i],projected.data[0][i+1],projected.data[1][i+1]);
         if(i < 4) line(projected.data[0][i],projected.data[1][i],projected.data[0][i+4],projected.data[1][i+4]);
         if(i==0 ||i==1 || i==4 || i==5) line(projected.data[0][i],projected.data[1][i],projected.data[0][i+2],projected.data[1][i+2]);
      }


      angle+=0.05;
      std::this_thread::sleep_for(std::chrono::milliseconds(16));
      cleardevice();
   }
 

   getch();
   closegraph();
   return 0;
}