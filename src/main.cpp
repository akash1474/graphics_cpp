#include "iostream"
#include "graphics.h"
#include <algorithm>
#include <deque>
#include <iomanip>
#include <sstream>
#include <stdint.h>
#include <stdlib.h>
#include "math.h"


struct Vec2{
    int x=0,y=0;
    int width,height;
};

struct Window{
    int width,height;
};

int random(int from, int to){
    return rand() % (to - from + 1) + from;
}

inline float transitionFn(float& x){
    return (float)(x == 1 ? 1 : 1 - pow(2, -10 * x));   
};

inline void rect(int x,int y,int x_max,int y_max){
    for(int i=x;i<=x_max;i++){
        line(i,y,i,y_max);
    }
}


struct Car{
    Vec2 pos;
    int width=50,height=90;
    Vec2 velocity;
    int arr[14];

    void draw(){
        // bar(pos.x,pos.y,pos.x+width,pos.y+height);
        // floodfill(pos.x+10,pos.y+10,WHITE);
      arr[0]=pos.x;
      arr[1]=pos.y+10;
      arr[2]=pos.x+15;
      arr[3]=pos.y;
      arr[4]=pos.x+35;
      arr[5]=pos.y;
      arr[6]=pos.x+width;
      arr[7]=pos.y+10;
      arr[8]=pos.x+width;
      arr[9]=pos.y+height;
      arr[10]=pos.x;
      arr[11]=pos.y+height;
      arr[12]=pos.x;
      arr[13]=pos.y+10;
      setfillstyle(SOLID_FILL,WHITE);
      fillpoly(7, arr);
      setcolor(BLACK);
      rect(pos.x+10,pos.y+50,pos.x+5,pos.y+height);
    }
};

struct Boundary{
    Vec2 min;
    Vec2 max;
};


inline void centered_text(const Window& win,const char* text,int y){
    outtextxy((win.width-textwidth((char*)text))/2, y,(char*)text);
}

enum class State{
    MENU,
    PLAYING,
    ENDED
};


struct Object{
    Vec2 pos;
    int width=50,height=90;
    int arr[14];
    int fill=1;

    void update(const int& speed){
        pos.y+=speed;
    }

    void draw(){
      arr[0]=pos.x;
      arr[1]=pos.y+10;
      arr[2]=pos.x+15;
      arr[3]=pos.y;
      arr[4]=pos.x+35;
      arr[5]=pos.y;
      arr[6]=pos.x+width;
      arr[7]=pos.y+10;
      arr[8]=pos.x+width;
      arr[9]=pos.y+height;
      arr[10]=pos.x;
      arr[11]=pos.y+height;
      arr[12]=pos.x;
      arr[13]=pos.y+10;
      fillpoly(7, arr);
    }
};


struct Objects{
    std::deque<Object> objects;
    int speed=0;
    void push(Object x){
         objects.push_front(x);
         if(objects.size()>7) objects.pop_back();
    }

    void update(const int& y_max){
        bool pop_back=false;
        std::for_each(objects.begin(), objects.end(),[&](Object& obj){
            if((obj.pos.y-(obj.height*3))> y_max) pop_back=true;
            obj.update(speed);
        });

        if(pop_back) objects.pop_back();
    }

    void render(){
        std::for_each(objects.begin(), objects.end(),[&](Object& obj){
            setfillstyle(SOLID_FILL,obj.fill);
            obj.draw();
        });
    }

    bool detect_collision(const Car& car){
        bool collision=false;
        std::for_each(objects.begin(), objects.end(),[&](Object& obj){
            bool c1=obj.pos.x < car.pos.x+car.width;
            bool c2=obj.pos.x+obj.width > car.pos.x;
            bool c3=obj.pos.y < car.pos.y+car.height;
            bool c4=obj.pos.y+obj.height > car.pos.y;
            if(c1 && c2 && c3 && c4) collision=true;
        });
        return collision;
    }
};



//Drawing the animated Lanes moving down
struct Lane{
    Vec2 pos;
    int width=7,height=50;

    void render(){
        setcolor(WHITE);
        rect(pos.x-(width/2),pos.y,pos.x+width-(width/2),pos.y+height);
    }

    void update(const int speed){
        pos.y+=speed;
    }
};

struct Lanes{
    std::deque<Lane> lanes;
    int speed=1;

