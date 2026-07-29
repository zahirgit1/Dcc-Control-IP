#ifndef SEGMET_WRITE_H			/* prevent circular inclusions */
#define SEGMET_WRITE_H		/* by using protection macros */
#ifdef __cplusplus
extern "C" {
#endif
#include "xgpio.h"
#include "xparameters.h"
#include "IP_Centrale_DCC.h"

void write_digit(XGpio *segment, u32 number); // function to display a number
void select_digit(XGpio *segment, u32 digit_index);
void write_number(XGpio *segment, u32 total_number,u32 del);
void write_letter_num(XGpio *segment, u32 letter_idx, u32 number, u32 delay_timer);// function to display the anode
#ifdef __cplusplus
}
#endif

#endif
