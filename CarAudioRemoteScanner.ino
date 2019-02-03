#include <Systronix_AD5274.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include "config.h"
#include "send_command.h"

uint16_t currentOutput = 0;
bool autoIncrement = false;
uint16_t incrementStep = 1;

const char* helpString = R"(
Use one of the following:
  h - get help
  a - read analog input
  i - autoincrement output on / off
  p - print current output value and step increment
  t - send tip output
  r - send tip-ring output
  v - new output value
  s - new increment step
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
    char cmd = 0;
	Serial.print("Command? > ");
    while(Serial.available() == 0);
    cmd = Serial.read();
    Serial.println();
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
            Serial.print("Current output is at: ");
            Serial.print(currentOutput, DEC);
            Serial.print(" | Current increment step is: ");
            Serial.print(incrementStep, DEC);
            Serial.print(" | Auto-increment is: ");
            Serial.println(autoIncrement ? "on":"off");
            break;
        case 't':
            sendCommand(currentOutput);
            if(autoIncrement) {
                currentOutput += incrementStep;
                if (currentOutput > 1023)
                    currentOutput = 0;
                Serial.println("New output value is: " + String(currentOutput));
            }
            break;
        case 'r':
            sendCommand(currentOutput, true);
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