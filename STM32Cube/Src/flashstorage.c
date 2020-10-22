#include <flashstorage.h>


void storeButtons(ButtonCommand* b) {
    HAL_FLASH_Unlock();
    // HAL_FLASH_Program();
    HAL_FLASH_Lock();
}

ButtonCommand* loadButtons() {
    return NULL;
}

