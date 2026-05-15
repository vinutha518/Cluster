/*
 * serial_to_parallel.h
 *
 *  Created on: 22-Jan-2019
 *      Author: MSWELT19
 */

#ifndef SERIAL_TO_PARALLEL_H_
#define SERIAL_TO_PARALLEL_H_
#define LED_DATA_OUT_HIGH		1
#define LED_DATA_OUT_LOW		0
#define LED_CLOCK_HIGH			1
#define LED_CLOCK_LOW			0
#define ENB_DATA_TX		        1
#define CLR_ALL_OP_BUFFER   	0
#define ALL_OP_DISABLE     		1
#define ALL_OP_ENABLE  			0
#define LATCH_ENABLE_HIGH		1
#define LATCH_ENABLE_LOW		0

void All_Telltale_OFF(void);
void All_Telltale_ON(void);
void output_shift_register(void);
void output_shift_register(void);
void Check_TT_ETM_Mode(void);
#ifdef PCBrevB
#else
void GI_Control(void);

void GI_off_SW_PRESS(void);
#endif

typedef union{
	struct {
		uint8_t bit0 : 1;
		uint8_t bit1 : 1;
		uint8_t bit2 : 1;
		uint8_t bit3 : 1;
		uint8_t bit4 : 1;
		uint8_t bit5 : 1;
		uint8_t bit6 : 1;
        uint8_t bit7 : 1;
		uint8_t bit8 : 1;
		uint8_t bit9 : 1;
		uint8_t bit10 : 1;
		uint8_t bit11 : 1;
		uint8_t bit12 : 1;
		uint8_t bit13 : 1;
		uint8_t bit14 : 1;
		uint8_t bit15 : 1;
	}u;

	uint32_t status;
}Led_status;

extern Led_status ls;
extern Led_status ls_Dummy;
extern _Bool sw_press_ok;
extern _Bool Digital_Speedo_Guage_off;

#define Led_Shift 			ls_Dummy.status

#define Led_status_word 			ls.status
#define AEBS_OFF_TT    				ls.u.bit0
#define SEAT_BELT_TT    			ls.u.bit1
#define LOW_FUEL_TT   				ls.u.bit2
#define BSG_TT				   		ls.u.bit3
#define FRONT_FOG_TT  	 			ls.u.bit4
#define REAR_FOG_TT   	 			ls.u.bit5
#define PARK_LAMP_TT   	 			ls.u.bit6
#define OVER_SPEED_TT 				ls.u.bit7
#define FOUR_WD_LOW_TT    			ls.u.bit8
#define FOUR_WD_HIGH_TT  		  	ls.u.bit9
#define Water_in_fuel_TT		   	ls.u.bit10
#define TEMP_TT						ls.u.bit11
#define Glow_Plug_TT			   	ls.u.bit12
#define REGEN_TT	    			   	ls.u.bit13
#define DOOR_OPEN_TT					ls.u.bit14
#define ECO_TT			    	ls.u.bit15


#endif /* SERIAL_TO_PARALLEL_H_ */
