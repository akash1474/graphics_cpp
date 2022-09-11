#ifndef G_HELPER_H
#define G_HELPER_H

#include "graphics.h"

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

void point(int x,int y,int w=5){
	setfillstyle(SOLID_FILL,WHITE);
	circle(x,y,w);
	floodfill(x,y,WHITE);	
}



#endif
