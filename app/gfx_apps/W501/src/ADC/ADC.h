/*
 * ADC.h
 *
 *  Created on: 24-Jan-2019
 *      Author: MSWELT23
 */

#include "r_typedefs.h"        /* Renesas basic types, e.g. uint32_t */

#ifndef APP_GFX_APPS_W501_SRC_ADC_H_
#define APP_GFX_APPS_W501_SRC_ADC_H_

extern void ADC_Start_Conversion(void);
extern void ADC_Start(void);
extern void ADC_Stop(void);
extern void ADC_Wait_Conversion(void);
extern void ADC_Read(void);

extern uint16_t Fuel_ADC_value;
extern uint16_t Batt_ADC_value;
extern uint16_t Fuel_EXVrefADC_value;
extern uint8_t ADC_Counter_u1g;
extern uint8_t AD_Done_u1g; /* This variable is need to be become 1 in "my_adc_isr()" */

#endif /* APP_GFX_APPS_W501_SRC_ADC_H_ */
