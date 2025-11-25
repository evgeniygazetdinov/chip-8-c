#include <stdint.h>


uint16_t decript(int data_for_decript);
void do_instruct(uint16_t instruction);
void decrease_timers();
void update_system_condition();
void main_chip_8_loop(int value);