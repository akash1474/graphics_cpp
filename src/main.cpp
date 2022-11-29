#include "iostream"
#include "graphics.h"
#include <algorithm>
#include <deque>
#include <iomanip>
#include "queue"

struct Vec2{
    int x=0,y=0;
    int width,height;
};

struct Window{
    int width,height;
};

struct Car{
    Vec2 pos;
    int width=50,height=90;
    Vec2 velocity;

    void draw(){
        //
    }
};

struct Boundary{
    Vec2 min;
    Vec2 max;
};

inline void rect(int x,int y,int x_max,int y_max){
    for(int i=x;i<=x_max;i++){
        line(i,y,i,y_max);
    }
}

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

    void update(const int& speed){
        pos.y+=speed;
    }

    void draw(){
        rect(pos.x,pos.y,pos.x+width,pos.y+height);
    }
};


//Drawing the animated Lanes moving down
struct Lane{

};


//Drawing shrubs on the side lane
struct Shrubs{

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
        setcolor(RED);
        std::for_each(objects.begin(), objects.end(),[&](Object& obj){
            obj.draw();
        });
        setcolor(WHITE);
    }

    bool detect_collision(const Car& car){
        static bool collision=false;
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

class GameEngine{
    Car m_Car;
    Window m_Window;
    Boundary m_Boundary;
    Objects m_Objects;
    int m_LaneWidth;
    int m_SideWidth=40;
    int m_SpawnDistance=20;
    int m_Count=0;
    int m_Page=0;
    State m_GameState;
    bool m_GameEnded=false;
    int m_Fps=60;
public:

    GameEngine(){
        m_GameState=State::MENU;
        get_window_dimensions();
        init_boundary();
        init_car();
        m_LaneWidth=(m_Window.width-(2*m_SideWidth))/3;
        m_Objects.speed=10;
        init_object();
    }

    void game_restart(){
        m_Objects.objects.clear();
        init_car();
        init_object();
    }

    void set_fps(const int& fps){
        m_Fps=fps;
    }


    int random(int from, int to){
        return rand() % (to - from + 1) + from;
    }


    void init_car(){
        m_Car.velocity.x=25;
        m_Car.velocity.y=25;
        m_Car.pos.x=(m_Window.width-m_Car.width)/2;
        m_Car.pos.y=m_Boundary.max.y-m_Car.height;
    }

    void init_object(){
        m_Objects.push(generate_object());
    }

    void spawn_object(){
        if(m_Count%m_SpawnDistance==0) m_Objects.push(generate_object());
        m_Count++;
    }




    Object generate_object(){
        Object obj;
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



    void render_lanes(){
        setlinestyle(DASHED_LINE,10,3);
        line(m_Boundary.min.x+m_LaneWidth,0,m_Boundary.min.x+m_LaneWidth,m_Window.height);
        line(m_Boundary.min.x+(m_LaneWidth*2),0,m_Boundary.min.x+(m_LaneWidth*2),m_Window.height);
        setlinestyle(SOLID_LINE, 0, 1);
    }

    void event_listener(){
        if(kbhit()){
            static int vec=0;
            static int* prev=NULL;
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
                default:
                    *prev+=1;
                    if(vec>0) vec--;
                    if(vec<0) vec=0;
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

    void render_car(){
        // readimagefile("car.jpg",m_Car.pos.x,m_Car.pos.y,m_Car.pos.x+m_Car.pos.width,m_Car.pos.y+m_Car.height);
        bar(m_Car.pos.x,m_Car.pos.y,m_Car.pos.x+m_Car.width,m_Car.pos.y+m_Car.height);
        floodfill(m_Car.pos.x+10,m_Car.pos.y+10,WHITE);
    }


    void debug(){
        std::cout << m_Window.width << std::endl;
        std::cout << m_Window.height << std::endl;
    }

    void render_menu(){
        int x=0;
        settextstyle(10,0,3);
        centered_text(m_Window,"RACE CAR",m_Window.height/2); x+=40;
        settextstyle(10,0,2);
        centered_text(m_Window,"Start",m_Window.height/2+x); x+=20;
        centered_text(m_Window,"High Score",m_Window.height/2+x);x+=20;
        centered_text(m_Window,"Exit",m_Window.height/2+x);
        if(kbhit() && getch()==' '){
            m_GameState=State::PLAYING;
        }
    }

    void render_game(){
        // setfillstyle(SOLID_FILL, BLACK);
        event_listener();
        spawn_object();
        m_Objects.update(m_Window.height);
        m_Objects.render();
        if(m_Objects.detect_collision(m_Car)) m_GameState=State::ENDED;
        render_lanes();
        render_car();
        generate_object();
        render_grass();
    }

    void render_end(){
        outtextxy(50, 50,(char*)"Game Over");
        if(kbhit() && getch()==' '){
            m_GameState=State::PLAYING;
            game_restart();
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
        debug();
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
    int gd=DETECT,gm;
    int win=initwindow(400, 600);
    setcurrentwindow(win);
    GameEngine engine;
    engine.run();
    getch();
    closegraph();
    return 0;
}