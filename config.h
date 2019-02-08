#ifndef CONFIG_H
#define CONFIG_H

// #define ENABLE_FASTADC
#define FASTADC_WITHOUT_TIMER1

// #define INPUT_TYPE_TEST
#define INPUT_TYPE_LANCER

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
#define INPUT_SAMPLES 4
#define INPUT_SAMPLE_INTERVAL 10
#define INPUT_SWAY 3

// Lancer
#ifdef INPUT_TYPE_LANCER
#define INPUT_IDLE_VALUE 646

#define IN_VOLUME_UP 357
#define IN_VOLUME_DOWN 371
#define IN_ARROW_UP 337
#define IN_ARROW_DOWN 345
#define IN_MODE 330
#define IN_VOICE_CMD 390
#define IN_PHONE_PCKUP 410
#define IN_PHONE_HGUP 437
#endif

// Test
#ifdef INPUT_TYPE_TEST
#define INPUT_IDLE_VALUE 1023

#define IN_VOLUME_UP 207
#define IN_VOLUME_DOWN 339
#define IN_ARROW_UP 449
#define IN_ARROW_DOWN 560
#define IN_MODE 649
#define IN_VOICE_CMD 732
#define IN_PHONE_PCKUP 790
#define IN_PHONE_HGUP 842
#endif

// Pioneer SPH-DA 120
#define OUT_SWITCH_SOURCE 13
#define OUT_MUTE_ATT 35
#define OUT_NEXT_TRACK 70
#define OUT_PREV_TRACK 105
#define OUT_VOLUME_UP 160
#define OUT_VOLUME_DOWN 240
#define OUT_RADIO_BAND 450

#endif
