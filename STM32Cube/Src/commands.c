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
     input : 256,
     output : 15
    },
    {// ButtonNextTrack
     input : 578,
     output : 80
    },
    {// ButtonPrevTrack
     input : 895,
     output : 125
    },
    {// ButtonVolUp
     input : 1271,
     output : 155
    },
    {// ButtonVolDown
     input : 1625,
     output : 230
    },
    {// ButtonVoiceCmd
     input : 2011,
     output : 0x03FF
    },
    {// ButtonPhoneUp
     input : 2329,
     output : 0x03FF
    },
    {// ButtonPhoneDown
     input : 2642,
     output : 0x03FF
    },
    {// ButtonIdle (always has to be last)
     input : 3666,
     output : 0x03FF
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
    HAL_GPIO_WritePin(TIP_SW_GPIO_Port, TIP_SW_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RING_SW_GPIO_Port, RING_SW_Pin, GPIO_PIN_RESET);
    uint16_t potVal = buttons[ButtonIdle].output & 0x3FF;
    AD5272_command_write(&hi2c1, AD5272_RDAC_WRITE, potVal);
}

void ButtonCallback(uint8_t btn)
{
#ifdef DEBUG
    SEGGER_RTT_printf(0, "%s DOWN\r\n", buttons[btn].name);
#endif
    uint16_t potVal = buttons[btn].output & 0x3FF;
    AD5272_command_write(&hi2c1, AD5272_RDAC_WRITE, potVal);
    if (buttons[btn].output & OUTPUT_ALT_FLAG)
    {
        HAL_GPIO_WritePin(RING_SW_GPIO_Port, RING_SW_Pin, GPIO_PIN_SET);
    }
    HAL_GPIO_WritePin(TIP_SW_GPIO_Port, TIP_SW_Pin, GPIO_PIN_SET);
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
