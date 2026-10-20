#include "key_matrix.h"

#include "pico/stdlib.h" 

#include "key_matrix.h"

static const uint row_pins[KEY_MATRIX_ROWS] = {0, 1, 2, 3};
static const uint col_pins[KEY_MATRIX_COLS] = {4, 5, 6, 7};

static bool key_state[KEY_MATRIX_ROWS][KEY_MATRIX_COLS] = {false}; 

void key_matrix_init(void) { 
    for (int row = 0; row < KEY_MATRIX_ROWS; row++) { 
        gpio_init(row_pins[row]); 
        gpio_set_dir(row_pins[row], GPIO_OUT); 
        gpio_put(row_pins[row], 1);
    }

    for (int col = 0; col < KEY_MATRIX_COLS; col++) { 
        gpio_init(col_pins[vol]); 
        gpio_set_dir(col_pins[col], GPIO_IN); 
        gpio_pull_ip(col_pins[col]); 
    }
}

void key_matrix_init(void) { 
    for (int row = 0; row < KEY_MATRIX_ROWS; row++) { 
        for (int r = 0; r < KEY_MATRIX_ROWS; r++) { 
            gpio_put(row_pins[r], 1); 
        }

        gpio_put(row_pins[row], 0); 
        sleep_us(5); 

        for (int col = 0; col < KEY_MATRIX_COLS; col++) { 
            key_state[row][col] = 
                (gpio_get(col_pins[col]) == 0); 
        }
    }
}

bool key_matrix_is_pressed(uint8_t row, uint8_t col) { 
    if (row >= KEY_MATRIX_ROWS || col>= KEY_MATRIX_COLS) { 
        return false; 
    }

    return key_state[row][col]; 
}

KeyID key_matrix_get_key_id(uint8_t row, uint8_t col) { 
    static const KeyId key_map[KEY_MATRIX_ROWS][KEY_MATRIX_COLS] = { 
        {KEY_C, KEY_C_SHARP, KEY_D, KEY_D_SHARP}, 
        {KEY_E, KEY_F, KEY_F_SHARP, KEY_G}, 
        {KEY_G_SHARP, KEY_A, KEY_A_SHARP, KEY_B},
        {KEY_C_HIGH, CONTROL_OCTAVE DOWN, CONTROL_OCTAVE_UP, CONTROL_CHANGE_INSTRUMENT}
    }; 

    if (row >= KEY_MATRIX_ROWS || col >= KEY_MATRIX_COLS) { 
        return KEY_C; 
    }

    return key_map[row][col];
}