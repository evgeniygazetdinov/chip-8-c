#ifndef PREPARE_H
#define PREPARE_H
#include <stdint.h>
#include <stdio.h>

extern uint8_t memory[4096];
extern uint8_t fontset[80];

void set_zeros_into_memory();
void set_values_into_memory();
void initialize_memory();
void upload_file_to_memory(const char* filename);
#endif
