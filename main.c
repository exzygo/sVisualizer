#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        SDL_Log("Usage: %s <path_to_image>\n", argv[0]);
        return 1;
    }

    const char* img_path = argv[1];

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("ERROR! SDL initialization error: %s\n", SDL_GetError());
        return 1;
    }

    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0) {
        SDL_Log("ERROR! SDL_image initialization error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface* surface = IMG_Load(img_path);
    if (!surface) {
        SDL_Log("ERROR! Loading image error: %s\n", IMG_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    int img_width = surface->w;
    int img_height = surface->h;

    SDL_Window* window = SDL_CreateWindow(img_path,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        img_width, img_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    float zoom = 1.0f;
    int offset_x = 0;
    int offset_y = 0;
    const int scroll_speed = 20;

    SDL_Event e;
    int running = 1;

    printf("img_path: %s\n", img_path);
    printf("img_width: %d, img_height: %d\n", img_width, img_height);

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = 0;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_PLUS:
                    case SDLK_EQUALS:
                        zoom *= 1.1f;
                        break;
                    case SDLK_MINUS:
                        zoom /= 1.1f;
                        if (zoom < 0.1f) zoom = 0.1f;
                        break;
                    case SDLK_LEFT:
                        offset_x += scroll_speed;
                        break;
                    case SDLK_RIGHT:
                        offset_x -= scroll_speed;
                        break;
                    case SDLK_UP:
                        offset_y += scroll_speed;
                        break;
                    case SDLK_DOWN:
                        offset_y -= scroll_speed;
                        break;
                    case SDLK_r:
                        offset_x = 0;
                        offset_y = 0;
                        zoom = 1.0f;
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 100);
        SDL_RenderClear(renderer);

        int win_w, win_h;
        SDL_GetWindowSize(window, &win_w, &win_h);

        int render_w = (int)(img_width * zoom);
        int render_h = (int)(img_height * zoom);

        SDL_Rect destRect = {
            .x = (win_w - render_w) / 2 + offset_x,
            .y = (win_h - render_h) / 2 + offset_y,
            .w = render_w,
            .h = render_h
        };

        SDL_RenderCopy(renderer, texture, NULL, &destRect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
