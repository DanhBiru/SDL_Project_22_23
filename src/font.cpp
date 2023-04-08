#include "font.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

using namespace std;

bool font::ttfInitialized = false;

font::font(size_t fontsize)
{
    if (!ttfInitialized && TTF_Init() == -1){
        cout << "Could not load ttf" << TTF_GetError() << endl;
    } else {
        cout << "ok roi day" << endl;
        ttfInitialized = true;
    }
    ourfont = TTF_OpenFont("8-bit-operator-8-regular.ttf", fontsize);
    if (ourfont == nullptr){
        cout << "could not load font" << endl;
        exit(1);
    }

}

font::~font()
{
    TTF_CloseFont(ourfont);
    SDL_DestroyTexture(textureText);
}

void font::DrawText(SDL_Renderer* renderer, std::string text){
    surfaceText = TTF_RenderText_Solid(ourfont, text.c_str(), {255, 255, 0});
    textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    SDL_FreeSurface(surfaceText);
    rectangleText = {30, 10, 200, 100};
    SDL_RenderCopy(renderer, textureText, NULL, &rectangleText);
}
