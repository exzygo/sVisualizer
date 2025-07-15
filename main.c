/*
 * Copyright (C) 2025 mvguest
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

/**
 * @brief Entry point of the application.
 *
 * This program loads an image using SDL2_image and displays it
 * in a resizable SDL window. It supports zooming and panning via keyboard.
 *
 * @param argc Number of command-line arguments.
 * @param argv Command-line arguments array. Expects the image path as argv[1].
 * @return int Exit status code.
 */
int main(int argc, char* argv[]) {
    // Check if image path was provided
    if (argc < 2) {
        SDL_Log("Usage: %s <path_to_image>\n", argv[0]);
        return 1;
    }

    const char* img_path = argv[1];

    // Initialize SDL video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("ERROR! SDL initialization error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_image with support for JPG and PNG
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0) {
        SDL_Log("ERROR! SDL_image initialization error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    // Load image into surface
    SDL_Surface* surface = IMG_Load(img_path);
    if (!surface) {
        SDL_Log("ERROR! Loading image error: %s\n", IMG_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    int img_width = surface->w;
    int img_height = surface->h;

    // Create SDL window with image dimensions
    SDL_Window* window = SDL_CreateWindow(img_path,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        img_width, img_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS);

    // Create renderer and texture from loaded image
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // Free surface after creating texture

    // Initialize zoom and offset settings
    float zoom = 1.0f;
    int offset_x = 0;
    int offset_y = 0;
    const int scroll_speed = 20;

    SDL_Event e;
    int running = 1;

    // Log image metadata
    printf("img_path: %s\n", img_path);
    printf("img_width: %d, img_height: %d\n", img_width, img_height);

    // Main event loop
    while (running) {
        // Handle events (keyboard input, window close, etc.)
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = 0;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_PLUS:
                    case SDLK_EQUALS:                   // Zoom in
                        zoom *= 1.1f;
                        break;
                    case SDLK_MINUS:                    // Zoom out
                        zoom /= 1.1f;
                        if (zoom < 0.1f) zoom = 0.1f;
                        break;
                    case SDLK_LEFT:                     // Move left
                        offset_x += scroll_speed;
                        break;
                    case SDLK_RIGHT:                    // Move right
                        offset_x -= scroll_speed;
                        break;
                    case SDLK_UP:                       // Move up
                        offset_y += scroll_speed;
                        break;
                    case SDLK_DOWN:                     // Move down
                        offset_y -= scroll_speed;
                        break;
                    case SDLK_r:                        // Refresh view
                        offset_x = 0;
                        offset_y = 0;
                        zoom = 1.0f;
                        break;
                    case SDLK_x:
                        goto exit;
                        break;
                }
            }
        }

        // Background renderer color
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 100);
        SDL_RenderClear(renderer);

        // Get current window size
        int win_w, win_h;
        SDL_GetWindowSize(window, &win_w, &win_h);

        // Calculate scaled image size
        int render_w = (int)(img_width * zoom);
        int render_h = (int)(img_height * zoom);

        // Calculate destination rectangle (centered + offset)
        SDL_Rect destRect = {
            .x = (win_w - render_w) / 2 + offset_x,
            .y = (win_h - render_h) / 2 + offset_y,
            .w = render_w,
            .h = render_h
        };

        // Render image to screen
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
        SDL_RenderPresent(renderer);
    }
    
    // Cleanup resources
    exit:
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();

    return 0;
}
