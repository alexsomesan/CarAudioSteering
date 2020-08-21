#ifndef __FLASHSTORAGE_H
#define __FLASHSTORAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx_hal.h"
#include "commands.h"

ButtonCommand* loadButtons();
void storeButtons(ButtonCommand* b);

#ifdef __cplusplus
extern "C" {
#endif

#endif