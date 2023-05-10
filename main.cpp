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

    void update(SDL_Rect *rect1);

    void land();

    void gravity();

    void render(SDL_Renderer *renderer, SDL_Texture *texture);

    SDL_Rect getDst();

private:
    double x, y;
    double vx, vy;
    double angle;
    double throttle;
    double fuel;
    SDL_Rect src;
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

SDL_Rect Lander::getDst() {
    return dst;
}

void Lander::gravity() {
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
}

void Lander::land() {
    x = x;
    y = y;
}

SDL_Rect rect(int h, int w, int x, int y) {
    SDL_Rect rect1;
    rect1.h = h;
    rect1.w = w;
    rect1.x = x;
    rect1.y = y;
    return rect1;
}

void Lander::update(SDL_Rect *rect1) {
    if (SDL_HasIntersection(rect1, &dst) == SDL_TRUE) {
        std::cout << "test" << std::endl;
    } else { gravity(); }
}

SDL_Texture *texturer(const char *path, SDL_Renderer *renderer) {
    SDL_Surface *surface = IMG_Load(path);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    return texture;
}


void Lander::render(SDL_Renderer *renderer, SDL_Texture *texture) {
    src = rect(480, 640, 0, 0);
    dst = rect(50, 50, x, y);
    SDL_RenderCopy(renderer, texture, &src, &dst);
}

int main(int argc, char *argv[]) {
    SDL_Window *window = SDL_CreateWindow("Lunar Lander", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect bg = rect(480, 640, 0, 0);
    SDL_Rect hillsSrc = rect(10, 640, 0, 0);
    SDL_Rect hillsDes = rect(10, 640, 0, 470);
    SDL_Texture *bgTexture = texturer("assets/bg.png", renderer);
    SDL_Texture *hillsTexture = texturer("assets/floor.png", renderer);
    SDL_Texture *shipTexture = texturer("assets/ship.png", renderer);

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
        SDL_RenderCopy(renderer, bgTexture, &bg, &bg);
        SDL_RenderCopy(renderer, hillsTexture, &hillsSrc, &hillsDes);
        lander.update(&hillsSrc);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        lander.render(renderer, shipTexture);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(hillsTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
