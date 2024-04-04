#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <iostream>
#include <string>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "config.h"

class p2{
public:
    p2(float x2, float y2){
    x = x2;
    y = y2;
  }
    float x, y; 
};

struct p3{
  float x, y, z;
};
inline int map(int x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

struct sglobs{
  int rpm;
  int battery;
  int speed;
  int enginetemp1, enginetemp2;
  int hvtemp;
  int hvpercent;
  bool wlowbattery;
  bool wtemphigh;
  bool wreadytodrive;
  bool wapps;
  int fanlevel;
  time_t laptime;
  time_t lapstart;
  time_t lasttime;
  time_t deltatime;
  bool fastertime;
  int layout;
}; 
extern sglobs globs;
class col{
    public:
        int  r;
        int g;
        int b;
        int a;
        col(char r1, char b1, char c1){
            r = r1;
            g = b1;
            b = c1;
            a = 255;
        };
        col(int c){
            r = c; g = c; b = c; a = 255;
        };
};

inline SDL_Rect mkrect(int x1, int y1, int w1, int h1){
  SDL_Rect r;
  r.x = x1;
  r.y = y1;
  r.w = w1;
  r.h = h1;
  return r;
}

inline void dbg(const char* msg){
    std::cout << msg << std::endl;
};
inline void scalevec(SDL_Rect& rect){
    rect.x *= scale;
    rect.y *= scale;
    rect.w *= scale;
    rect.y *= scale;
}

