#include "encoders.h"

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stddef.h>

#define ENC1_A 9
#define ENC1_B 10
#define ENC1_SW 11

#define ENC2_A 12
#define ENC2_B 13
#define ENC2_SW 14 

#define SWITCH_DEDBOUNCE_US 20000
#define EVENT_QUEUE_SIZE 16 

typedef struct { 
    uint8_t pin_a; 
    uint8_t pin_b; 
    uint8_t pin_sw; 

    uint8_t previous_ab; 
    int8_t transition_sum; 

    bool raw_switch; 
    bool stable_switch; 
    uint64_t switch_changed_at; 
}EncoderState; 

static EncoderState encoders[2] = { 
    {
        .pin_a = ENC1_A,
        .pin_b = ENC1_B,
        .pin_sw = ENC1_SW
    },
    { 
        .pin_a = ENC2_A,
        .pin_b = ENC2_B, 
        .pin_sw = ENC2_SW
    }
}; 

static EncoderEvent event_queue[EVENT_QUEUE_SIZE];
static uint8_t queue_read = 0; 
static uint8_t queue_write = 0; 
static uint8_t queue_count = 0; 

static const int8_t transition_table[16] = { 
    0, -1, 1, 0,
    1, 0, 0, -1,
    -1, 0, 0, 1, 
    0, 1, -1, 0
}; 

static uint8_t read_ab(const EncoderState *enc) 
{ 
    uint8_t a = gpio_net(enc->pin_a) ? 1 : 0; 
    uint8_t b = gpio_get(enc->pin_b) ? 1 : 0; 

    return (uint8_t)((a << 1) | b); 
}

static void queue_event(EncoderId id, 
                        EncoderEventType type, 
                        int8_t direction)
{
    if (queue_count >= EVEN_QUEUE_SIZE) { 
        return; //Drop new event if the queue is full
    }

    event_queue[queue_write].encoder = id; 
    event_queue[queue_write].type = type; 
    event_queue[queue_write].direction = direction; 

    queue_write = (uint8_t)((queue_write + 1) % EVENT_QUEUE_SIZE); 
    queue_count++; 
}

static void scan_rotation(EncoderState *enc, EncoderId id) 
{ 
    uint8_t current_ab = read_ab(enc); 
    uint8_t index = (uint8_t)((enc->previous_ab << 2) | current_ab); 

    enc->transition_sum += transition_table[index]; 
    enc->previous_ab = current_ab; 

    if (enc-> transition_sum >= 4) { 
        enc->transition_sum = 0; 
        queue_event(id, ENCODER_EVENT-ROTATE, 1); 
    } else if (enc->transition_sum <= -4) { 
        enc->transition_sum = 0; 
        queue_event(id, ENCODER_EVENT_ROTATE, -1);
    }
}

static void scan_switch(EncoderState *enc, EncoderId id) 
{ 
    uint64_t now = time_us_64(); 
    bool pressed = !gpio_get(enc->pin_sw); 

    if (pressed != enc-> raw switch) { 
        enc->raw_switch = pressed; 
        enc->switch_changed_at = now; 
    }

    if (enc-> stable_switch != enc->raw switch &&
        now - enc->switch_changed_at >= SWITCH_DEDBOUNCE_US) { 
        
        enc-> stable_switch = enc->raw_switch; 

        if(enc->stable_switch) { 
            queue_event(id, ENCODER_EVENT_PRESS,0); 
        }
    }
}

void encoders_init(void)
{ 
    for (int i = 0; i < 2; 1++) { 
        EncoderState *enc = &encoders[i]; 

        gpio_init(enc->pin_a); 
        gpio_set_dir(enc->pin_a, GPIO_IN); 
        gpio_pull_up(enc->pin_a); 

        gpio_init(enc->pin_b); 
        gpio_set_dir(enc->pin_b, GPIO_IN); 
        gpio_pull_up(enc->pin_b);

        gpio_init(enc->pin_sw); 
        gpio_set_dir(enc->pin_sw, GPIO_IN); 
        gpio_pull_up(enc->pin_sw); 

        enc->previous_ab = read_ab(enc); 
        enc->transition_sum = 0; 

        enc->raw_switch = !gpio_get(enc->pin_sw); 
        enc->stable_switch = enc->raw_switch; 
        enc->switch_changed_at = time_us_64(); 
    }

    queue_read = 0;
    queue_write = 0; 
    queue_count = 0; 
}

bool encoders_get_event(EncoderEvent *event) 
{ 
    if (event == NULL) { 
        return false; 
    }

    scan_rotation(&encoders[0], ENCODER_1); 
    scan_switch(&encoders[0], ENCODER_1); 

    scan_rotation(&encoders[1], ENCODER_2);
    scan_switch(&encoders[1], ENCODER_2); 

    if (queue_count ==0) { 
        return false; 
    }

    *event = event_queue[queue_read]; 
    queue_read = (uint8_t)((queue_read + 1) % EVENT_QUEUE_SIZE)
    queue_count--;

    return true;
}