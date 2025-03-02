#ifndef WAVE_H_
#define WAVE_H_

#include <raylib.h>
#include <stdlib.h>
#include <math.h>

#define R HEIGHT/TILE
#define C WIDTH/TILE

#define RP(v) (int)(R*(v))
#define CP(v) (int)(C*(v))

#define DAMPING_ZONE 60
#define DAMPING_FACTOR 0.95

typedef enum {
  ORDINARY,
  WALL
} CellType;

typedef struct {
  float p, v, m;
  CellType t;
} Cell;

#endif


#ifdef WAVE_IMPLEMENTATION

Cell *cs;
Cell *ncs;

void cs_draw(){
  for(int i = 0; i < R; i++){
    for(int j = 0; j < C; j++){
      Color c = {255, 255, 255, (unsigned char)(fminf(fabsf(cs[i * C + j].p)*255, 255.0))};
      if(cs[i*C+j].t == WALL) c = RED;
      DrawRectangle(j * TILE, i * TILE, TILE, TILE, c);
    }
  }
}

void cs_upd_pos(){
  for(int i = 0; i < R; i++){
    for(int j = 0; j < C; j++){
      ncs[i*C+j] = cs[i*C+j];
      float t = 0, c = 0;
      if(i > 0){t += cs[(i-1)*C+j].p;c+=1.0;}
      if(i < R-1){t += cs[(i+1)*C+j].p;c+=1.0;}
      if(j > 0){t += cs[i*C+j-1].p;c+=1.0;}
      if(j < C-1){t += cs[i*C+j+1].p;c+=1.0;}

      ncs[i*C+j].v += (t/c - ncs[i*C+j].p) / ncs[i*C+j].m;
    }
  }

  for(int i = 0; i < R; i++){
    for(int j = 0; j < C; j++){
      if(ncs[i*C+j].t != WALL)
	ncs[i*C+j].p += ncs[i*C+j].v * GetFrameTime();
    }
  }

  for(int i = 0; i < R; i++){
    for(int j = 0; j < C; j++){
      if(i < DAMPING_ZONE || i >= R-DAMPING_ZONE || 
         j < DAMPING_ZONE || j >= C-DAMPING_ZONE){
        int dx = fmin(i, R-1-i);
        int dy = fmin(j, C-1-j);
        int dist = fmin(dx, dy);
        float factor = DAMPING_FACTOR + (1.0-DAMPING_FACTOR) * (DAMPING_ZONE-dist)/(float)DAMPING_ZONE;
        
        ncs[i*C+j].v *= factor;
        ncs[i*C+j].p *= factor;
      }
    }
  }
  
  Cell *b = cs;
  cs = ncs;
  ncs = b;
}

void cs_init(){
  cs = malloc(sizeof(Cell) * R * C);
  ncs = malloc(sizeof(Cell) * R * C);
  
  for(int i = 0; i < R; i++){
    for(int j = 0; j < C; j++){
      cs[i*C+j].p = 0;
      cs[i*C+j].v = 0;
      cs[i*C+j].m = MASS;
      cs[i*C+j].t = ORDINARY;
    }
  }
}

void circle_wave(int x, int y, float radius, float amp){
  for(int i = 0; i < R; i++){
    for(int j = 0; j < C; j++){
      int dx = x - j;
      int dy = y - i;
	 
      float distance = sqrt(dx * dx + dy * dy);

      if(distance < radius){
	cs[i*C+j].p = (1.0 - distance / radius) * amp;
      }
    }
  }
}

void circle_wave_2(int x, int y, float amp, float freq){
  cs[y*C+x].p = sin(GetTime()*freq)*amp;
}

void vertical_line_wave(int x, int y, int l, float amp, float freq){
  for(int i = 0; i < l; i++){
    if(y+i < R-1) cs[(y+i)*C+(x)].p = sin(GetTime()*freq)*amp;
  }
}

void vertical_line_wall(int x, int y, int l){
  for(int i = 0; i < l; i++){
    if(y+i < R-1) cs[(y+i)*C+(x)].t = WALL;
  }
}

#endif
