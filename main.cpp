#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Lander {
public:
    Lander();
    ~Lander();
    void handleEvent(SDL_Event &e);
    void update();
    void render(SDL_Renderer *renderer);
private:
    double x, y;
    double vx, vy;
    double angle;
    double throttle;
    double fuel;
};

Lander::Lander() {
    x = SCREEN_WIDTH / 2;
    y = 240;
    vx = 0;
    vy = 0;
    angle = 0;
    throttle = 0;
    fuel = 100000000;
}

Lander::~Lander() {}

void Lander::handleEvent(SDL_Event &e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                throttle = 1;
                break;
            case SDLK_LEFT:
                angle = -0.1;
                break;
            case SDLK_RIGHT:
                angle = 0.1;
                break;
        }
    } else if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                throttle = 0;
                break;
            case SDLK_LEFT:
            case SDLK_RIGHT:
                angle = 0;
                break;
        }
    }
}

void Lander::update() {
    double dt = 0.01;
    double gravity = 9.81;
    double mass = 1000;
    double thrust = 20000 * throttle;
    double burnRate = 1;
    double burn = burnRate * throttle * dt;
    fuel -= burn;
    if (fuel < 0) {
        fuel = 0;
        throttle = 0;
    }
    double thrustX = thrust * sin(angle);
    double thrustY = thrust * cos(angle);
    double ax = thrustX / mass;
    double ay = gravity - thrustY / mass;
    vx += ax * dt;
    vy += ay * dt;
    x += vx * dt;
    y += vy * dt;
    std::cout<<y<<std::endl;

//    if(SDL_hasIntersection(rect, rect2)) {
// land()}else {gravity()}
// void land() {
// dstRect.y = getRect()
// }
//
//    if(!SDL_hasIntersection(rect, rect2)) {
// gravity()}
// void land() {
// dstRect.y = getRect()
// }

}

void Lander::render(SDL_Renderer *renderer) {
    SDL_Surface* surfaceShip = IMG_Load("assets/ship.png");
    SDL_Texture* textureShip = SDL_CreateTextureFromSurface(renderer, surfaceShip);
    SDL_Rect srcBgShip;
    srcBgShip.h = 480;
    srcBgShip.w = 640;
    srcBgShip.x = srcBgShip.y = 0;
    SDL_Rect dstBgShip = {int(x - 5), int(y - 5), 50, 50};
    SDL_RenderCopy(renderer, textureShip, &srcBgShip, &dstBgShip);
}

SDL_Texture* texturer(std::string path, SDL_Renderer *renderer) {
    SDL_Surface* surface = IMG_Load(path);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    return texture;
}

SDL_Rect rect(int h, int w, int x, int y) {
    SDL_Rect rect1;
    rect1.h = h;
    rect1.w = w;
    rect1.x = x;
    rect1.y = y;
    return rect1;
}

int main(int argc, char *argv[]) {
    SDL_Window *window = SDL_CreateWindow("Lunar Lander", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* surfaceHills = IMG_Load("assets/hills.png");
    SDL_Texture* textureHills = SDL_CreateTextureFromSurface(renderer, surfaceHills);
    SDL_Rect srcBgHills;
    srcBgHills.h = 480;
    srcBgHills.w = 640;
    srcBgHills.x = srcBgHills.y = 0;
    SDL_Rect dstBgHills;
    dstBgHills.h = 480;
    dstBgHills.w = 640;
    dstBgHills.x = dstBgHills.y = 0;

    SDL_Rect srcBg = rect(480, 640, 0, 0);

    Lander lander;
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            lander.handleEvent(e);
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texturer("assets/bg.png", renderer), &srcBg, &srcBg);
        SDL_RenderCopy(renderer, textureHills, &srcBgHills, &srcBgHills);
        lander.update();
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        lander.render(renderer);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
