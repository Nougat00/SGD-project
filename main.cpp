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

    void update(SDL_Rect *rect1, SDL_Renderer* renderer, SDL_Rect bg, SDL_Texture *gameoverTexture, SDL_Texture *winTexture);

    void land();

    void gravity();

    void render(SDL_Renderer *renderer, SDL_Texture *texture);

private:
    double x, y;
    double vx, vy;
    double angle;
    double throttle;
    double fuel;
    SDL_Rect dst;
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

void Lander::gravity() {
    double dt = 0.01;
    double gravity = 9.81;
    double mass = 1000;
    double thrust = 25000 * throttle;
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
}

void Lander::land() {
    x = x;
    y = y;
}

SDL_Texture *texturer(const char *path, SDL_Renderer *renderer) {
    SDL_Surface *surface = IMG_Load(path);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void Lander::update(SDL_Rect *rect1, SDL_Renderer* renderer, SDL_Rect bg, SDL_Texture *gameoverTexture, SDL_Texture *winTexture) {
    if (SDL_HasIntersection(rect1, &dst) == SDL_TRUE) {
        if (vy > 10) {
            SDL_RenderCopy(renderer, gameoverTexture, NULL, &bg);
        } else {
            SDL_RenderCopy(renderer, winTexture, NULL, &bg);
            land();
        }
    } else { gravity(); }
}

void Lander::render(SDL_Renderer *renderer, SDL_Texture *texture) {
    dst = {int(x), int(y), 50, 50};
    SDL_RenderCopy(renderer, texture, NULL, &dst);
}

int main(int argc, char *argv[]) {
    SDL_Window *window = SDL_CreateWindow("Lunar Lander", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect bg = {0, 0, 640, 480};
    SDL_Rect hillsDes = {0, 470, 640, 10};
    SDL_Texture *bgTexture = texturer("assets/bg.png", renderer);
    SDL_Texture *hillsTexture = texturer("assets/floor.png", renderer);
    SDL_Texture *shipTexture = texturer("assets/ship.png", renderer);
    SDL_Texture *gameoverTexture = texturer("assets/gameover.png", renderer);
    SDL_Texture *winTexture = texturer("assets/win.png", renderer);

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
        SDL_RenderCopy(renderer, bgTexture, NULL, &bg);
        SDL_RenderCopy(renderer, hillsTexture, NULL, &hillsDes);
        lander.render(renderer, shipTexture);
        lander.update(&hillsDes, renderer, bg, gameoverTexture, winTexture);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(hillsTexture);
    SDL_DestroyTexture(gameoverTexture);
    SDL_DestroyTexture(winTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
