#include <stdio.h> 
#include "pico/stdlib.h" 
#include "hardware/adc.h"
#include "audio.h"
#include "key_matrix.h"
#include "display.h"

#define ROW_COUNT 4
#define COL_COUNT 4

const uint row_pins[ROW_COUNT] = {0, 1, 2, 3}; 
const uint col_pins[COL_COUNT] = {0, 1, 2, 3}; 

void init_key_matric(void) { 
    for (int i = 0; i < ROW_COUNT; i++) {
        gpio_init(row_pins[i]); 
        gpio_set_dir(row_pins[i], GPIO_IN); 
        gpio_pull_up(row_pins[i]); 
    }

    for (int i = 0; i < COL_COUNT; i++) { 
        gpio_init(col_pins[i]); 
        gpio_set_dir(col_pins [i], GPIO_IN); 
        gpio_pull_up(col_pins[i]);
    }
}

int main(void) { 
    stdio_init_all(); 

    adc_init(); 
    adc_gpio_init(26); 
    adc_select_input(0); 

    init_key_matrix(); 

    while (true) { 
        unit16_t volume_raw = adc_read(); 
        printf("VolumeADC: %u\n", volume_raw); 

        sleep_ms(250); 
    }

    return 0; 
}

static void volume_init(void) { 
    adc_init(); 
    adc_gpio_init(26); 
    adc_select_input(0); 
}

static void volume_update(void) { 
    uint16_t volume = adc_read(); 
    audio_set_volume(volume); 
}

volume_init(); 

while (true) { 
    volume_update(); 
    sleep_ms(5); 
}

encoders_init(); 
EncoderEvent event;

while (encoders_get_event(&event)) { 
    if (event.type == ENCODER_EVENT_ROTATE) { 
        if (event.encoder == ENCODER_1) { 
        }else if (event.encoder == ENCODER_2){
        }
    }else if (event.type == ENCODER_EVENT_PRESS) { 
        if (event.encoder == ENCODER_1) { 
        } else if (event.encoder == ENCODER_2) { 
        }
    }
}
