#include "r_typedefs.h"

extern void Get_Default_ADCvalue(void);
extern void Display_Fuel_Bar(void);
extern uint8_t Current_Fuel_Bar(void);
extern void ADC_Avg(void);
extern void Fuel_Init(void);
extern void IGN_mode(void);

extern uint8_t Present_level;
extern uint8_t displayed_fuel_level;
extern uint8_t New_fuel_level;
extern uint16_t New_fuel_read_ad1;
extern uint16_t New_fuel_read_avg;

extern uint8_t fuel_segment1;
extern uint8_t fuel_segment2;
extern uint8_t fuel_segment3;
extern uint8_t fuel_segment4;
extern uint8_t fuel_segment5;
extern uint8_t fuel_segment6;
extern uint8_t fuel_segment7;
extern uint8_t fuel_segment8;

extern uint8_t reference_level;
extern uint8_t ADC_Read_Flag;
extern uint8_t ADC_Read_300msec_Flag;
extern uint8_t delta_flag_1msec_Flag;

extern uint32_t Counter_30sec_Flag;
extern uint8_t Counter_10sec_Flag;//r
extern uint8_t Counter_5sec_Flag;
extern _Bool Counter_FF_5sec_Flag;
extern uint8_t Counter_5min_Flag;
extern uint8_t Counter_5min_spd_flag;//r
extern uint8_t Counter_13sec_Flag;
extern uint8_t Counter_13sec_ign_Flag;
extern uint8_t Recovery_Counter_10sec_Flag;
extern uint8_t Counter_300msec_Flag;
extern uint8_t fast_drain_start;
/*Fast fill new delay*/
extern uint8_t Per_Min_Counter_Flag;
extern uint8_t Counter_3min_Flag;
extern uint8_t u8_DTE_Fuel_En;

extern uint32_t Timer_ON_30sec;
extern uint8_t Timer_ON_10sec;//r
extern uint8_t Timer_ON_13sec;
extern uint8_t Timer_ON_13sec_ign;
extern uint8_t Timer_recovery_ON_10sec;
extern uint8_t Timer_ON_5sec;//r
extern _Bool Timer_fastfill_5sec;
extern uint8_t Timer_ON_300msec;
extern uint8_t Timer_ON_100msec;
extern uint8_t Timer_ON_5min;
extern uint8_t Timer_ON_5min_spd;//r
extern uint8_t Timer_ON_fast_drain;
/*Fast fill new delay*/
extern uint8_t Timer_ON_Per_Liter;
extern uint32_t Per_Minute_count;
extern uint8_t Timer_ON_3min;
extern _Bool Counter_330msec_Flag;


extern uint16_t ADC_Array1[10];
extern uint16_t ADC_Array2[10];

extern uint32_t Fuel_ml;//r
extern uint32_t Fuel_ml_DTE;//r
extern uint32_t FUEL_LEVEL_RAW;

extern uint32_t FLOAT_RESISTANCE_RAW;
extern uint32_t FLOAT_RESISTANCE_FILTER;
extern uint32_t FUEL_LVL_RAW;
extern uint8_t FLOAT_SENSOR_RESISTANCE_RAW;
extern uint8_t FLOAT_SENSOR_RESISTANCE_RAW_AVG;//Raw resistance level

extern uint8_t go_to_sleep;//r
extern uint8_t fuel_fault;//r
extern _Bool fuel_fault_30sec;
extern uint8_t fuel_fault_TT_blink;//r

extern uint8_t fuel_widget;
extern _Bool Fast_fill_Started;//for DTE
extern _Bool fast_drain;//for DTE
extern uint32_t FUEL_LEVEL_FOR_DTE;
extern uint32_t ADC_Update_Speed;
extern uint8_t copy_ADC;
extern _Bool Fuel_init_done;
extern uint8_t from_recovery;
extern uint8_t fuel_error;//r
