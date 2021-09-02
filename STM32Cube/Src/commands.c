#include "memory.h"
#include "gpio.h"
#include <config.h>
#include <commands.h>
#include <statemachine.h>
#include <ad5272.h>
#include <i2c.h>
extern uint8_t AD5272Addr;

#ifdef DEBUG
#include <SEGGER_RTT.h>
#endif

uint8_t bracket = TOLERANCE;

ButtonCommand buttons[ButtonCount] = {
    {// ButtonMode
     input : 0xA0A0,
     output : 0xB0B0
    },
    {// ButtonNextTrack
     input : 0xA1A1,
     output : 0xB1B1
    },
    {// ButtonPrevTrack
     input : 0xA2A2,
     output : 0xB2B2
    },
    {// ButtonVolUp
     input : 0xA3A3,
     output : 0xB3B3
    },
    {// ButtonVolDown
     input : 0xA4A4,
     output : 0xB4B4
    },
    {// ButtonVoiceCmd
     input : 0xA5A5,
     output : 0xB5B5
    },
    {// ButtonPhoneUp
     input : 0xA6A6,
     output : 0xB6B6
    },
    {// ButtonPhoneDown
     input : 0xA7A7,
     output : 0xB7B7
    },
    {// ButtonIdle (always has to be last)
     input : 0xA8A8,
     output : 0xB8B8
    }};

void ProcessInput(uint32_t inVal)
{
    for (uint32_t i = 0; i < ButtonCount; i++)
    {
        if (((buttons[i].input - bracket) < inVal) && ((buttons[i].input + bracket) > inVal))
        {
            ProcessEvent(i);
        }
    }
}

void ButtonIdleCallback()
{
#ifdef DEBUG
    SEGGER_RTT_printf(0, "Button UP\r\n");
#endif
    HAL_GPIO_WritePin(RING_SW_GPIO_Port, RING_SW_Pin, GPIO_PIN_RESET);
    AD5272_command_write(&hi2c1, AD5272_RDAC_WRITE, buttons[ButtonIdle].output & ~OUTPUT_ALT_FLAG);
}

void ButtonCallback(uint8_t btn)
{
#ifdef DEBUG
    SEGGER_RTT_printf(0, "%s DOWN\r\n", buttons[btn].name);
#endif
    if (buttons[btn].output & OUTPUT_ALT_FLAG)
    {
        HAL_GPIO_WritePin(RING_SW_GPIO_Port, RING_SW_Pin, GPIO_PIN_SET);
    }
    AD5272_command_write(&hi2c1, AD5272_RDAC_WRITE, buttons[btn].output & ~OUTPUT_ALT_FLAG);
}

uint32_t currentState = ButtonIdle;

void ProcessEvent(uint32_t ev)
{
    switch (currentState)
    {
    case ButtonIdle:
        switch (ev)
        {
        case ButtonIdle:
            break;
        default:
            currentState = ev;
            ButtonCallback(ev);
            break;
        }
        break;
    default:
        switch (ev)
        {
        case ButtonIdle:
            ButtonIdleCallback();
            currentState = ButtonIdle;
            break;
        default:
            break;
        }
        break;
    }
}

#ifdef DEBUG
void InitCommandNames()
{
    memcpy(buttons[ButtonMode].name, "MODE", 5);
    memcpy(buttons[ButtonNextTrack].name, "NEXT_TRK", 9);
    memcpy(buttons[ButtonPrevTrack].name, "PERV_TRK", 9);
    memcpy(buttons[ButtonVolUp].name, "VOL_UP", 7);
    memcpy(buttons[ButtonVolDown].name, "VOL_DWN", 8);
    memcpy(buttons[ButtonVoiceCmd].name, "VOICE_CMD", 10);
    memcpy(buttons[ButtonPhoneUp].name, "PHONE_DWN", 10);
    memcpy(buttons[ButtonPhoneDown].name, "PHONE_UP", 9);
}
#endif
