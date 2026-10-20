#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h> 
#include "audio.h"

void display_init(void); 
void display_update(uint8_t midi_note,
                    int octave, 
                    Instrument instrument, 
                    uint16_t volume); 

void display_init(void); 
void display_show_octave(int octave); 
void display_show_volume(uint16_t volume); 
void display_show_instrument(Instrument instrument); 
void display_update(void); 

#endif