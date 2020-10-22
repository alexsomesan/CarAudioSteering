#ifndef __STATEMACHINE_H
#define __STATEMACHINE_H

#ifdef __cplusplus
extern "C"
{
    #endif

    #include <stdint.h>
    #include <stdlib.h>

    void ProcessEvent(uint32_t ev);

    #ifdef __cplusplus
}
#endif

#endif 