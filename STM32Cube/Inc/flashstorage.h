#ifndef __FLASHSTORAGE_H
#define __FLASHSTORAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
#include "stm32f0xx_hal.h"
#include "commands.h"

void loadButtons();
void storeButtons();

#ifdef __cplusplus
extern "C" {
#endif

#endif