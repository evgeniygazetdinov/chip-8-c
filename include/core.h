#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdint.h>
uint16_t decript();
void do_instruct();
void decrease_timers();
void main_chip_8_loop(SDL_Renderer* renderer, TTF_Font* font);
void sdl_run(bool running, SDL_Event event, SDL_Renderer* renderer,
             TTF_Font* font);