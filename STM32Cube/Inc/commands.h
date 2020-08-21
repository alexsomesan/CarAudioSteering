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
    Button1, Button2, Button3, Button4, Button5, Button6, Button7, Button8,
    ButtonIdle, ButtonCount // these two must always come last, in this exact order
};

typedef struct _btncmd {
    uint16_t input;
    uint16_t output;
    void (*callback)(void);
    // char name[10];
} ButtonCommand;

extern ButtonCommand buttons[ButtonCount];

void InitCommands();
void ProcessInput(uint32_t inVal);

void ButtonIdleCallback();

void Button1Callback();
void Button2Callback();
void Button3Callback();
void Button4Callback();
void Button5Callback();
void Button6Callback();
void Button7Callback();
void Button8Callback();

#ifdef __cplusplus
}
#endif

#endif
