#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <SDL2/SDL.h>


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
    system("speaker-test -t sine -f 800 &"); // Запуск в фоне
}