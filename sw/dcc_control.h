#ifndef DCC_CONTROL_H			/* prevent circular inclusions */
#define DCC_CONTROL_H	/* by using protection macros */
#ifdef __cplusplus
extern "C" {
#endif

#include "xgpio.h"
#include "xparameters.h"
#include "IP_Centrale_DCC.h"



void Function5_12(UINTPTR BaseAddress, u8 address, u8 speed, u8 group);
void Function0_4(UINTPTR BaseAddress, u8 address, u8 function);
void set_speed_dcc(UINTPTR BaseAddress, u8 address, u8 speed, u8 forward);
void Function13_20(UINTPTR BaseAddress, u8 address, u8 function);

#ifdef __cplusplus
}
#endif

#endif
