#include<raylib.h>
#include<cmath>
#include<chrono>
#include<thread>

const float win_width = 800;
const float win_height = 800;

bool game_over = false; //Breaks out of game loop if this is true.
void gameover(){
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    while(!WindowShouldClose()){   
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("GAME OVER", win_width/2 - 300, win_height/2 - 50, 100, RED);
        EndDrawing();
    }
    game_over = true;
}

class obj{
    //main square at the centre.
    public:
        float forcex = 0, forcey = 0;
        float velx = 0, vely = 0;
        float speed = 700;
        float width = 50, height = 50;
        float posx = win_width/2 - width/2;
        float posy = win_height/2 - height/2;
        
        float timer = 0;
        float out_time = 2;
        float elapsed = timer / out_time;
        
        void draw(){
            DrawRectangle(posx, posy, width, height, RED);
        }
        void move(){
            // moves the square on keyboard input.
            
            forcex = -600*(posx+width/2 - win_width/2);
            forcey = -600*(posy+height/2 - win_height/2);
            velx = forcex*GetFrameTime();
            vely = forcey*GetFrameTime();
            posx += velx*GetFrameTime();
            posy += vely*GetFrameTime();
            
            if(IsKeyDown(KEY_UP) || IsKeyDown(KEY_K)){
                posy -= speed*GetFrameTime();    
            }
            if(IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_J)){
                posy += speed*GetFrameTime();
            }
            if(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_L)){
                posx += speed*GetFrameTime();
            }
            if(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_H)){
                posx -= speed*GetFrameTime();
            }
        }
        bool chk_outof_axs(){
            // checks if the square is not on any axis(x or y).
            // the main square can't collide with the moving square if this is the case.
            // one can stay in this region for <out_time> seconds.

            float diffX = fabs(posx + width/2.0f - win_width/2.0f);
            float diffY = fabs(posy + height/2.0f - win_height/2.0f);
            if(diffX >= 5.0f && diffY >= 5.0f){
                if(timer <= out_time){
                    timer += GetFrameTime();
                    float elapsed = timer / out_time;
                    BeginDrawing();
                    DrawRectangle(0, win_height-20, (int)(elapsed*win_width), 20, RED);
                    EndDrawing();
                    return true; 
                }else{
                    gameover();    
                }
            }
            return false;
        }
};

class obsy{
    // Squares moving in the y-axis.
    public:
    float width = 30, height = 30;
    float ps_x = win_width/2 - width/2,
          ps_y = win_height + GetRandomValue(0, 100);
    static float speed;    
    Color colour = BLUE;

    void draw(){
        DrawRectangle(ps_x, ps_y, width, height, colour);
    }
    void move(){
        ps_y -= speed*GetFrameTime();
    }
};
float obsy::speed = 100;

class obsx{
    // Squares moving in x-axis.
    public:
    float width = 30, height = 30;
    float ps_x = win_width + GetRandomValue(1000, 5000),
          ps_y = win_height/2 - height/2;
    static float speed;    
    Color colour = GREEN;

    void draw(){
        DrawRectangle(ps_x, ps_y, width, height, colour);
    }
    void move(){
        ps_x -= speed*GetFrameTime();
    }
};
float obsx::speed = 100;

void checkcollision(obj& o, obsx** ox, obsy** oy, int n){
    for(int i=0; i<n; ++i){      
        Rectangle a = {o.posx, o.posy, o.width, o.height};
        Rectangle x = {ox[i]->ps_x, ox[i]->ps_y, ox[i]->width, ox[i]->height};
        Rectangle y = {oy[i]->ps_x, oy[i]->ps_y, oy[i]->width, oy[i]->height};

        if(CheckCollisionRecs(a, x) || CheckCollisionRecs(a, y)){
            gameover();
        }
    }
}

void showscore(){
    // Shows the score and increases the difficulty with the score.
    int score;
    int weight = 1;
    score = weight*GetTime();
    if(score%10 == 0){
        weight += 1;
        obsx::speed += 0.5;
        obsy::speed += 0.5;
    }
    DrawText(TextFormat("Score: %d", score), 20, 20, 30, ORANGE);
}

int main(){
    InitWindow(win_width, win_height, "game");
    SetTargetFPS(60);

    int num = 50;
    int gap = 500;
    obsx* ox[num];  // Arrays of pointers to be pointed to square objects in 
    obsy* oy[num];  // x and y axes on the heap below.
    
    for(int i=0; i<num; ++i){
        ox[i] = new obsx;
        oy[i] = new obsy;
    }
               
    obj rect; // Main square at the centre.
    while(!WindowShouldClose()){
        BeginDrawing();
                
        ClearBackground(BLACK);

        if(GetTime() < 5){
            DrawText("VIM Keys Can Be Used To Move", 10, win_height-20, 15, LIGHTGRAY);
        }
        for(int i=0; i<num; ++i){
            // Checks if the squares in the x and y axes overlap or if they are
            // too close to each other.
            // Separates them if they are.
            
            for(int j=i+1; j<num; ++j){
                if(j == i)
                    continue;
                
                if(fabs(ox[i]->ps_x - ox[j]->ps_x) < gap){
                    if(ox[i]->ps_x > ox[j]->ps_x)
                        ox[i]->ps_x += GetRandomValue(gap, 5000);
                    else
                        ox[j]->ps_x += GetRandomValue(gap, 5000);
                }
                if(fabs(oy[i]->ps_y - oy[j]->ps_y) < gap){
                    if(oy[i]->ps_y > oy[j]->ps_y)
                        oy[i]->ps_y += GetRandomValue(gap, 5000);
                    else
                        oy[j]->ps_y += GetRandomValue(gap, 5000);
                }
            }
        }

        for(int i=0; i<num; ++i){
            ox[i]->draw();
            oy[i]->draw();
            ox[i]->move();
            oy[i]->move();

            if(ox[i]->ps_x+ox[i]->width < 0){
                delete ox[i];
                ox[i] = new obsx;
            }
            if(oy[i]->ps_y+oy[i]->height < 0){
                delete oy[i];
                oy[i] = new obsy;
            }
        }
        showscore();
        rect.draw();
        rect.move();

        checkcollision(rect, ox, oy, num);
        if(!rect.chk_outof_axs()){
            rect.timer = 0;
        }
         if(game_over){
            EndDrawing();
            break;
        }
            
        EndDrawing();
    }

    for(int i=0; i<num; ++i){
        delete ox[i];
        delete oy[i];
    }

    CloseWindow();
    return 0;
}
