#include <stdint.h>
#include <stdlib.h>
#include "commands.h"
#include "SEGGER_RTT.h"

uint32_t currentState = ButtonIdle;

void ProcessEvent(uint32_t ev) {
    switch (currentState)
    {
    case ButtonIdle:
        switch (ev)
        {
        case ButtonIdle:
            break;
        default:
            currentState = ev;
            buttons[ev].callback();
            break;
        }
        break;
    default:
        switch (ev)
        {
        case ButtonIdle:
            buttons[ButtonIdle].callback();
            currentState = ButtonIdle;
            break;
        default:
            break;
        }
        break;
    }
}
