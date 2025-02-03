#include <raylib.h>

#define WIDTH 1280
#define HEIGHT 920
#define TILE 2

#define MASS 0.05

#define DAMPING_ZONE 40
#define DAMPING_FACTOR 0.92

#define WAVE_IMPLEMENTATION
#include "wave.h"

int main(void){
  InitWindow(WIDTH, HEIGHT, "window");
  SetTargetFPS(60);

  cs_init();

  while(!WindowShouldClose()){
    BeginDrawing();
    ClearBackground(BLACK);

    vertical_line_wave(CP(0.3), RP(0.3), RP(0.4), 1.0, 8);

    float t = 0.4, c = 0.18;
    
    vertical_line_wall(CP(0.6), RP(0), RP(t));
    vertical_line_wall(CP(0.6), RP(0.5-c/2.0), RP(c));
    vertical_line_wall(CP(0.6), RP(1-t), RP(t));

    cs_upd_pos();
    cs_draw();
    
    EndDrawing();
  }

  CloseWindow();
}
