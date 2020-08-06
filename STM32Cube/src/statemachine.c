#include <stdint.h>
#include <stdlib.h>
#include "commands.h"
#include "SEGGER_RTT.h"

enum _states {
    STATE_IDLE,
    STATE_BT1_DOWN,
    STATE_BT2_DOWN,
    STATE_BT3_DOWN,
    STATE_BT4_DOWN,
    STATE_BT5_DOWN,
    STATE_BT6_DOWN,
    STATE_BT7_DOWN,
    STATE_BT8_DOWN,
    STATE_BT1_UP,
    STATE_BT2_UP,
    STATE_BT3_UP,
    STATE_BT4_UP,
    STATE_BT5_UP,
    STATE_BT6_UP,
    STATE_BT7_UP,
    STATE_BT8_UP
};

uint32_t currentState = STATE_IDLE;

void ProcessEvent(uint32_t ev) {
    switch (currentState)
    {
    case STATE_IDLE:
        switch (ev)
        {
        case ButtonIdle:
            break;
        case Button1:
            currentState = STATE_BT1_DOWN;
            buttons[Button1].callback();
            break;
        case Button2:
            currentState = STATE_BT2_DOWN;
            buttons[Button2].callback();
            break;
        case Button3:
            currentState = STATE_BT3_DOWN;
            buttons[Button3].callback();
            break;
        case Button4:
            currentState = STATE_BT4_DOWN;
            buttons[Button4].callback();
            break;
        case Button5:
            currentState = STATE_BT5_DOWN;
            buttons[Button5].callback();
            break;
        case Button6:
            currentState = STATE_BT6_DOWN;
            buttons[Button6].callback();
            break;
        case Button7:
            currentState = STATE_BT7_DOWN;
            buttons[Button7].callback();
            break;
        case Button8:
            currentState = STATE_BT8_DOWN;
            buttons[Button8].callback();
            break;
        default:
            SEGGER_RTT_printf(0, "Unsupported event %d in state %d\n", ev, currentState);
            break;
        }
        break;
    case STATE_BT1_DOWN:
    case STATE_BT2_DOWN:
    case STATE_BT3_DOWN:
    case STATE_BT4_DOWN:
    case STATE_BT5_DOWN:
    case STATE_BT6_DOWN:
    case STATE_BT7_DOWN:
    case STATE_BT8_DOWN:
        switch (ev)
        {
        case ButtonIdle:
            buttons[ButtonIdle].callback();
            currentState = STATE_IDLE;
            break;
        default:
            break;
        }
        break;
    default:
        SEGGER_RTT_printf(0, "Uknown state %d\n", currentState);
        break;
    }
}
