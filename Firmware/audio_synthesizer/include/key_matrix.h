#ifndef KEY_MATRIX_H
#define KEY_MATRIX_H 

#include <stdbool.h> 
#include <stdint.h> 

#define KEY_MATRIX_ROW 4
#define KEY_MATRIX_ROW 4

typedef enum { 
    KEY_C = 0, 
    KEY_C_SHARP, 
    KEY_D, 
    KEY_D_SHARP, 
    KEY_E, 
    KEY_F, 
    KEY_F_SHARP, 
    KEY_G, 
    KEY_G_SHARP, 
    KEY_A, 
    KEY_A_SHARP,
    KEY_B
    KEY_C_HIGH,

    CONTROL_OCTAVE_DOWN, 
    CONTROL_OCTAVE_UP, 
    CONTROL_CHANGE_INSTRUMENT
}KeyId;

void key_matrix_init(void); 
void key_matrix_scan(void); 
bool key_matrix_is_pressed(uint8_t row, uint8_t col); 
KeyId key_matrix_get_key_id(uint8_t row, uint8_t col);

#endif 