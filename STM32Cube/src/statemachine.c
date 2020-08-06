#include <stdint.h>
#include <stdlib.h>
#include "btinput.h"
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

void ProcEvent(uint32_t ev) {
    switch (currentState)
    {
    case STATE_IDLE:
        switch (ev)
        {
        case ButtonIdle:
            break;
        case Button1:
            currentState = STATE_BT1_DOWN;
            SEGGER_RTT_printf(0, "Button %d DOWN\n", Button1);
            break;
        case Button2:
            currentState = STATE_BT2_DOWN;
            SEGGER_RTT_printf(0, "Button %d DOWN\n", Button2);
            break;
        case Button3:
            currentState = STATE_BT3_DOWN;
            SEGGER_RTT_printf(0, "Button %d DOWN\n", Button3);
            break;
        case Button4:
            currentState = STATE_BT4_DOWN;
            SEGGER_RTT_printf(0, "Button %d DOWN\n", Button4);
            break;
        case Button5:
            currentState = STATE_BT5_DOWN;
            SEGGER_RTT_printf(0, "Button %d DOWN\n", Button5);
            break;
        case Button6:
            currentState = STATE_BT6_DOWN;
            SEGGER_RTT_printf(0, "Button %d DOWN\n", Button6);
            break;
        case Button7:
            currentState = STATE_BT7_DOWN;
            SEGGER_RTT_printf(0, "Button %d DOWN\n", Button7);
            break;
        case Button8:
            currentState = STATE_BT8_DOWN;
            SEGGER_RTT_printf(0, "Button %d DOWN\n", Button8);
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
            SEGGER_RTT_printf(0, "Button %d UP\n", currentState);
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
