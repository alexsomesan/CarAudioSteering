#include <flashstorage.h>
#ifdef FEAT_FLASH_STORAGE
#include "memory.h"

#ifdef DEBUG
#include "stdio.h"
#include "SEGGER_RTT.h"
#endif

void storeButtons()
{
    uint32_t err;

    err = HAL_FLASH_Unlock();
    if (err != HAL_OK)
    {
        return;
    }

    uint32_t pageBaseAddr = FLASH_BASE + FLASH_PAGE_SIZE * 15;
    FLASH_EraseInitTypeDef eraseCfg = {
        TypeErase : FLASH_TYPEERASE_PAGES,
        NbPages : 1,
        PageAddress : pageBaseAddr
    };

    uint32_t sectErr = 0;
    err = HAL_FLASHEx_Erase(&eraseCfg, &sectErr);
    if (err != HAL_OK)
    {
#ifdef DEBUG
        SEGGER_RTT_TerminalOut(2, "Failed to erase flash page");
#endif
        return;
    }

    for (uint8_t i = 0; i < ButtonCount; i++)
    {
        uint32_t writeval = (buttons[i].input) << 16 | buttons[i].output;
        size_t flashTarget = pageBaseAddr + i * sizeof(uint32_t);
        int err = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flashTarget, writeval);
        if (err != HAL_OK)
        {
#ifdef DEBUG
            SEGGER_RTT_TerminalOut(2, "Failed to save to flash");
#endif
        }
    }
    HAL_FLASH_Lock();
}

void loadButtons()
{
    for (int i = 0; i < ButtonCount; i++)
    {
        uint32_t flashval = *((uint32_t *)FLASH_LOCATION + i);
        buttons[i].input = (flashval >> 16) & 0xFFFF;
        buttons[i].output = flashval & 0xFFFF;
    }
}

#endif