    void push(Lane& lane){
        lanes.push_front(lane);
        if(lanes.size()>18) lanes.pop_back();
    }

    void update(int y_max){
        std::for_each(lanes.begin(), lanes.end(),[&](Lane& lane){
            if((lane.pos.y-(lane.height*2))> y_max) lane.pos.y=-2*lane.height;
            lane.update(speed);
        });
    }

    void init_lanes(int m_LaneCount,int laneX1,int laneX2){
        int x=-180; 
        for(int i=0;i<m_LaneCount;i++){
            Lane lane1;
            Lane lane2;
            lane1.pos.x=laneX1;
            lane2.pos.x=laneX2;
            lane1.pos.y=x;
            lane2.pos.y=x;
            lanes.push_front(lane1);
            lanes.push_front(lane2);
            x+=90;
        }
    }


    void render(){
        std::for_each(lanes.begin(), lanes.end(),[&](Lane& lane){
            lane.render();
        });
    }

};

class GameEngine{
    Car m_Car;
    Window m_Window;
    Boundary m_Boundary;
    Objects m_Objects;
    Lanes m_Lanes;
    // Shrubs shrubs;
    int m_LaneWidth;
    int m_SideWidth=40;
    int m_SpawnDistance=20;
    int m_Count=0;
    int m_Page=0;
    State m_GameState;
    bool m_GameEnded=false;
    int m_StripDistance=50;
    int m_Fps=60;
    bool m_DetectCollision=true;
    uint16_t m_Score=0;
    unsigned int m_Incrementor=1;
public:
    GameEngine(){
        m_GameState=State::MENU;
        get_window_dimensions();
        init_boundary();
        init_car();
        m_LaneWidth=(m_Window.width-(2*m_SideWidth))/3;
        m_Objects.speed=10;
        init_object();
        m_Lanes.init_lanes(18, m_Boundary.min.x+m_LaneWidth,m_Boundary.min.x+(m_LaneWidth*2));
        m_Lanes.speed=30;
        // shrubs.pos.x=0;
        // shrubs.pos.y=0;
    }


    void game_restart(){
        m_Score=0;
        m_Incrementor=1;
        m_Objects.objects.clear();
        m_Objects.speed=10;
        init_car();
        init_object();
    }

    void set_fps(const int& fps){
        m_Fps=fps;
    }


    void init_car(){
        m_Car.velocity.x=25;
        m_Car.velocity.y=25;
        m_Car.pos.x=(m_Window.width-m_Car.width)/2;
        m_Car.pos.y=m_Boundary.max.y-m_Car.height;
    }

    void init_object(){
        m_Count=0;
        m_Objects.push(generate_object());
    }

    void spawn_object(){
        if(m_Count%m_SpawnDistance==0) m_Objects.push(generate_object());
        m_Count++;
    }

    Object generate_object(){
        Object obj;
        obj.pos.y=-obj.height;
        obj.fill=random(1,12);
        int offset=m_Boundary.min.x-((m_LaneWidth+obj.width)/2);
        switch(random(0,2)){
            case 0:
                obj.pos.x=m_LaneWidth+offset;
                break;
            case 1:
                obj.pos.x=(m_LaneWidth*2)+offset;
                break;
            case 2:
                obj.pos.x=(m_LaneWidth*3)+offset;
                break;

        }
        return obj;
    }

    void init_boundary(){
        m_Boundary.max.x=m_Window.width-m_SideWidth;
        m_Boundary.max.y=m_Window.height-m_SideWidth;
        m_Boundary.min.x=m_SideWidth;
        m_Boundary.min.y=m_SideWidth;
    }


    void get_window_dimensions(){
        m_Window.width=getwindowwidth()-6;
        m_Window.height=getwindowheight()-29;
    }


