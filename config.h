#ifndef CONFIG_H
#define CONFIG_H

#define POT_ADDR AD5274_BASE_ADDR_VDD
#define TIP_PIN  PORTB3 // Arduino pin 11
#define RING_PIN PORTB2 // Arduino pin 10
#define CMD_DURATION 100
#define INPUT_ANALOG A0
#define INPUT_SAMPLES 8
#define INPUT_SAMPLE_INTERVAL 30

extern Systronix_AD5274 my_ad5274;

#endif
