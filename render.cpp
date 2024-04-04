#include "render.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <charconv>

void XRender::layout1(){
    SDL_Rect scr = mkrect(0,0,windowx, windowy);
    box(scr, col(0), true);
    //rpm
    SDL_Rect r;
    r.x = 50; r.y = 50 * scale; r.w = windowx - 100; r.h=50 * scale;
    rpm(r);
    //temp(r, globs.hvtemp, 10, 60, "HVDC TEMP");
    //warning lights
    lights(0.1);
    //speed
    const char* spd = std::to_string(globs.speed).c_str();
    int tx, ty;
    TTF_SizeText(speedfont,spd, &tx, &ty);
    text( spd,speedfont, windowx / 2 - (tx / 2), windowy / 2 - ty / 2, col(255));
    
    //temp
    SDL_Rect e1 = mkrect(30, 100 * scale, 0, 0);
    temp(e1, globs.hvpercent, 0, 100, "HVDC Charge");
    SDL_Rect e2 = mkrect(windowx - 210 * scale ,100 * scale, 0, 0);
    atemp(e2, globs.enginetemp1, 50, 150, "LBLDC Temp");
    SDL_Rect e3 = mkrect(30, 200 * scale, 0, 0);
    temp(e3, globs.hvpercent, 10, 070, "LVDC Charge");
    SDL_Rect e4 = mkrect(windowx - 210 * scale,200 * scale, 10, 70);
    atemp(e4, globs.enginetemp2, 10, 070, "RBLDC Temp");
    SDL_Rect e5 = mkrect(70, 300 * scale, 0, 0);
    percent(e5, globs.fanlevel, 50, 150, "Fan Speed");
    SDL_Rect e6 = mkrect(windowx - 210 * scale,300 * scale, 0, 0);
    atemp(e6, globs.hvpercent, 50, 150, "Controllers T");


    //laps
    TTF_SizeText(rpmfont, "wa", &tx, &ty);
    SDL_Rect blap = mkrect(0, windowy - ty, windowx, ty);
    box(blap, col(40), true);
    char buf[100];
    struct tm * timeinfo;
    timeinfo = std::localtime(&globs.laptime);
    strftime(buf, 100, "%M:%S", timeinfo);
    TTF_SizeText(rpmfont, buf, &tx, &ty);
    text(buf,rpmfont, windowx/2 - tx / 2, windowy - ty, col(255));
    timeinfo = std::localtime(&globs.lasttime);
    strftime(buf, 100, "%M:%S", timeinfo);
    text(buf, rpmfont, 10 * scale, windowy - ty, col(255));
    timeinfo = std::localtime(&globs.deltatime);
    strftime(buf, 100, "%M:%S", timeinfo);
    if(globs.fastertime)
        text(buf, rpmfont, windowx - tx - 10, windowy-ty, col(255,0,0));
    else
        text(buf, rpmfont, windowx - tx - 10, windowy-ty, col(0,255,0));
     
}

void XRender::layout(int layid){
    switch(layid){
        case 1:
            layout1();
            break;
        default:
            text("no layout", rpmfont, 100, 100, col(255));
            break;
    }
}

void XRender::loadlights(){
   int lightcount = sizeof(path_iconite_warninguri)/100;
    
    for(int i = 0; i < lightcount; i++){
        warning_light light;
        const char* path = path_iconite_warninguri[i];
        const char* name = nume_iconite_warninguri[i];
        SDL_Surface* surf = IMG_Load(path);
        if(!surf){
            printf("Nu s-a putut deschide imaginea %s\n" , path);
        }else{
            light.texture = SDL_CreateTextureFromSurface(renderer,  surf);
            if(!light.texture)
                dbg(SDL_GetError());
            light.flashing = 0;
            light.active = 1;
            SDL_SetTextureColorMod(light.texture, 255,0,0);
            dash_lights.push_back(light);
            SDL_FreeSurface(surf);
            printf("Loaded light %s\n" , path);
        }
    }
}

void XRender::lights(float percenth){
    int rendered_lights = 0;
    for(auto light : dash_lights){
        if(light.active){
            SDL_Rect drawpos = mkrect(light_padding * ++rendered_lights + --rendered_lights *  light_size, light_padding, light_size, light_size);
            SDL_RenderCopy(renderer, light.texture, NULL, &drawpos);  
            rendered_lights++;
        }
    }
}

