#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>

void fatal_error(const char* format, ...) {
    fprintf(stderr, "FATAL ERROR: ");
    
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}

int should_update_timers(){
    static __uint32_t last_time = 0;
    __uint32_t current_time = SDL_GetTicks();
    if((current_time - last_time) >= (1000/60)){
        last_time = current_time;
        return 1;
    }
    return 0;
}
void play_beep() {
    system("speaker-test -t sine -f 800 &");
}

void sdl_run(bool running,SDL_Event event, SDL_Renderer *renderer,TTF_Font *font){
        while (running)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }
                if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        running = false;
                    }
                }
            }

            SDL_SetRenderDrawColor(renderer, 40, 44, 52, 255);

            if (font)
            {
                SDL_Color white = {255, 255, 255, 255};
                SDL_Surface *text_surface = TTF_RenderText_Blended(font, "Hello, SDL2 World!", white);

                if (text_surface)
                {
                    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

                    if (text_texture)
                    {
                        int text_width = text_surface->w;
                        int text_height = text_surface->h;

                        SDL_Rect text_rect = {
                            (800 - text_width) / 2,
                            (600 - text_height) / 2,
                            text_width,
                            text_height};

                        SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
                        SDL_DestroyTexture(text_texture);
                    }

                    SDL_FreeSurface(text_surface);
                }
            }
            else
            {
                SDL_Rect rect = {300, 250, 200, 100};
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderFillRect(renderer, &rect);
            }

            SDL_RenderPresent(renderer);
            SDL_Delay(16); // ~60 FPS
        
        }
    }