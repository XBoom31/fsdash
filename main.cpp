#include "base.h"
#include "render.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

XRender rend;
sglobs globs;
bool running = true;

void readecu(){
}


void input(SDL_Event* event){
    if(event->type == SDL_KEYDOWN){
    if(event->key.keysym.sym == SDLK_UP){
      if(globs.laptime > globs.lasttime){
          globs.fastertime = true;
          globs.deltatime = globs.laptime - globs.lasttime;
      }else{
          globs.fastertime = false;
          globs.deltatime = globs.lasttime - globs.laptime;
      }
      globs.lasttime = globs.laptime;
      time(&globs.lapstart);
    }
  }
}

void update(){

  readecu(); 

  time(&globs.laptime);
  
  globs.laptime -= globs.lapstart;
}

void init(SDL_Window* win){
    
    if(fullscreen)
        SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);

    SDL_Renderer* renderer= SDL_CreateRenderer(win, -1, 0);

    TTF_Init();

    IMG_Init(IMG_INIT_PNG);
    
    time(&globs.lapstart);

    rend.renderer = renderer;
    
    rend.rpmfont = TTF_OpenFont("Sans.ttf", rpmfontsize);

    rend.speedfont = TTF_OpenFont("Sans.ttf", speedfontsize);
    
    rend.tempfont = TTF_OpenFont("Sans.ttf", tempfontsize);
    
    rend.loadlights();
    
    globs.rpm = 8365;
    globs.enginetemp1 = 87;
    globs.enginetemp2 = 39;
    globs.hvpercent = 80;
    globs.hvtemp = 45;
    globs.layout = 1;
}    
void draw(SDL_Window* win){
    
    SDL_RenderClear(rend.renderer);
  
    SDL_Rect scr = mkrect(0,0,windowx, windowy);
    
    rend.layout(globs.layout);
    
    SDL_RenderPresent(rend.renderer);
}


int main(){
    
    SDL_Init(SDL_INIT_EVERYTHING);
    
    SDL_Window* win;
    
    win = SDL_CreateWindow("xdash", 0, 0, windowx, windowy, 0);
    
    if(!win)
        dbg("Could not initialize, quitting");
    
    init(win);
    
    SDL_Event event;
    
    unsigned int now, last_tick;
    
    while(running){
        now = SDL_GetTicks();
         
        if(now - last_tick < 1000/fpscap)
            continue;
       
        while(!!SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT)
                running = false;
            input(&event);
        }
        
        update();
        
        draw(win);
        
        last_tick = now; 
    }
    
    SDL_DestroyWindow(win);
    
    SDL_Quit();
    dbg("Goodbye");
    
    return 0;
}
