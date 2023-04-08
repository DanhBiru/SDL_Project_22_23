#ifndef GAME_H
#define GAME_H


class game
{
    public:
        game();
        virtual ~game();
        //Loads image at specified path
        bool loadFromFile( std::string path );

        void free();

        void render( int x, int y, SDL_Rect* clip = NULL );

        int getWidth();
        int getHeight();

    protected:

    private:
        SDL_Texture* mTexture;

        int mWidth;
        int mHeight;
};

#endif // GAME_H
