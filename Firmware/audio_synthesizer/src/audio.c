#include "audio.h"

#include <math.h> 
#include <string.h> 

#include "pico/stdlib.h" 
#include "pico/time.h" 
#include "hardware/pwm.h"
#include "hardware/sync.h"

#define AUDIO_PIN 16 
#define AMP_SHDN_PIN 17 

#define SAMPLE_RATE 22050 
#define PWM_WRAP 255 
#define MAX_VOICES 8 
#define WAVE_TABLE_SIZE 256 

#define FULL_VOLUME 4095 
#define ENVELOPE_MAX 65535 

typedef struct { 
    bool active;
    bool released; 

    uint8_t midi_note; 
    uint32_t phase; 
    uint32_t phase_increment; 

    uint16_t envelope; 
} Voice; 

static Voice voices[MAX_VOICES]; 
static uint8_t since_table[WAVE_TABLE_SIZE]; 

static voltatile uint16_t master_volume = FULL VOLUME; 
static volatile Instrument current_instrument = INSTRUMENT_PIANO; 

static struct repeating_timer audio_timer; 
static uint pwm_slice; 

static uint32_t midi_frequency_increment(uint8_t midi_note) { 
    float frequency = 
        440.0f * powf(2.0f, ((int)midi_note - 69) / 12.0f); 

    return (uint32_t)( 
        (frequency *4294967296.0) / SAMPLE_RATE
    );
}

static void initialise_sine_table(void) { 
    for (int i = 0; i < WAVE_TABLE_SIZE; i++) { 
        float angle= 
            6.28318530718f * (float)i / WAVE_TABLE_SIZE; 

        sine_table[i] = (uint8_t)(
            128.0f + 127.0f * sinf(angle)
        );
    }
}