void XRender::box(SDL_Rect &rect, col c, bool filled){
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(renderer, &rect);
}
void XRender::line(SDL_Rect &pos, col c){
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawLine(renderer, pos.x, pos.y, pos.w, pos.h);
}
void XRender::text(const char* text,TTF_Font* font, int x, int y, col c){
    
    SDL_Color color = {(Uint8)c.r, (Uint8)c.g, (Uint8)c.b};
    
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color); 

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    auto rect = mkrect(x, y, 0,0);
    
    TTF_SizeText(font, text, &rect.w, &rect.h);
    
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
}
void XRender::rpm(SDL_Rect &size){
  int rpm = map(globs.hvtemp, 25, 60, 0, 60);
  const int maxrpm = 60;
  const int steps = 9;
  const int fs = rpmfontsize;
  box(size, col(24,24,24), true); // rpm background
  SDL_Rect rbar = mkrect(size.x + 5, size.y + 5, rpm * (size.w - 10) / maxrpm, size.h - 10);
  box(rbar, col(rpm * 255 / maxrpm, 255 - (rpm * 255 / maxrpm) , 0), true);
  for(int i = 1; i < steps; i++){
    int x = size.x + size.w / steps * i;
    SDL_Rect r = mkrect(x, size.y, x, size.y + size.h);
    line(r, col(0));
  }
  int tx, ty;
  TTF_SizeText(rpmfont, std::to_string(globs.hvtemp).c_str(), &tx, &ty);
  text(std::to_string(globs.hvtemp).c_str(),rpmfont, size.x + (size.w / 2) - tx / 2, size.y + (size.h / 2) - (ty / 2), col(255));
}

void XRender::atemp(SDL_Rect &pos, int temp, int min, int max, const char* name){
    temp = temp;
    const char* txt = (std::to_string(temp) + "C").c_str();
    int tx, ty;
    TTF_SizeText(tempfont,txt, &tx, &ty);
    int nx, ny;
    TTF_SizeText(tempfont,name, &nx, &ny);
    text(name, tempfont, pos.x, pos.y, col(255));
    int perc = map(temp, 0, 100, 0, 100);
    SDL_Rect barbg = mkrect(pos.x, pos.y + ny, nx, 22);  
    box(barbg, col(107), true); // rpm background
    SDL_Rect bar = mkrect(pos.x, pos.y + ny + 1, perc * nx / 100, 20);
    box(bar, col(perc * 255 / 100, 255 - (perc*255/100),0), true);
  //  SDL_Rect rbar = mkrect(pos.x, pos.y + 10, temp * (nx) / max, 10);
    //box(rbar, col(temp * 255 / max, 255 - (temp * 255 / max) , 0), true);
    text(txt,tempfont, pos.x + nx / 2 - tx / 2, pos.y+ny + 5, col(255));
}
void XRender::temp(SDL_Rect &pos, int temp, int min, int max, const char* name){
    temp = temp;
    const char* txt = (std::to_string(temp) + "%").c_str();
    int tx, ty;
    TTF_SizeText(tempfont,txt, &tx, &ty);
    int nx, ny;
    TTF_SizeText(tempfont,name, &nx, &ny);
    text(name, tempfont, pos.x, pos.y, col(255));
    int perc = map(temp, 0, 100, 0, 100);
    SDL_Rect barbg = mkrect(pos.x, pos.y + ny, nx, 22);  
    box(barbg, col(107), true); // rpm background
    SDL_Rect bar = mkrect(pos.x, pos.y + ny + 1, perc * nx / 100, 20);
    box(bar, col((100-perc) * 255 / 100, 255 - ((100-perc)*255/100),0), true);
  //  SDL_Rect rbar = mkrect(pos.x, pos.y + 10, temp * (nx) / max, 10);
    //box(rbar, col(temp * 255 / max, 255 - (temp * 255 / max) , 0), true);
    text(txt,tempfont, pos.x + nx / 2 - tx / 2, pos.y+ny + 5, col(255));
}
void XRender::percent(SDL_Rect &pos, int temp, int min, int max, const char* name){
    const char* txt = (std::to_string(temp) + "%").c_str();
    int tx, ty;
    TTF_SizeText(tempfont,txt, &tx, &ty);
    int nx, ny;
    TTF_SizeText(tempfont,name, &nx, &ny);
    text(name, tempfont, pos.x, pos.y, col(255));
    text(txt,tempfont, pos.x + nx / 2 - tx / 2, pos.y+ny, col(255));
}
