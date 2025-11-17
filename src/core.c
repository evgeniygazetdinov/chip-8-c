#include <stdio.h>
#include "core.h"
#include "prepare.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>


uint16_t PC = 0x200;
const char* filename = "test_ibm_rom.ch8";
bool chip_8_running = true;
uint8_t display[64*32];


uint16_t decript(int data_for_decript) {
    uint16_t command = (memory[PC]<< 8) | memory[PC+1];
    return command;
}

void do_instruct(uint16_t instruction){
    uint16_t nnn = instruction & 0x0FFF; // address (12bit)
    uint16_t kk = instruction & 0x00FF; // Byte (8 bit)
    uint16_t n = instruction & 0x000F; // halfbyte (4 bit)
    uint16_t x = (instruction & 0x0F00) >> 8; // register x
    uint16_t y = (instruction & 0x00F0) >> 4; // register y

    // ============decode and execute====================

    switch (instruction & 0xF000) // check first 4 bits
    {
    case 0x000:
        switch (instruction)
        {
        case 0x00E0: // clear display
            memset(display, 0, sizeof(display));
            PC += 2;
            break;
        
        default:
            break;
        }
        break;
    
    default:
        break;
    }

}
void decrease_timers(){

}
void move_pointer_by_next_instruct(){

}
void update_system_condition(){
    decrease_timers();
    move_pointer_by_next_instruct();
}


void main_chip_8_loop(int value){
    
    initialize_memory();
    upload_file_to_memory(filename);
    
    
    while(chip_8_running){

    // begin from PC init value as 512 after moving by plan

    uint16_t decripted_data = decript(value);
    do_instruct(decripted_data);
    update_system_condition();
    chip_8_running = false;
    }
}
