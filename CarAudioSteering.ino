#include <FastADC.h>
#include <Systronix_AD5274.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include "config.h"

#define KINDA_EQUALS(A, B) ((A - B) < INPUT_SWAY || (B - A) < INPUT_SWAY)

enum RUN_STATES
{
    RUN_IDLE,
    RUN_BUTTON_PRESSED
};

byte runState = RUN_IDLE;

bool autoIncrement = false;
uint16_t currentOutput = 0;
uint16_t incrementStep = 1;
uint16_t cmdDuration = CMD_DURATION;

const char *helpString = R"(
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

Systronix_AD5274 digipot(POT_ADDR);
FastADC(analog_in, INPUT_ANALOG, true);

void setup()
{
    DDRB = DDRB | B00001100;
    analog_in.reference(INPUT_ANALOG, INTERNAL);
    Serial.begin(115200); // use max baud rate
    while ((!Serial) && (millis() < 10000))
        ;
    Serial.println("Car audio steering. Hello :)");

    int8_t status = 0;
    int16_t read_from_ad5274 = 0;
    digipot.begin();
    status = digipot.control_write_verified(0x0);
    status += digipot.control_write_verified(AD5274_RDAC_WIPER_WRITE_ENABLE);
    if (status > 0)
    {
        Serial.print(" Error count after control writes=");
        Serial.println(status);
    }
    // unlock RDAC
    status = digipot.command_write(AD5274_CONTROL_WRITE, AD5274_RDAC_WIPER_WRITE_ENABLE);
    read_from_ad5274 = digipot.command_read(AD5274_CONTROL_READ, 0x00);
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
    uint16_t inputCmd = analog_in.read(INPUT_ANALOG); //analogRead(INPUT_ANALOG);
    switch (runState)
    {
    case RUN_IDLE:
        if (handleInputCmd(inputCmd))
        {
            runState = RUN_BUTTON_PRESSED;
        }
        // Handle interactive activity
        if (Serial.available() != 0)
        {
            Serial.print("Command? > ");
            char cmd = Serial.read();
            Serial.println();
            handleInteractiveAction(cmd);
        }
        break;
    case RUN_BUTTON_PRESSED:
        if (KINDA_EQUALS(inputCmd, INPUT_IDLE_VALUE))
        {
            endCommand();
            runState = RUN_IDLE;
        }
        break;
    default:
        break;
    }
}

bool handleInputCmd(uint16_t value)
{
    if (KINDA_EQUALS(value, IN_VOLUME_UP))
    {
        startCommand(OUT_VOLUME_UP, false);
        return true;
    }
    if (KINDA_EQUALS(value, IN_VOLUME_DOWN))
    {
        startCommand(OUT_VOLUME_DOWN, false);
        return true;
    }
    if (KINDA_EQUALS(value, IN_ARROW_UP))
    {
        startCommand(OUT_NEXT_TRACK, false);
        return true;
    }
    if (KINDA_EQUALS(value, IN_ARROW_DOWN))
    {
        startCommand(OUT_PREV_TRACK, false);
        return true;
    }
    if (KINDA_EQUALS(value, IN_MODE))
    {
        startCommand(OUT_SWITCH_SOURCE, false);
        return true;
    }
    return false;
}

uint16_t getInputAnalog()
{
    uint16_t avg = 0;
    for (byte i = 0; i < INPUT_SAMPLES; i++)
    {
        avg += analogRead(INPUT_ANALOG);
        delay(INPUT_SAMPLE_INTERVAL);
    }
    return (avg / INPUT_SAMPLES);
}

void sendCommand(uint16_t value, bool useRing, uint16_t duration)
{
    digitalWrite(LED_BUILTIN, HIGH);
    startCommand(value, useRing);
    delay(duration);
    endCommand();
    digitalWrite(LED_BUILTIN, LOW);
}

void startCommand(uint16_t value, bool useRing)
{
    // set wiper
    digipot.command_write(AD5274_RDAC_WRITE, value);

    // open gates
    PORTB |= ((1 << TIP_PIN) + ((useRing ? 1 : 0) << RING_PIN));
}

void endCommand()
{
    //close gates
    PORTB &= ~((1 << TIP_PIN) + (1 << RING_PIN));

    // set wiper to max
    digipot.command_write(AD5274_RDAC_WRITE, 1023);
}

void handleInteractiveAction(char cmd)
{
    switch (cmd)
    {
    case 'h':
        Serial.println(helpString);
        break;
    case 'a':
        Serial.println(getInputAnalog());
        break;
    case 'i':
        autoIncrement = !autoIncrement;
        Serial.print("Auto-increment is now: ");
        Serial.println(autoIncrement ? "on" : "off");
        break;
    case 'p':
        Serial.print("Output: ");
        Serial.print(currentOutput, DEC);
        Serial.print(" | Duration: ");
        Serial.print(cmdDuration, DEC);
        Serial.print(" | Increment step: ");
        Serial.print(incrementStep, DEC);
        Serial.print(" | Auto-increment: ");
        Serial.println(autoIncrement ? "on" : "off");
        break;
    case 't':
        sendCommand(currentOutput, false, cmdDuration);
        if (autoIncrement)
        {
            currentOutput += incrementStep;
            if (currentOutput > 1023)
                currentOutput = 0;
            Serial.println("New output value is: " + String(currentOutput));
        }
        break;
    case 'r':
        sendCommand(currentOutput, true, cmdDuration);
        if (autoIncrement)
        {
            currentOutput += incrementStep;
            if (currentOutput > 1023)
                currentOutput = 0;
            Serial.println("New output value is: " + String(currentOutput));
        }
        break;
    case 'v':
        Serial.print(F("New output value? > "));
        while (Serial.available() == 0)
            ;
        currentOutput = Serial.parseInt();
        if (currentOutput > 1023)
            currentOutput = 1023;
        Serial.println("\nOutput value set to " + String(currentOutput));
        break;
    case 's':
        Serial.print(F("New increment step value? > "));
        while (Serial.available() == 0)
            ;
        incrementStep = Serial.parseInt();
        Serial.println("\nIncrement step set to " + String(incrementStep));
        break;
    case 'c':
        Serial.print(F("New command duration value? > "));
        while (Serial.available() == 0)
            ;
        cmdDuration = Serial.parseInt();
        Serial.println("\nIncrement command duration to " + String(cmdDuration));
        break;
    }
}
