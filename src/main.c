#include "core.h"
#include "helpers.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("error init SDL: %s\n", SDL_GetError());

        return 1;
    }

    if (TTF_Init() < 0)
    {
        printf("error init SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window =
        SDL_CreateWindow("some message", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

    if (!window)
    {
        printf("Window error creation: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer)
    {
        printf("error render creation: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* font =
        TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 48);
    if (!font)
    {
        font = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSans.ttf", 48);
    }
    if (!font)
    {
        printf("cant load font: %s\n", TTF_GetError());
        printf("install fonts: sudo apt install fonts-freefont-ttf\n");
    }
    main_chip_8_loop(renderer, font);
    if (font)
        TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}