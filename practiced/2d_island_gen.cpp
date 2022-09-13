#include <chrono>
#include <graphics.h>
#include "iostream"
#include "conio.h"
#include "thread"
#include "g_helper.h"
#include "PerlinNoise.hpp"
#include "time.h"


int main()
{
   time_t begin=time(NULL);
   int WIDTH=400;
   int HEIGHT=400;
   int win1=initwindow(WIDTH, HEIGHT,"Perlin Noise",200,200);
   setcurrentwindow(win1);
   int n=20;
   int w=(WIDTH/n),h=HEIGHT/n;
   Vec2 grid[n][n];
   int lineColor=150;
   setcolor(COLOR(lineColor,lineColor,lineColor));
   setbkcolor(COLOR(30,30,30));

   siv::PerlinNoise noise;
   float incx=0.01;
   float incy=0.01;


   for (int i = 0; i < WIDTH; i++){
      for(int j=0;j< HEIGHT;j++){
         double nx=noise.octave2D_01(i*incx,j*incx, 10);
         if(nx <= 0.7) putpixel(i,j,COLOR(40,100,196));
         else if(nx <= 0.75) putpixel(i,j,YELLOW);
         else if(nx < 1.0) putpixel(i,j,GREEN);
      }
   }
   std::cout << "Done" << std::endl;
   time_t end=time(NULL);
   printf("Seconds: %.2f",difftime(end,begin));

   getch();
   return 0;
}