#pragma once
#include "base.h"
#include <SDL2/SDL_ttf.h>

typedef struct{
    SDL_Texture* texture;
    char name[20];
    bool active;
    int color; // 0 red 1 yellow 2 green
    bool flashing;
} warning_light;

class XRender{
    private:
        void layout1();
    public:
        void text(const char* text, TTF_Font* font, int x, int y, col c);
        void box(SDL_Rect &rect, col c, bool filled);
        void line(SDL_Rect &pos, col c);
        void percent(SDL_Rect &pos, int temp, int min, int max, const char* name);
        void temp(SDL_Rect &pos, int temp, int min, int max, const char* name);
        void atemp(SDL_Rect &pos, int temp, int min, int max, const char* name);
        void rpm(SDL_Rect &size); 
        void loadlights();
        void lights(float percenth);
        void layout(int layid);
        SDL_Renderer* renderer;
        TTF_Font* rpmfont;
        TTF_Font* speedfont;
        TTF_Font* tempfont;
        std::vector<warning_light> dash_lights;
        SDL_Texture* tlowbat;
        SDL_Texture* thightemp;
        SDL_Texture* treadytodrive;
        SDL_Texture* tapps;
        SDL_Texture* trelay1;
        SDL_Texture* trelay2;
  //TTF_Font* fontsans;
};
