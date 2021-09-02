#include <flashstorage.h>
#ifdef FEAT_FLASH_STORAGE
#include "memory.h"
#include "adc.h"
#include "usart.h"

#ifdef DEBUG
#include "stdio.h"
#include "SEGGER_RTT.h"
#endif

void storeButtons()
{
    uint32_t err;

    HAL_ADC_Stop_DMA(&hadc);
    HAL_UART_DMAStop(&huart1);
    __disable_irq();

    err = HAL_FLASH_Unlock();
    if (err != HAL_OK)
    {
        return;
    }

#ifdef FEAT_FLASH_STORAGE_ERASE
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
#endif

    for (uint8_t i = ButtonCount - 1; i >= 0; i++)
    {
        uint32_t writeval = (buttons[i].input) << 16 | buttons[i].output;
        size_t flashTarget = FLASH_BANK1_END - (i + 1) * sizeof(uint32_t);
        int err = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flashTarget, writeval);
        if (err != HAL_OK)
        {
#ifdef DEBUG
            SEGGER_RTT_TerminalOut(2, "Failed to save to flash");
#endif
        }
    }
    HAL_FLASH_Lock();

    __enable_irq();
    HAL_ADC_Start_DMA(&hadc, (uint32_t *)readouts, BUT_BUF_LEN);
    HAL_UART_Receive_DMA(&huart1, rxBuff, RX_BUF_LEN);
}

void loadButtons()
{
    size_t flashTarget = FLASH_BANK1_END - (ButtonCount * sizeof(uint32_t));
    for (int i = 0; i < ButtonCount; i++)
    {
        uint32_t flashval = *((uint32_t *)flashTarget + i);
        buttons[i].input = (flashval >> 16) & 0xFFFF;
        buttons[i].output = flashval & 0xFFFF;
    }
}

#endif