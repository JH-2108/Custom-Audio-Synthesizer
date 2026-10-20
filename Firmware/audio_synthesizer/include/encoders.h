#ifndef ENCODERS_H
#define ENCODERS_H

#include <stdbool.h>
#include <stdint.h>

typedef enum { 
    ENCODER_NONE=0, 
    ENCODER_1,
    ENCODER_2
} EncoderId; 

typedef enum { 
    ENCODER_EVENT_NONE = 0, 
    ENCODER_EVENT_ROTATE, 
    ENCODER_EVENT_PRESS
} EncoderEventType; 

typedef struct { 
    EncoderId encoder; 
    EncoderEventType type; 
    int8_t direction; 
} EncoderEvent; 

void encoders_init(void); 
bool encoders_get_event(EncoderEvent *event); 

#endif