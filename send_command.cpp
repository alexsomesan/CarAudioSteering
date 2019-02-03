#include "send_command.h"
#include <Arduino.h>
#include <avr/io.h>
#include <Systronix_AD5274.h>

#include "config.h"

void sendCommand(uint16_t value, bool useRing) {
    digitalWrite(LED_BUILTIN, HIGH);
    // set wiper
    my_ad5274.command_write(AD5274_RDAC_WRITE, value);

    // open gates
    PORTB |= ((1 << TIP_PIN) + ((useRing ? 1 : 0) << RING_PIN));
    
    delay(CMD_DURATION);
    
    //close gates
    PORTB &= ~((1 << TIP_PIN) + ((useRing ? 1 : 0) << RING_PIN));
    digitalWrite(LED_BUILTIN, LOW);
}
