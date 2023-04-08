#include <iostream>
#include <random>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "game.h"
#include <sstream>
#include "font.h"

using namespace std;

extern const int SCREEN_WIDTH = 1200;
extern const int SCREEN_HEIGHT = 800;
const string WINDOW_TITLE = "game";

int so_cot_dan = 3;
int van_toc_doc_coin = 10;

int second = 0;

random_device xx;
uniform_int_distribution<int> dist(0,so_cot_dan);
uniform_int_distribution<int> distr(3,4);


//***********************************************
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal = false);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();
//
SDL_Texture* loadTexture(string path, SDL_Renderer* renderer);
SDL_Texture* loadText(string path, SDL_Renderer* renderer);

//***********************************************

//hàm vẽ đạn bay
void bulletcoming(int &a, int &b);

//Kích cỡ đạn
int bullet_width = 300;
int bullet_height = 200;

//hàm vẽ đồng xu bay
void coinflying(int &c, int &d);

int shield_number = 4;
int shield_size = 100;
bool dinh_dan = false;

//điểm ban đầu bằng 0
int diem = 0;

int main(int argc, char* argv[]){
    //khai báo các biến cơ bản
    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;
    initSDL(m_pWindow, m_pRenderer);

    //khai báo khiên
    SDL_Rect shields[shield_number];
    for (int i = 0; i < shield_number; ++i){
        shields[i] = {SCREEN_WIDTH - (i+1)*shield_size, SCREEN_HEIGHT - shield_size, shield_size, shield_size};
    }

    //biến đếm khiên
    int remaining_shields = shield_number;

    //stringstream để tính điểm
    stringstream ss;
    string gay;
    ss << diem;
    ss >> gay;

    //khai bảo biến class font với cỡ chữ 60
    font tinhdiem(60);

    //khai báo các texture hiểu thị
    SDL_Texture* m_pTexture = loadTexture("anh/birbs.png", m_pRenderer);
    SDL_Texture* background = loadTexture("anh/background.png", m_pRenderer);
    SDL_Texture* bullet = loadTexture("anh/bullet.png", m_pRenderer);
    SDL_Texture* m_coin = loadTexture("anh/coinz.png", m_pRenderer);
    SDL_Texture* m_shield = loadTexture("anh/shield.png", m_pRenderer);


    //Số đạn hiển thị trên màn hình
    int bullet_number = 4;
    SDL_Rect m_bullet[bullet_number];

    //Tạo sẵn các viên đạn nằm ngoài màn hình
    for (int i = 0; i < bullet_number/2; ++i){
        m_bullet[i+i] = {SCREEN_WIDTH*3/2, dist(xx)*SCREEN_HEIGHT/so_cot_dan, bullet_width, bullet_height};
        m_bullet[i+i+1] = {SCREEN_WIDTH*2, dist(xx)*SCREEN_HEIGHT/so_cot_dan, bullet_width, bullet_height};
    }

    SDL_Rect m_srcCoin = {0, 0, 89, 90};
    SDL_Rect m_desCoin = {-200, 0, 100, 100};

    SDL_Rect m_srcRect = {0, 0, 115, 139};
    SDL_Rect m_head = {50, 20, 150, 150};

    //Khai báo biến sự kiện
    SDL_Event e;
    bool quit = false;

    //Khai báo biến thuộc class game cho việc di chuyển nhân vật
    game quanque;

    //vòng lạp game
    while (!quit){
        quanque.Nhap(&m_head);
        const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
        if (SDL_PollEvent(&e)!=0){
            //click X hoặc ấn ESC để thoát game
            if (e.type == SDL_QUIT||(e.key.keysym.sym == SDLK_ESCAPE)) quit = true;
            if (currentKeyStates[SDL_SCANCODE_UP]){
                //Khi giữ phím nhân vật di chuyển lên
                quanque.vantocdoc = 10;
            } else {
                //Nhả phím nhân vật di chuyển xuống
                quanque.vantocdoc = -10;
            }
        }

        //Điều chỉnh và trả về vị trí nhân vật
        quanque.adjust(&m_head);

        SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 0);
        SDL_RenderClear(m_pRenderer);

        //Hiển thị background
        SDL_RenderCopy(m_pRenderer, background, NULL, NULL);

        //Viên đạn di chuyển và va chạm
        for (int i = 0; i < bullet_number; ++i){
            if ((abs(m_bullet[i].x - m_head.x) < m_head.w - 10)&&(abs(m_bullet[i].y - m_head.y) < m_head.h/2 - 10)){
                //Va chạm nhân vật thì bool = true
                dinh_dan = true;
            }
            bulletcoming(m_bullet[i].x, m_bullet[i].y);
            SDL_RenderCopy(m_pRenderer, bullet, NULL, &m_bullet[i]);
        }

        //Nếu ăn được đồng xu thì +50 điểm và đồng xu biến mất khỏi màn hình
        if ((abs(m_desCoin.x - m_head.x) < 100)&&(abs(m_desCoin.y - m_head.y) < 100)){
            diem += 50;
            m_desCoin.x -= 500;
        }

        //timer
        second += 1;

        //bool = true thì trừ khiên
        if (second == 30&&dinh_dan == true){
            remaining_shields -= 1;
            dinh_dan = false;
        }

        //cộng điểm theo thời gian
        if (second == 40){
            diem += 1;
            second = 0;
        }

        //Đồng xu di chuyển
        coinflying(m_desCoin.x, m_desCoin.y);

        //Hoạt họa cho nhân vật và đồng xu
        m_srcRect.x = 115 * int(((SDL_GetTicks()/100) % 10));
        m_srcCoin.x = 90 * int(((SDL_GetTicks()/100) % 6));

        //update điểm thông qua stringstream
        ss.clear();
        ss << diem;
        ss >> gay;

        //In ra màn hình điểm
        tinhdiem.DrawText(m_pRenderer, gay);

        //hiện khiên theo số khiên còn lại
        if (remaining_shields != -1){
            for (int i = 0; i < remaining_shields; ++i){
                SDL_RenderCopy(m_pRenderer, m_shield, NULL, &shields[i]);
            }
        } else {
            //hết khiên thì ngoẻo
            quit = true;
        }

        //in ra màn hình đồng xu và nhân vật
        SDL_RenderCopy(m_pRenderer, m_coin, &m_srcCoin, &m_desCoin);
        SDL_RenderCopy(m_pRenderer, m_pTexture, &m_srcRect, &m_head);

        SDL_RenderPresent(m_pRenderer);
    }

    SDL_Delay(2000);
    tinhdiem.~font();
    waitUntilKeyPressed();
    quitSDL(m_pWindow, m_pRenderer);
    return 0;
}

