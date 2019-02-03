#include <Systronix_AD5274.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include "config.h"

#define KINDA_EQUAL(A, B) ((A-B) < INPUT_SWAY || (B-A) < INPUT_SWAY)

bool autoIncrement = false;
uint16_t currentOutput = 0;
uint16_t incrementStep = 1;
uint16_t cmdDuration = CMD_DURATION;

const char* helpString = R"(
Use one of the following:
  a - read analog input
  i - autoincrement output on / off
  p - print current state
  t - send tip output
  r - send tip-ring output
  v - new output value
  s - new increment step
  c - command duration
  h - get help
)";

Systronix_AD5274 my_ad5274(POT_ADDR);

void setup()
{
    DDRB = DDRB | B00001100;
    Serial.begin(115200);     // use max baud rate
    while((!Serial) && (millis()<10000));
	Serial.print("Command scanner for Pioneer AppRadio.\nAD5272 at 0x");
    Serial.println(my_ad5274.BaseAddr, HEX);

    int8_t status = 0;
    int16_t read_from_ad5274 = 0;
    my_ad5274.begin();
	status = my_ad5274.control_write_verified(0x0);
	status += my_ad5274.control_write_verified(AD5274_RDAC_WIPER_WRITE_ENABLE);
	if (status > 0)
	{
		Serial.print(" Error count after control writes=");
		Serial.println(status);
	}
	// unlock RDAC
	status = my_ad5274.command_write(AD5274_CONTROL_WRITE, AD5274_RDAC_WIPER_WRITE_ENABLE);
	read_from_ad5274 = my_ad5274.command_read(AD5274_CONTROL_READ, 0x00);
	if (read_from_ad5274 & AD5274_RDAC_WIPER_WRITE_ENABLE)
	{
		Serial.print("RDAC unlock successful: ");
		Serial.println(read_from_ad5274);
	}
    else
	{
		Serial.print("RDAC unlock failed: ");
		Serial.println(read_from_ad5274);
	}
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    uint16_t inputCmd = getInputAnalog();
    handleInputCmd(inputCmd);

    // if (! KINDA_EQUAL(inputCmd, INPUT_IDLE_VALUE)) {
    // }
    if (Serial.available() != 0) {
        Serial.print("Command? > ");
        char cmd = Serial.read();
        Serial.println();
        handleInteractiveAction(cmd);
    }
}

void handleInputCmd(uint16_t value) {
    if (KINDA_EQUAL(value, IN_VOLUME_UP)) {
        sendCommand(OUT_VOLUME_UP, false, cmdDuration);
    }
    if (KINDA_EQUAL(value, IN_VOLUME_DOWN)) {
        sendCommand(OUT_VOLUME_DOWN, false, cmdDuration);
    }
    if (KINDA_EQUAL(value, IN_ARROW_UP)) {
        sendCommand(OUT_NEXT_TRACK, false, cmdDuration);
    }
    if (KINDA_EQUAL(value, IN_ARROW_DOWN)) {
        sendCommand(OUT_PREV_TRACK, false, cmdDuration);
    }
    if (KINDA_EQUAL(value, IN_MODE)) {
        sendCommand(OUT_SWITCH_SOURCE, false, cmdDuration);
    }
}

uint16_t getInputAnalog() {
    uint16_t avg = 0;
    for(byte i = 0; i < INPUT_SAMPLES; i++) {
        avg += analogRead(INPUT_ANALOG);
        delay(INPUT_SAMPLE_INTERVAL);
    }
    return (avg / INPUT_SAMPLES);
}

void sendCommand(uint16_t value, bool useRing, uint16_t duration) {
    digitalWrite(LED_BUILTIN, HIGH);
    // set wiper
    my_ad5274.command_write(AD5274_RDAC_WRITE, value);

    // open gates
    PORTB |= ((1 << TIP_PIN) + ((useRing ? 1 : 0) << RING_PIN));
    
    delay(duration);
    
    //close gates
    PORTB &= ~((1 << TIP_PIN) + ((useRing ? 1 : 0) << RING_PIN));
    digitalWrite(LED_BUILTIN, LOW);
}

void handleInteractiveAction(char cmd) {
    switch(cmd) {
        case 'h':
            Serial.println(helpString);
            break;
        case 'a':
            Serial.println(getInputAnalog());
            break;
        case 'i':
            autoIncrement = !autoIncrement;
            Serial.print("Auto-increment is now: ");
            Serial.println(autoIncrement ? "on":"off");
            break;
        case 'p':
            Serial.print("Output: ");
            Serial.print(currentOutput, DEC);
            Serial.print(" | Duration: ");
            Serial.print(cmdDuration, DEC);
            Serial.print(" | Increment step: ");
            Serial.print(incrementStep, DEC);
            Serial.print(" | Auto-increment: ");
            Serial.println(autoIncrement ? "on":"off");
            break;
        case 't':
            sendCommand(currentOutput, false, cmdDuration);
            if(autoIncrement) {
                currentOutput += incrementStep;
                if (currentOutput > 1023)
                    currentOutput = 0;
                Serial.println("New output value is: " + String(currentOutput));
            }
            break;
        case 'r':
            sendCommand(currentOutput, true, cmdDuration);
            if(autoIncrement) {
                currentOutput += incrementStep;
                if (currentOutput > 1023)
                    currentOutput = 0;
                Serial.println("New output value is: " + String(currentOutput));
            }
            break;
        case 'v':
            Serial.print(F("New output value? > "));
            while(Serial.available() == 0);
            currentOutput = Serial.parseInt();
            if (currentOutput > 1023)
                    currentOutput = 1023;
            Serial.println("\nOutput value set to " + String(currentOutput));
            break;
        case 's':
            Serial.print(F("New increment step value? > "));
            while(Serial.available() == 0);
            incrementStep = Serial.parseInt();
            Serial.println("\nIncrement step set to " + String(incrementStep));
            break;
        case 'c':
            Serial.print(F("New command duration value? > "));
            while(Serial.available() == 0);
            cmdDuration = Serial.parseInt();
            Serial.println("\nIncrement command duration to " + String(cmdDuration));
            break;
    }
}
