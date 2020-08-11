#include "memory.h"
#include "gpio.h"
#include <config.h>
#include <commands.h>
#include <statemachine.h>
#include <SEGGER_RTT.h>

uint8_t bracket = TOLERANCE;

ButtonCommand buttons[ButtonCount];

void InitCommands() {
    buttons[ButtonIdle].input = 4096;
    buttons[Button1].input = 3372;
    buttons[Button2].input = 3158;
    buttons[Button3].input = 2915;
    buttons[Button4].input = 2578;
    buttons[Button5].input = 2194;
    buttons[Button6].input = 1708;
    buttons[Button7].input = 1215;
    buttons[Button8].input = 610;

    buttons[ButtonIdle].output = 1023;
    buttons[Button1].output = 13;    /* SWITCH_SOURCE */
    buttons[Button2].output = 35;    /* MUTE_ATT */
    buttons[Button3].output = 70;    /* NEXT_TRACK */
    buttons[Button4].output = 105;   /* PREV_TRACK */
    buttons[Button5].output = 160;   /* VOLUME_UP */
    buttons[Button6].output = 240;   /* VOLUME_DOWN */
    buttons[Button7].output = 450;   /* RADIO_BAND */
    buttons[Button8].output = 1024;

    memcpy(buttons[Button1].name, "SWSRC", 5);
    memcpy(buttons[Button2].name, "MUTEATT", 7);
    memcpy(buttons[Button3].name, "NXTRACK", 7);
    memcpy(buttons[Button4].name, "PVTRACK", 7);
    memcpy(buttons[Button5].name, "VOLUP", 5);
    memcpy(buttons[Button6].name, "VOLDOWN", 7);
    memcpy(buttons[Button7].name, "RDIOBND", 7);
    memcpy(buttons[Button8].name, "NO-OP", 5);

    buttons[Button1].callback = &Button1Callback;
    buttons[Button2].callback = &Button2Callback;
    buttons[Button3].callback = &Button3Callback;
    buttons[Button4].callback = &Button4Callback;
    buttons[Button5].callback = &Button5Callback;
    buttons[Button6].callback = &Button6Callback;
    buttons[Button7].callback = &Button7Callback;
    buttons[Button8].callback = &Button8Callback;

    buttons[ButtonIdle].callback = &ButtonIdleCallback;
}

void ProcessInput(uint32_t inVal) {
    for (uint32_t i = 0; i < ButtonCount; i++) {
        if (((buttons[i].input - bracket) < inVal) && ((buttons[i].input + bracket) > inVal)) {
            ProcessEvent(i);
        }
    }
}

void Button1Callback() {
    SEGGER_RTT_printf(0, "%s DOWN\r\n", buttons[Button1].name);
    HAL_GPIO_WritePin(RING_SW_GPIO_Port, RING_SW_Pin, GPIO_PIN_SET);
}

void Button2Callback() {
    SEGGER_RTT_printf(0, "%s DOWN\r\n", buttons[Button2].name);
    HAL_GPIO_WritePin(RING_SW_GPIO_Port, RING_SW_Pin, GPIO_PIN_SET);
}

void Button3Callback() {
    SEGGER_RTT_printf(0, "%s DOWN\r\n", buttons[Button3].name);
    HAL_GPIO_WritePin(RING_SW_GPIO_Port, RING_SW_Pin, GPIO_PIN_SET);
}

void Button4Callback() {
    SEGGER_RTT_printf(0, "%s DOWN\r\n", buttons[Button4].name);
    HAL_GPIO_WritePin(RING_SW_GPIO_Port, RING_SW_Pin, GPIO_PIN_SET);
}

void Button5Callback() {
    SEGGER_RTT_printf(0, "%s DOWN\r\n", buttons[Button5].name);
    HAL_GPIO_WritePin(RING_SW_GPIO_Port, RING_SW_Pin, GPIO_PIN_SET);
}

void Button6Callback() {
    SEGGER_RTT_printf(0, "%s DOWN\r\n", buttons[Button6].name);
    HAL_GPIO_WritePin(RING_SW_GPIO_Port, RING_SW_Pin, GPIO_PIN_SET);
}

void Button7Callback() {
    SEGGER_RTT_printf(0, "%s DOWN\r\n", buttons[Button7].name);
    HAL_GPIO_WritePin(RING_SW_GPIO_Port, RING_SW_Pin, GPIO_PIN_SET);
}

void Button8Callback() {
    SEGGER_RTT_printf(0, "%s DOWN\r\n", buttons[Button8].name);
    HAL_GPIO_WritePin(RING_SW_GPIO_Port, RING_SW_Pin, GPIO_PIN_SET);
}

void ButtonIdleCallback() {
    SEGGER_RTT_printf(0, "Button UP\r\n");
    HAL_GPIO_WritePin(RING_SW_GPIO_Port, RING_SW_Pin, GPIO_PIN_RESET);
}
