cmake_minimum_required(VERSION 3.13) 

include(pico_sdk_import.cmake) 

project(CustomAudioSynthesizer C CXX ASM) 

set(CMAKE_C_STANDARD 11) 
set(CMAKE_CXX_STANDARD 17) 

pico_sdk_init() 

add_executable(custom_audio_synthesizeer 
    src/main.c
)

target_include_directories(custom_audio_synthesizer PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/include     
)

target_link_libraries(custom_audio_synthesizer
    pico_stdlib
    hardware_adc
    hardware_gpio 
    hardware_pwm
    hardware_i2c
    m
)

add_executable(custom_audio_synthesizer 
    src/main.c
    src/key_matrix.c
    src/audio.c
    src/encoders.c
    src/display.c
)

pico_enable_stdio_usb(custom_audio_synthesizer 1) 
pico_enable_stdio_uart(custom_audido_synthesizer 0) 

pico_add_extra_outputs(custom_audio_synthesizer)

