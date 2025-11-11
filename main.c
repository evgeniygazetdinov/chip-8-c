#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "core.h"

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Ошибка инициализации SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() < 0) {
        printf("Ошибка инициализации SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Hello World",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        printf("Ошибка создания окна: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        printf("Ошибка создания рендерера: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 48);
    if (!font) {
        font = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSans.ttf", 48);
    }
    if (!font) {
        printf("Не удалось загрузить шрифт: %s\n", TTF_GetError());
        printf("Установите шрифты: sudo apt install fonts-freefont-ttf\n");
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 40, 44, 52, 255); 

        if (font) {
            SDL_Color white = {255, 255, 255, 255};
            SDL_Surface* text_surface = TTF_RenderText_Blended(font, "Hello, SDL2 World!", white);
            
            if (text_surface) {
                SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
                
                if (text_texture) {
                    int text_width = text_surface->w;
                    int text_height = text_surface->h;
                    
                    SDL_Rect text_rect = {
                        (800 - text_width) / 2,
                        (600 - text_height) / 2,
                        text_width,
                        text_height
                    };
                    
                    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
                    SDL_DestroyTexture(text_texture);
                }
                
                SDL_FreeSurface(text_surface);
            }
        } else {
            SDL_Rect rect = {300, 250, 200, 100};
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }

        SDL_RenderPresent(renderer);
        main_chip_8_loop(1);
        SDL_Delay(16); // ~60 FPS
    }

    if (font) TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}