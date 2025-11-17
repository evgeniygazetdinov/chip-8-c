#include <stdio.h>
#include "core.h"
#include "prepare.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

uint16_t PC = 0x200; // program counter
const char *filename = "test_ibm_rom.ch8";
bool chip_8_running = true;
uint8_t display[64 * 32];

uint16_t IND; // index register
uint16_t stack[16];
uint16_t SP;    // stack pointer
uint16_t V[16]; // registers V0-VF

uint16_t decript(int data_for_decript)
{
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
        if (V[x] == kk){
            PC += 2;
        }
        PC += 2;
        break;
    case 0x4000:
        if (V[x] != kk){
            PC += 2;
        }
        PC += 2;
        break;
    case 0x5000:
        if (V[x] == V[y]){
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
        switch (n){
        case 0x0:
            V[x] = V[y];
            break;
        case 0x1:
            V[x] |= V[y]; //set VX=VX or VY -8xy1
            break;
        case 0x2:
            V[x] &= V[y]; // set VX=VX and VY - 8xy2
            break;
        case 0x3:
            V[x] ^= V[y]; // set VX=VX XOR VY - 8xy3
            break;
        case 0x4:
            V[0xF] = (V[x] + V[y] > 0xFF) ? 1 : 0; //SET VX=VX+VY VF=move 8xy4
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
        PC+=2;
        break;
    }
    }
    void decrease_timers()
    {
    }
    void move_pointer_by_next_instruct()
    {
    }
    void update_system_condition()
    {
        decrease_timers();
        move_pointer_by_next_instruct();
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
            chip_8_running = false;
        }
    }
