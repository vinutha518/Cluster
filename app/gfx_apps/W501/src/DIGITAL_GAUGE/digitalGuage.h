/* **********************************************************************************************************************
File                    :       digitalGuage.h
Author                  :       Viresh Wadle
Model                   :       Mahindra W501
Revision Author         :
Reviewed By             :
Company                 :       Minda Stoneridge Instrumentation Limited ,Chakan,Pune

*********************************************************************************************************************** */
#include "r_typedefs.h"

#ifndef DIGITALGUAGE_H_
#define DIGITALGUAGE_H_

typedef union
{
	uint16_t w;
	uint8_t b[2];
}SplitWord;
extern SplitWord down_scale;

extern uint16_t final_vechile_speed;
extern uint16_t final_vechile_speed_gauge_raw;
extern uint32_t display_speedometer_speed;
extern uint32_t final_tripmeter_distance;
extern uint32_t after_dec_val;//
extern uint32_t final_ACtripmeter_distance;
extern uint32_t final_ACtripmeter_distance1;
extern uint8_t AC_Trip_1min_ON_flag;
extern uint32_t Counter_1min;
extern uint32_t final_odomter_distance;
extern uint32_t old_odometer_distance;
extern uint32_t display_ACtripmeter_speed;
extern int32_t TRIP_AC_OFFSET;
extern uint8_t sts_ac_compressor_flag;
extern _Bool AC_ON;
extern _Bool speed;
extern uint32_t Send_odometer_data;
extern _Bool vehicle_moving_state;
extern _Bool Speed_Invalid_Default;
extern _Bool Dist_Invalid_Default;
extern _Bool Trip_AC_Rollover;
/*km/mile selection*/
extern uint8_t Dist_Unit;

/*odo Error*/
extern uint8_t ODO_ERROR_DTC;
extern uint8_t odo_error_flag;
extern uint8_t odometer_reset_flag;
extern uint8_t odometer_reset_flag_1;

extern uint32_t final_tripmeter_B_distance;

extern int8_t Engine_Speed ;
extern uint8_t temp_widget;
extern uint8_t blink_temp;
extern uint8_t new_temp_value;
extern Split32 AVS_sec_count_A;
extern Split32 AVS_sec_count_B;
extern Split32 DT_sec_count_A;
extern Split32 DT_sec_count_B;
extern uint8_t drive_count_start;
extern uint8_t drivesum_count_start;
extern uint16_t Display_Min_A;
extern uint16_t Display_Hr_A;
extern uint16_t Display_Min_B;
extern uint16_t Display_Hr_B;

extern uint32_t Final_Drivesum_odo_distance;
extern uint32_t Final_Drivesum_DT_sec_count;
extern uint16_t DSum_Display_Hr;
extern uint16_t DSum_Display_Min;
extern uint32_t Dsum_Avg_vehicle_speed;
extern uint32_t Dsum_Display_Average_speed;
extern Split32 DriveSumsec_count;
extern uint64_t Avg_vehicle_speed1;
extern uint64_t Avg_vehicle_speed2;
extern uint32_t Accumulated_distance_A;
extern uint32_t Accumulated_distance_B;
extern uint16_t Display_Average_speed_A;
extern uint16_t Display_Average_speed_B;
extern uint32_t Init_2WD_odo_distance;
extern uint32_t Prev_2WD_odo_distance;
extern uint32_t Final_Drivesum_2WD_distance;

extern void speedometer(void);
extern void odometer(void);
extern void tripmeter(void);
extern void AC_tripmeter(void);
extern void Engine_Coolant_Temperature(void);
extern void tripmeter_B(void);

extern void reset_odometer(void);
extern void reset_tripmeterA(void);
extern void reset_tripmeterB(void);
extern void reset_AC_tripmeter(void);
extern void Average_vehicle_speed(void);
void drive_time(void);
void Drive_summery(void);

#endif /* DIGITALGUAGE_H_ */
