#ifndef FONT_H
#define FONT_H
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

class font
{
    public:
        font(size_t fontsize);
        virtual ~font();

        void DrawText(SDL_Renderer* renderer, std::string text);

    protected:

    private:
        SDL_Rect rectangleText;
        SDL_Texture* textureText;
        SDL_Surface* surfaceText;
        TTF_Font* ourfont;
        static bool ttfInitialized;
};

#endif // FONT_H
