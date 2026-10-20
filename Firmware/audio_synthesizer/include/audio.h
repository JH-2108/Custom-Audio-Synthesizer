#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>
#include <stdint.h> 

typedef enum { 
    INSTRUMENT_PIANO = 0, 
    INSTRUMENT_ORGAN,
    INSTRUMENT_SYNTH, 
    INSTRUMENT_COUNT
} Instrument; 

void audio_init(void); 
void audio_set_note(uint8_t midi_note, bool pressed); 
void audio_set_volume(uint16_t volume); 
void audio_set_instrument(Instrument instrument); 

#endif