    void event_listener(){
        if(kbhit()){
            // static int vec=0;
            // static int* prev=NULL;
            switch(getch()){
                case (int)'d': //right
                    if(m_Car.pos.x+m_Car.width+m_Car.velocity.x >= m_Boundary.max.x){
                        m_Car.pos.x=m_Boundary.max.x-m_Car.width;
                        break;
                    }
                    m_Car.pos.x+=m_Car.velocity.x;
                    // vec=m_Car.velocity.x;
                    // prev=&m_Car.pos.x;
                    break;
                case (int)'a': //left
                    if(m_Car.pos.x-m_Car.velocity.x < m_Boundary.min.x){
                        m_Car.pos.x=m_Boundary.min.x;
                        break;
                    };
                    m_Car.pos.x-=m_Car.velocity.x;
                    // vec=10;
                    break;
                // case (int)'w': //speed
                //     if(m_Car.pos.y-m_Car.velocity.y <= m_Boundary.min.y){
                //         m_Car.pos.y=m_Boundary.min.y;
                //         break;
                //     };
                //     m_Car.pos.y-=m_Car.velocity.x;
                //     // vec+=1;
                //     break;
                // case (int)'s': //break;
                //     if(m_Car.pos.y+m_Car.height+m_Car.velocity.y >= m_Boundary.max.y){
                //         m_Car.pos.y=m_Boundary.max.y-m_Car.height;
                //         break;
                //     };
                //     m_Car.pos.y+=m_Car.velocity.x;
                //     // vec-=5;
                //     break;
                // default:
                    // *prev+=1;
                    // if(vec>0) vec--;
                    // if(vec<0) vec=0;
                    // m_Car.pos.y++;
            }
        }
    }



    void render_grass(){
        setcolor(COLOR(0,183,127));
        rect(0,0, m_Boundary.min.x, m_Window.height);
        rect(m_Boundary.max.x,0, m_Window.width, m_Window.height);
        setcolor(WHITE);
    }


    #ifndef BUILD_RELEASE
    void debug(){
        std::cout << m_Window.width << std::endl;
        std::cout << m_Window.height << std::endl;
    }
    #endif


    void render_score(){
        static std::ostringstream oss;
        static bool shouldIncrement=false;
        if(shouldIncrement){ m_Incrementor+=2; shouldIncrement=false;}
        if(m_Score % 100 == 0) {shouldIncrement=true; m_Objects.speed+=2;}
        if(m_Score % 1000 == 0) {shouldIncrement=true; m_Objects.speed+=2;}
        oss.clear();
        oss.str("");
        oss << "Score: " << std::to_string(m_Score) << std::endl;
        setcolor(COLOR(0,108,255));
        rect(0,m_Window.height-35,m_Window.width,m_Window.height);
        centered_text(m_Window,oss.str().c_str(),m_Window.height-25);
        m_Score+=m_Incrementor;
    }

    void render_menu(){
        int x=0;
        static float anim_y=0;
        settextstyle(10,0,4);
        int val=255*transitionFn(anim_y);
        setcolor(COLOR(val,val,val));
        centered_text(m_Window,"RACE CAR",(m_Window.height/2)*transitionFn(anim_y)); x+=40;
        settextstyle(10,0,2);
        centered_text(m_Window,"Press Space to start",(m_Window.height/2+x)); x+=20;
        if(kbhit() && getch()==' '){
            m_GameState=State::PLAYING;
            anim_y=0;
        }
        if(anim_y <=1.0) anim_y+=0.05;
        delay(20);
    }

    void render_game(){
        event_listener();
        spawn_object();
        m_Lanes.update(m_Window.height);
        m_Objects.update(m_Window.height);
        m_Lanes.render();
        m_Objects.render();
        if(m_DetectCollision && m_Objects.detect_collision(m_Car)) m_GameState=State::ENDED;
        render_score();
        m_Car.draw();
        generate_object();
        render_grass();
    }

    void render_end(){
        centered_text(m_Window, "Game Over",m_Window.height/2);
        centered_text(m_Window, std::to_string(m_Score).c_str(),m_Window.height/2+20);
        if(kbhit() && getch()==' '){
            game_restart();
            m_GameState=State::PLAYING;
        }
    }

    void render_game_state(){
        switch(m_GameState){
            case State::MENU:
                render_menu();
                break;
            case State::PLAYING:
                render_game();
                break;
            case State::ENDED:
                render_end();
                break;
        }
    }

    void run(){
        #ifndef BUILD_RELEASE
        debug();
        #endif
        while(!m_GameEnded){
            setactivepage(m_Page);
            setvisualpage(1-m_Page);
            cleardevice();

            render_game_state();

            m_Page=1-m_Page;
            delay(1000/m_Fps);
        }
    }
};


int main(){
    int win=initwindow(400, 600,"Doger");
    setcurrentwindow(win);
    GameEngine engine;
    engine.run();
    getch();
    closegraph();
    return 0;
}