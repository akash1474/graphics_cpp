#include "iostream"
#include "cmath"
#include "graphics.h" 


void bre_line(int x1,int y1,int x2,int y2){
   if(y2 < y1) {
      int a=x2,b=y2;
      x2=x1;
      y2=y1;
      x1=a;
      y1=b;
   }
   float dy=abs(y2-y1);
   float dx=abs(x2-x1);
   float m=dy/dx;
   float p=(2*dy)-dx;
   putpixel(x1, y1, 255);
   printf("(%d,%d)\n",x1,y1);
   std::cout << m << std::endl;
   int n=std::max(dy,dx); 
   while(n>0){
      if(p<0){
         x1+=1;
         p+=(2*dy);
         printf("(%d,%d)\n",x1,y1);
         n--;
         putpixel(x1, y1, 255);
      }
      if(n==0) break;
      if(p>=0){
         x1+=1;
         y1+=1;
         p+=(2*(dy-dx));
         printf("(%d,%d)\n",x1,y1);
         n--;
         putpixel(x1, y1, 255);
      }
   }
}


void dda_line(int x1,int y1,int x2,int y2){
   float dy=y2-y1;
   float dx=x2-x1;
   float m=dy/dx;
   if(y2 < y1){
      int a=x2,b=y2;
      x2=x1;
      y2=y1;
      x1=a;
      y1=b;
   }
   std::cout << m << std::endl;
   int n=std::max(abs(dy),abs(dx));
   printf("(%d,%d)\n",x1,y1);
   putpixel(x1,y1, 255);
   float delta=m>1? (1.0/m) : m;
   float sum=m>1? delta+x1 : delta+y1 ;
   while(n>0){
      if(m<1){
         x1+=1;
         y1=round(sum);
         sum+=delta; 
      }else{
         y1+=1;
         x1=round(sum);
         sum+=delta;
      }
      printf("(%d,%d)\n",x1,y1);
      putpixel(x1,y1, 255);
      n--;
   }
}

int main()
{
   int gd=DETECT,gm;
   initgraph(&gd, &gm,NULL);
   int lineColor=150;
   setcolor(COLOR(lineColor,lineColor,lineColor));
   setbkcolor(COLOR(30,30,30));

   dda_line(200,20,400,100);
   // dda_line(100,10,25,32);
   std::cout << "Done" << std::endl;

   getch();
   return 0;
}