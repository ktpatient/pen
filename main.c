#include <stdio.h>
#include <math.h>
#include "raylib.h"

#define CIRCLE_COUNT 1024

#define xoff GetScreenWidth()/2
#define yoff 275    

typedef struct {
    Vector2 pos;
    Color color;
} Circle;

int main(void){ 

    Circle Circles[CIRCLE_COUNT] = {0};

    SetTargetFPS(60);
    // SetConfigFlags(FLAG_WINDOW_UNDECORATED|FLAG_WINDOW_MOUSE_PASSTHROUGH);
    InitWindow(1600, 900, "Double Pendulum");

    int r1 = 300;
    int r2 = 300;

    int m1 = 10;
    int m2 = 10;

    float a1_a = 0.0f;
    float a2_a = 0.0f;

    float a1_v = 0.0f;
    float a2_v = 0.0f;
    float ra = 3 * (GetRandomValue(0,10)/10.0f)/2;
    float a1 = ra;
    float a2 = PI/2;

    float g = 1.0f;

    Vector2 startPoint = (Vector2) {xoff, yoff};
    Vector2 point1 = (Vector2) {0};
    Vector2 point2 = (Vector2) {0};
    int frameCounter = 0;

    while (!WindowShouldClose()){
        
        float pa1 = -g * (2 * m1 + m2) * sinf(a1);
        float pa2 = -m2 * g * sin(a1 - 2 *a2);
        float pa3 = -2 *sinf(a1 - a2) * m2;
        float pa4 = a2_v * a2_v * r2 + a1_v * a1_v * r1 * cosf(a1 - a2);
        float denom = (r1 * (2 * m1 + m2 - m2 * cosf(2 * a1 - 2 * a2)));

        a1_a = (pa1 + pa2 + pa3 * pa4) / denom;
        
        pa1 = 2 * sinf(a1 - a2);
        pa2 = (a1_v * a1_v * r1 * (m1 + m2));
        pa3 = g * (m1 + m2) * cosf(a1);
        pa4 = a2_v * a2_v * r2 * m2 * cosf(a1 - a2);
        denom = r2 * (2 * m1 + m2 - m2 * cosf(2 * a1 - 2 * a2));

        a2_a = (pa1 * (pa2 + pa3 + pa4)) / denom;

        point1.x = xoff + sinf(a1) * r1;
        point1.y = yoff + cosf(a1) * r1;
        
        point2.x = point1.x + sinf(a2) * r2;
        point2.y = point1.y + cosf(a2) * r2;

        Circles[frameCounter] = (Circle) {.pos = point2, .color = ColorContrast(ColorFromHSV(GetTime()*2,0.4f,((point2.y)/(GetScreenHeight()+25))), GetTime())};


        BeginDrawing();
        ClearBackground(GetColor(0x181818FF));
        // DrawCircleV(startPoint,25,BLUE);
        // DrawCircleV(point1, 25, YELLOW);
        
        for (int i = 0; i < CIRCLE_COUNT; i++) {
            DrawCircleV(Circles[(i + frameCounter) % CIRCLE_COUNT].pos,25,Circles[(i + frameCounter) % CIRCLE_COUNT].color);
        }

        DrawLineEx(startPoint, point1, 5.0f, YELLOW);    
        DrawLineEx(point1, point2, 5.0f, YELLOW);
        DrawCircleV(Circles[frameCounter].pos, 25, Circles[frameCounter].color);

        EndDrawing();

        a1_v += a1_a;
        a2_v += a2_a;

        a1 += a1_v;
        a2 += a2_v;
        frameCounter = ((frameCounter += 1) % CIRCLE_COUNT);
    }
    CloseWindow();
    return 0;
}