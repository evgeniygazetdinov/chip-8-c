#include <stdio.h>
#include "core.h"
#include "prepare.h"
#include "helpers.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 


uint16_t PC = 0x200; // program counter
const char *filename = "test_ibm_rom.ch8";
bool chip_8_running = true;
uint8_t display[64 * 32];
uint8_t delay_timer;
uint8_t sound_timer;

uint16_t IND; // index register
uint16_t stack[16];
uint16_t SP;       // stack pointer
uint16_t V[16];    // registers V0-VF
uint16_t keys[16]; // keys conditioins


uint16_t decript(int data_for_decript)
{
    printf("PC = 0x%04X\n", PC);  // what now in PC in HEX format

    uint16_t command = (memory[PC] << 8) | memory[PC + 1];
    return command;
}

void do_instruct(uint16_t instruction)
{
    uint16_t nnn = instruction & 0x0FFF;      // address (12bit)
    uint16_t kk = instruction & 0x00FF;       // Byte (8 bit)
    uint16_t n = instruction & 0x000F;        // halfbyte (4 bit)
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

        case 0x00EE:
            SP--;
            PC = stack[SP];
            PC += 2;

        default:
            printf("Unknown instructions 0x%04X\n", instruction);
            PC += 2;
        }
        break;

    case 0x1000: // moving to adress nnn
        PC = nnn;
        break;

    case 0x2000: // caling subprogram via adress nnn
        stack[SP] = PC;
        SP++;
        PC = nnn;
        break;

    case 0x3000:
        if (V[x] == kk)
        {
            PC += 2;
        }
        PC += 2;
        break;
    case 0x4000:
        if (V[x] != kk)
        {
            PC += 2;
        }
        PC += 2;
        break;
    case 0x5000:
        if (V[x] == V[y])
        {
            PC += 2;
        }
        PC += 2;
        break;

    case 0x6000:
        V[x] == kk;
        PC += 2;
        break;

    case 0x7000:
        V[x] += kk;
        PC += 2;
        break;

    case 0x8000:
        switch (n)
        {
        case 0x0:
            V[x] = V[y];
            break;
        case 0x1:
            V[x] |= V[y]; // set VX=VX or VY -8xy1
            break;
        case 0x2:
            V[x] &= V[y]; // set VX=VX and VY - 8xy2
            break;
        case 0x3:
            V[x] ^= V[y]; // set VX=VX XOR VY - 8xy3
            break;
        case 0x4:
            V[0xF] = (V[x] + V[y] > 0xFF) ? 1 : 0; // SET VX=VX+VY VF=move 8xy4
            V[x] += V[y];
            break;
        case 0x5:
            V[0xF] = (V[x] > V[y]) ? 1 : 0; // 8xy5 set VX=VX+XY set VF= not zaem
            V[x] -= V[y];
            break;
        case 0x6:
            V[0xF] = V[x] & 0x1; // 8xy6  move VX to left by 1 /VF yound bit before sdvig
            V[x] >>= 1;
            break;
        case 0x7:
            V[0xF] = (V[y] > V[x]) ? 1 : 0; // 8xy7 set Vx=VY-VX set vf = not zaem
            V[x] = V[y] - V[x];
            break;
        case 0xE:
            V[0xF] = (V[x] >> 7) & 0x1; // 8xyE moving VX left by one, VF=ELDER bit before sdvig
            V[x] <<= 1;
            break;
        default:
            printf("Unknown operation: 0x%04X\n", instruction);
        }
        PC += 2;
        break;
    // 9xy0: Pass next instruct if vx!=vy
    case 0x9000:
        if (V[x] != V[y])
        {
            PC += 2;
        }
        PC += 2;
        break;
    case 0xA000:
        IND = nnn;
        PC += 2;
        break;
    // Cxkk: set VX random number and kk
    case 0xB000:
        PC = nnn + V[0];
        break;
    case 0XC000:
        V[x] = (rand() % 256) & kk;
        PC += 2;
        break;
    // Draw conditions
    case 0xD000:
    {
        uint16_t x_cord = V[x] % 64;
        uint16_t y_coord = V[y] % 32;
        V[0xF] = 0;
        for (int row = 0; row < n; row++)
        {
            uint8_t sprite_byte = memory[IND + row];
            for (int col = 0; col < 8; col++)
            {
                if ((sprite_byte & (0x80 >> col)) != 0)
                {
                    int display_x = (x_cord + col) % 64;
                    int display_y = (y_coord + row) % 32;
                    int pixel_index = display_y * 64 + display_x;
                    if (display[pixel_index] == 1)
                    {
                        V[0xF] = 1;
                    }
                    display[pixel_index] ^= 1;
                }
            }
        }
        PC += 2;
        break;
    }
    // Group Exxx: Operation input
    case 0xE000:
        switch (kk)
        {
        case 0x9E:
            if (keys[V[x]] != 0)
            {
                PC += 2;
            }
            PC += 2;
            break;
        case 0xA1:
            if (keys[V[x]] == 0)
            {
                PC += 2;
            }
            PC += 2;
            break;
        default:
            printf("unknown instruction 0X%04X\n", instruction);
        }
    case 0xF000:
        switch (kk)
        {
        case 0x07: // Fx07: Установить Vx = значение таймера задержки
            V[x] = delay_timer;
            break;

        case 0x0A:
        { // Fx0A: waiting for key pressed
            int key_pressed = 0;
            for (int i = 0; i < 16; i++)
            {
                if (keys[i] != 0)
                {
                    V[x] = i;
                    key_pressed = 1;
                    break;
                }
            }
            if (!key_pressed)
            {
                // if key is not push try again
                return;
            }
            break;
        }

        case 0x15: // Fx15: Установить таймер задержки = Vx
            delay_timer = V[x];
            break;

        case 0x18: // Fx18: Установить звуковой таймер = Vx
            sound_timer = V[x];
            break;

        case 0x1E: // Fx1E: Установить I = I + Vx
            IND += V[x];
            break;

        case 0x29:                   // Fx29: set  IND = adreress form for symbol Vx
            IND = 0x50 + (V[x] * 5); // fonts begin from 0x50, each by 5 bites
            break;

        case 0x33: // Fx33: SAVE BCD into memory
            memory[IND] = V[x] / 100;
            memory[IND + 1] = (V[x] / 10) % 10;
            memory[IND + 2] = V[x] % 10;
            break;

        case 0x55: // Fx55: save V0-Vx into memory
            for (int i = 0; i <= x; i++)
            {
                memory[IND + i] = V[i];
            }
            break;

        case 0x65: // Fx65: load V0-Vx from memory
            for (int i = 0; i <= x; i++)
            {
                V[i] = memory[IND + i];
            }
            break;

        default:
            printf("Unknown instruction Fxxx: 0x%04X\n", instruction);
        }
        PC += 2;
        break;

    default:
        printf("Unknown instruction: 0x%04X\n", instruction);
        PC += 2;
    }
}
void decrease_timers()
{
    if(delay_timer > 0){
        delay_timer--;
    } 
    if(sound_timer >0){
        sound_timer--;
        if(sound_timer > 0){
            play_beep();
        }
    }

}
void update_system_condition()
{
    if (should_update_timers())
    {
        decrease_timers();
    }
}

void main_chip_8_loop(int value)
{

    initialize_memory();
    upload_file_to_memory(filename);

    while (chip_8_running)
    {

        // begin from PC init value as 512 after moving by plan

        uint16_t decripted_data = decript(value);
        do_instruct(decripted_data);
        update_system_condition();
        //chip_8_running = false;
        sleep(1);

    }
}
