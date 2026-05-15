/***********************************************************************************************************************
File            :       VIN_MISMATCH.h
Author          :         
Model           :       
Revision Author :       
Reviewed By     :       
Company         :       Minda Stoneridge Instrumentation Limited ,Chakan,Pune  
/***********************************************************************************************************************/
#ifndef VIN_MISMATCH_H
#define VIN_MISMATCH_H

#include "r_typedefs.h"
//#include "r_cg_macrodriver.h"

extern uint8_t Data0[8];
extern uint8_t Vin_counter_ulg;
extern _Bool SYS_Fun_Flag;
extern _Bool Limp_home_mode;

extern uint8_t VIN_Mismatch(uint8_t VIN_Data_Received[]);
extern void VIN_flg_check(void);
extern void Vin_Clear_Variables(void);

#endif
