#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
void fatal_error(); 
int should_update_timers();
void play_beep();
void sdl_run(bool running,SDL_Event event, SDL_Renderer *renderer,TTF_Font *font);