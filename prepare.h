#ifndef PREPARE_H
#define PREPARE_H
#include <stdint.h>

extern uint16_t PC;
extern uint8_t memory;
extern uint8_t fontset; 

void set_zeros_into_memory();
void set_values_into_memory();
void initialize_memory();
#endif
