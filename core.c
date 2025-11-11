#include <stdio.h>


int decript(int data_for_decript) {
    return 1;
}

void do_instruct(int instruction){

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
    int decripted_data;
    decripted_data = decript(value);
    do_instruct(decripted_data);
    update_system_condition();

    printf("core file included corect values is %d\n", value);

}