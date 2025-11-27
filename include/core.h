#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
uint16_t decript();
void do_instruct();
void decrease_timers();
void main_chip_8_loop(SDL_Renderer *renderer,TTF_Font *font);