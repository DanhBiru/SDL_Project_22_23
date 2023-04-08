#include "game.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

void game::Nhap(SDL_Rect* _dau){
            dau = *_dau;
        }

void game::process(SDL_Event e){
    switch (e.key.keysym.sym){
        case SDLK_UP:
        case SDLK_w:
            vantocdoc = 20;
            break;
    }
}

void game::adjust(SDL_Rect* daudau){
    dau.y -= vantocdoc;
    if (dau.y + dau.h >= SCREEN_HEIGHT||dau.y <= 0){
        dau.y += vantocdoc;
    }
    *daudau = dau;
}

bool game::stop(){

}


