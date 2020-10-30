#ifndef __COMMANDS_H
#define __COMMANDS_H

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include <stdlib.h>

#define OUTPUT_ALT_FLAG 0x8000

enum _buttons {
    ButtonMode, ButtonNextTrack, ButtonPrevTrack, ButtonVolUp, ButtonVolDown, ButtonVoiceCmd, ButtonPhoneUp, ButtonPhoneDown,
    ButtonIdle, ButtonCount // these two must always come last, in this exact order
};

typedef struct _btncmd {
    uint16_t input;
    uint16_t output;
#ifdef DEBUG    
    char name[10];
#endif
} ButtonCommand;

extern ButtonCommand buttons[ButtonCount];

void ProcessInput(uint32_t inVal);
void ButtonIdleCallback();
void ButtonCallback(uint8_t btn);

#ifdef DEBUG
void InitCommands();
#endif

#ifdef __cplusplus
}
#endif

#endif
