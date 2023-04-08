#ifndef GAME_H
#define GAME_H
#include <SDL.h>


class game
{
    public:
//        game();
//        virtual ~game();

        void process(SDL_Event e);

        void Nhap(SDL_Rect* _dau);

        void adjust(SDL_Rect* daudau);

        int vantocdoc = 20;

        bool stop();
    protected:
        SDL_Rect dau;
    private:
};

#endif // GAME_H
