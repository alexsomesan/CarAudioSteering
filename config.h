#ifndef CONFIG_H
#define CONFIG_H

// #define ENABLE_FASTADC
#define FASTADC_WITHOUT_TIMER1

#define POT_ADDR AD5274_BASE_ADDR_VDD
#define TIP_PIN PORTB3  // Arduino pin 11
#define RING_PIN PORTB2 // Arduino pin 10

#ifndef ENABLE_FASTADC
// Arduino
#define INPUT_ANALOG A0
#else
// FastADC
#define INPUT_ANALOG 0
#endif

#define CMD_DURATION 100
#define INPUT_SAMPLES 8
#define INPUT_SAMPLE_INTERVAL 30
#define INPUT_SWAY 3

//Lancer
// #define INPUT_IDLE_VALUE 656

// Lancer
// #define IN_VOLUME_UP 372
// #define IN_VOLUME_DOWN 386
// #define IN_ARROW_UP 353
// #define IN_ARROW_DOWN 361
// #define IN_MODE 346
// #define IN_VOICE_CND 405
// #define IN_PHONE_PCKUP 425
// #define IN_PHONE_HGUP 451

// Test
#define INPUT_IDLE_VALUE 1023

#define IN_VOLUME_UP 205
#define IN_VOLUME_DOWN 339
#define IN_ARROW_UP 450
#define IN_ARROW_DOWN 561
#define IN_MODE 651
#define IN_VOICE_CND 732
#define IN_PHONE_PCKUP 790
#define IN_PHONE_HGUP 842

// Pioneer SPH-DA 120
#define OUT_SWITCH_SOURCE 13
#define OUT_MUTE_ATT 35
#define OUT_NEXT_TRACK 70
#define OUT_PREV_TRACK 105
#define OUT_VOLUME_UP 160
#define OUT_VOLUME_DOWN 240
#define OUT_RADIO_BAND 450

#endif