void bulletcoming(int &a, int &b){
    if (a >= -bullet_width){
        a -= 20;
    } else {
        a += SCREEN_WIDTH*distr(xx)/2;
        b = dist(xx)*SCREEN_HEIGHT/(so_cot_dan+1);
    }
}

void coinflying(int &c, int &d){
    if (d>SCREEN_HEIGHT - 100||d<0){
        van_toc_doc_coin *= -1;
    }
    if (c >= -SCREEN_WIDTH){
        c -= 5;
        d += van_toc_doc_coin;
    } else {
        c += SCREEN_WIDTH*3;
        d = dist(xx)*SCREEN_HEIGHT/(so_cot_dan+1);
    }
}

SDL_Texture* loadTexture(string path, SDL_Renderer* renderer)
{
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr){
        cout << "Unable to load image" << path << "SDL_image Error: " << IMG_GetError() << endl;
    } else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == nullptr){
            cout << "Unable to create texture from " << path << "SDL Error:" << SDL_GetError() << endl;
            SDL_FreeSurface(loadedSurface);
        }
    }
    return newTexture;
}

void logSDLError(std::ostream& os, const std::string &msg, bool fatal){
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal){
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        logSDLError(std::cout, "SDL_Init", true);
    }


    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
    //   SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);


    //Khi thông thường chạy với môi trường bình thường ở nhà
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

    //Khi chạy ở máy thực hành WinXP ở trường (máy ảo)
//    renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}


void quitSDL(SDL_Window* window, SDL_Renderer* renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void waitUntilKeyPressed(){
    SDL_Event e;
    while (true){
        if (SDL_WaitEvent(&e) != 0&&(e.type == SDL_KEYDOWN || e.type == SDL_QUIT))
            return;
        SDL_Delay(100);
    }
}



