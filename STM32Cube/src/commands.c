#include <commands.h>
#include <statemachine.h>
#include <SEGGER_RTT.h>

uint8_t bracket = 10;
ButtonCommand buttons[NUM_BUTTONS + 1];

void InitCommands() {
    buttons[ButtonIdle].input = 4099;
    buttons[Button1].input = 3372;
    buttons[Button2].input = 3154;
    buttons[Button3].input = 2915;
    buttons[Button4].input = 2573;
    buttons[Button5].input = 2188;
    buttons[Button6].input = 1701;
    buttons[Button7].input = 1210;
    buttons[Button8].input = 602;

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
    for (uint32_t i = 0; i < NUM_BUTTONS+1; i++) {
        if (((buttons[i].input - bracket) < inVal) && ((buttons[i].input + bracket) > inVal)) {
            ProcessEvent(i);
        }
    }
}

void Button1Callback() {
    SEGGER_RTT_printf(0, "Button %d DOWN\n", Button1);
}

void Button2Callback() {
    SEGGER_RTT_printf(0, "Button %d DOWN\n", Button2);
}

void Button3Callback() {
    SEGGER_RTT_printf(0, "Button %d DOWN\n", Button3);
}

void Button4Callback() {
    SEGGER_RTT_printf(0, "Button %d DOWN\n", Button4);
}

void Button5Callback() {
    SEGGER_RTT_printf(0, "Button %d DOWN\n", Button5);
}

void Button6Callback() {
    SEGGER_RTT_printf(0, "Button %d DOWN\n", Button6);
}

void Button7Callback() {
    SEGGER_RTT_printf(0, "Button %d DOWN\n", Button7);
}

void Button8Callback() {
    SEGGER_RTT_printf(0, "Button %d DOWN\n", Button8);
}

void ButtonIdleCallback() {
    SEGGER_RTT_printf(0, "Button UP\n");
}
