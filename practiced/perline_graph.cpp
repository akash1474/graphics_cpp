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
   int WIDTH=400;
   int HEIGHT=400;
   int FPS=60;
   int win1=initwindow(WIDTH, HEIGHT,"Perlin Noise",200,200);
   setcurrentwindow(win1);
   int n=20;
   int w=(WIDTH/n),h=HEIGHT/n;
   Vec2 grid[n][n];
   int lineColor=150;
   setcolor(COLOR(lineColor,lineColor,lineColor));
   setbkcolor(COLOR(30,30,30));

   siv::PerlinNoise noise;
   float inc=0.01;
   float start=0.0;

   while(true){
      float xoff=start;
      if(ismouseclick(WM_LBUTTONDOWN)){
         inc-=0.01;
      }
      if(ismouseclick(WM_RBUTTONDOWN)){
         inc+=0.01;
      }
      clearmouseclick(WM_LBUTTONDOWN);
      clearmouseclick(WM_RBUTTONDOWN);
      Vec2 prev;
      bool isFirst=true;
      for (int x = 0; x < 50; x++){
         double ny=noise.octave1D_01(xoff,4);
         if(isFirst){
            putpixel(x,ny*HEIGHT,WHITE);
            isFirst=false;
         }else{
            line(prev.x,prev.y,x*8,ny*HEIGHT);
         }
         prev.x=x*8;
         prev.y=ny*HEIGHT;
         xoff+=inc;
      }
      start+=inc;
      std::this_thread::sleep_for(std::chrono::milliseconds(1000/FPS));
      cleardevice();
   }
   std::cout << "Done" << std::endl;

   getch();
   return 0;
}