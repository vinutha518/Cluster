/*
 * odometer.c
 *
 *  Created on: Feb 17, 2019
 *      Author: MSWELT12
 */
#include "init.h"
#include "digitalGuage.h"
#include "r_typedefs.h"
#include "UDSS.h"
#include "tachometer.h"
#include "my_can.h"
#include  "r_cg_serial_user.h"
#include "odometer.h"
#include "r_cg_serial_user.h"
#include "Memmory_Map.h"
#include "UDSS_APP.h"

void eeprom_writeOdometer(int32_t new_odometer);
int32_t eeprom_readOdometer(void);

void eeprom_readEeprom(int16_t ee_addr, uint8_t len, uint8_t *ptr);
void eeprom_writeEeprom(int16_t ee_addr, uint8_t len, uint8_t *ptr);

void eeprom_write_Trip1_Odometer(int32_t new_odometer);
void eeprom_write_Trip2_Odometer(int32_t new_odometer);
void eeprom_write_Trip_AC_Odometer(int32_t new_odometer);
void eeprom_write_Trip_AC_Odometer1(int32_t new_odometer);
void eeprom_write_Trip_AC_Offset(int32_t new_odometer);

int32_t eeprom_readTrip1_Odometer(void);
int32_t eeprom_readTrip2_Odometer(void);
int32_t eeprom_readTrip_AC_Odometer(void);
int32_t eeprom_readTrip_AC_Odometer1(void);
int32_t eeprom_readTrip_AC_Offset(void);

void DID_ODO_Update(void);

_Bool invalid_ODO_input = 0;
extern uint8_t  U8_Reset_DTE;
extern _Bool Flag_AFE_reset;
extern _Bool ODO_write;
PUBLIC void eeprom_writeOdometer(int32_t new_odometer)
/**************************************************************************************
**
**  Parameter(s):       new_odometer - updated odometer value
**
**  Return value:       -
**
**  Var dependencies:
**
**  Description:        Saves the new odometer value to the EEPROM.
**                      To prevent from writing to many times to one location in the
**                      EEPROM the routine will use a ring-buffer to store the value.
**
**************************************************************************************/
{
	Split32 tmp;
	uint8_t odo_chksum;

   tmp.u32 = new_odometer;
   odo_chksum = tmp.u8[0] + tmp.u8[1] + tmp.u8[2] + tmp.u8[3];
   odo_chksum = ~odo_chksum;
   odo_chksum += 1;

   priv_odometer_index++;
   if (priv_odometer_index >= NO_OF_ODOMETER_VALUES)
   {
      priv_odometer_index = 0;
   }/* MISRA C rule 14.9 */

   eeprom_writeEeprom(PARAM_ODOMETER_BUFFER + (5 * priv_odometer_index), 4, tmp.u8);
   eeprom_writeEeprom(PARAM_ODOMETER_BUFFER + (5 * priv_odometer_index)+ 4, 1, &odo_chksum);
}


PUBLIC int32_t eeprom_readOdometer(void)
/**************************************************************************************
**
**  Parameter(s):       -
**
**  Return value:       stored odometer value
**
**  Var dependencies:   priv_odometer_index (RW)
**
**  Description:        Reads out the last stored odometer value out of the EEPROM
**                      and returns it.
**
**************************************************************************************/
{
	int32_t     max_odometer = 0,max_odometer1=0,max_odometer2=0;
	Split32 odometer;
   uint8_t     i,odo_chksum,chk_chksum;

   for (i = 0; i < NO_OF_ODOMETER_VALUES; i++)
   {
      eeprom_readEeprom(PARAM_ODOMETER_BUFFER + (5 * i), 4, odometer.u8);
      eeprom_readEeprom(PARAM_ODOMETER_BUFFER + (5 * i) + 4, 1, &odo_chksum);

      chk_chksum = odometer.u8[0] + odometer.u8[1] + odometer.u8[2] + odometer.u8[3] + odo_chksum;

      if(chk_chksum == 0)
      {
        if (odometer.u32 > max_odometer1 && odometer.u32 < (ODO_MAX+1)) /* TEA2_I-001946 -Sachin A */
        {
            max_odometer1        = odometer.u32;
            priv_odometer_index = i;
        }
      }
   }

   for (i = 0; i < NO_OF_ODOMETER_VALUES; i++)
   {
      eeprom_readEeprom(PARAM_ODOMETER_BUFFER + (5 * i), 4, odometer.u8);
      eeprom_readEeprom(PARAM_ODOMETER_BUFFER + (5 * i) + 4, 1, &odo_chksum);

      chk_chksum = odometer.u8[0] + odometer.u8[1] + odometer.u8[2] + odometer.u8[3] + odo_chksum;

      if(chk_chksum == 0)
      {
        if (odometer.u32 > max_odometer2 && odometer.u32 < (ODO_MAX+1)) /* TEA2_I-001946 -Sachin A */
        {
            max_odometer2        = odometer.u32;
            priv_odometer_index = i;
        }
      }
   }

   if(max_odometer1 != max_odometer2)
   {

	   for (i = 0; i < NO_OF_ODOMETER_VALUES; i++)
	   {
		  eeprom_readEeprom(PARAM_ODOMETER_BUFFER + (5 * i), 4, odometer.u8);
		  eeprom_readEeprom(PARAM_ODOMETER_BUFFER + (5 * i) + 4, 1, &odo_chksum);

		  chk_chksum = odometer.u8[0] + odometer.u8[1] + odometer.u8[2] + odometer.u8[3] + odo_chksum;

		  if(chk_chksum == 0)
		  {
			if (odometer.u32 > max_odometer && odometer.u32 < (ODO_MAX+1)) /* TEA2_I-001946 -Sachin A */
			{
				max_odometer        = odometer.u32;
				priv_odometer_index = i;
			}
		  }
	   }
   }
   else
   {
	   max_odometer = max_odometer1;
   }

   return max_odometer;
}


PUBLIC void eeprom_write_Trip1_Odometer(int32_t new_odometer)
/**************************************************************************************
**
**  Parameter(s):       new_odometer - updated odometer value
**
**  Return value:       -
**
**  Var dependencies:
**
**  Description:        Saves the new odometer value to the Trip location in EEPROM.
**                      
**                      
**
**************************************************************************************/
{
	Split32 tmp;
	uint8_t odo_chksum;

   tmp.u32 = new_odometer;
   odo_chksum = tmp.u8[0] + tmp.u8[1] + tmp.u8[2] + tmp.u8[3];
   odo_chksum = ~odo_chksum;
   odo_chksum += 1;

   eeprom_writeEeprom(PARAM_TRIP1_ODOMETER_BUFFER, 4, tmp.u8);
   eeprom_writeEeprom(PARAM_TRIP1_ODOMETER_BUFFER + 4, 1, &odo_chksum);
}

PUBLIC void eeprom_write_Trip2_Odometer(int32_t new_odometer)
/**************************************************************************************
**
**  Parameter(s):       new_odometer - updated odometer value
**
**  Return value:       -
**
**  Var dependencies:
**
**  Description:        Saves the new odometer value to the Trip location in EEPROM.
**                      
**                      
**
**************************************************************************************/
{
	Split32 tmp;
	uint8_t odo_chksum;

   tmp.u32 = new_odometer;
   odo_chksum = tmp.u8[0] + tmp.u8[1] + tmp.u8[2] + tmp.u8[3];
   odo_chksum = ~odo_chksum;
   odo_chksum += 1;

   eeprom_writeEeprom(PARAM_TRIP2_ODOMETER_BUFFER, 4, tmp.u8);
   eeprom_writeEeprom(PARAM_TRIP2_ODOMETER_BUFFER + 4, 1, &odo_chksum);
}

PUBLIC void eeprom_write_Trip_AC_Odometer(int32_t new_odometer)
/**************************************************************************************
**
**  Parameter(s):       new_odometer - updated odometer value
**
**  Return value:       -
**
**  Var dependencies:
**
**  Description:        Saves the new odometer value to the Trip location in EEPROM.
**
**
**
**************************************************************************************/
{
	Split32 tmp;
	uint8_t odo_chksum;

   tmp.u32 = new_odometer;
   odo_chksum = tmp.u8[0] + tmp.u8[1] + tmp.u8[2] + tmp.u8[3];
   odo_chksum = ~odo_chksum;
   odo_chksum += 1;

   eeprom_writeEeprom(PARAM_TRIP_AC_ODOMETER_BUFFER, 4, tmp.u8);
   eeprom_writeEeprom(PARAM_TRIP_AC_ODOMETER_BUFFER + 4, 1, &odo_chksum);
}

PUBLIC void eeprom_write_Trip_AC_Odometer1(int32_t new_odometer)
/**************************************************************************************
**
**  Parameter(s):       new_odometer - updated odometer value
**
**  Return value:       -
**
**  Var dependencies:
**
**  Description:        Saves the new odometer value to the Trip location in EEPROM.
**
**
**
**************************************************************************************/
{
	Split32 tmp;
	uint8_t odo_chksum;

   tmp.u32 = new_odometer;
   odo_chksum = tmp.u8[0] + tmp.u8[1] + tmp.u8[2] + tmp.u8[3];
   odo_chksum = ~odo_chksum;
   odo_chksum += 1;

   eeprom_writeEeprom(PARAM_TRIP_AC_ODOMETER_BUFFER1, 4, tmp.u8);
   eeprom_writeEeprom(PARAM_TRIP_AC_ODOMETER_BUFFER1 + 4, 1, &odo_chksum);
}



void eeprom_write_Trip_AC_Offset(int32_t new_odometer)
/**************************************************************************************
**
**  Parameter(s):       AC_offset - updated AC_offset value
**
**  Return value:       -
**
**  Var dependencies:
**
**  Description:        Saves the new AC_offset value to the offset location in EEPROM.
**
**
**
**************************************************************************************/
{
	Split32 tmp;
	uint8_t odo_chksum;

   tmp.u32 = new_odometer;
   odo_chksum = tmp.u8[0] + tmp.u8[1] + tmp.u8[2] + tmp.u8[3];
   odo_chksum = ~odo_chksum;
   odo_chksum += 1;

   eeprom_writeEeprom(PARAM_TRIP_AC_TIMER_ODOMETER_BUFFER, 4, tmp.u8);
   eeprom_writeEeprom(PARAM_TRIP_AC_TIMER_ODOMETER_BUFFER + 4, 1, &odo_chksum);
}

PUBLIC int32_t eeprom_readTrip1_Odometer(void)
/**************************************************************************************
**
**  Parameter(s):       -
**
**  Return value:       stored Trip odometer value
**
**  Var dependencies:   
**
**  Description:        Reads out the last stored Trip odometer reset value out of the EEPROM
**                      and returns it.
**
**************************************************************************************/
{

	Split32 Trip_odometer;
	uint8_t tripodo_chksum,chk_chksum;

    eeprom_readEeprom(PARAM_TRIP1_ODOMETER_BUFFER, 4, Trip_odometer.u8);
    eeprom_readEeprom(PARAM_TRIP1_ODOMETER_BUFFER + 4, 1, &tripodo_chksum);
 
    chk_chksum = Trip_odometer.u8[0] + Trip_odometer.u8[1] + Trip_odometer.u8[2] + Trip_odometer.u8[3] + tripodo_chksum;

         if(chk_chksum != 0)
         {
        	 Trip_odometer.u32=0;
         }

   return Trip_odometer.u32;
}

PUBLIC int32_t eeprom_readTrip2_Odometer(void)
/**************************************************************************************
**
**  Parameter(s):       -
**
**  Return value:       stored Trip odometer value
**
**  Var dependencies:   
**
**  Description:        Reads out the last stored Trip odometer reset value out of the EEPROM
**                      and returns it.
**
**************************************************************************************/
{

	Split32 Trip_odometer;
	uint8_t tripodo_chksum,chk_chksum;

    eeprom_readEeprom(PARAM_TRIP2_ODOMETER_BUFFER, 4, Trip_odometer.u8);
    eeprom_readEeprom(PARAM_TRIP2_ODOMETER_BUFFER + 4, 1, &tripodo_chksum);
 
    chk_chksum = Trip_odometer.u8[0] + Trip_odometer.u8[1] + Trip_odometer.u8[2] + Trip_odometer.u8[3] + tripodo_chksum;

         if(chk_chksum != 0)
         {
        	 Trip_odometer.u32=0;
         }

   return Trip_odometer.u32;
}


PUBLIC int32_t eeprom_readTrip_AC_Odometer(void)
/**************************************************************************************
**
**  Parameter(s):       -
**
**  Return value:       stored Trip odometer value
**
**  Var dependencies:
**
**  Description:        Reads out the last stored Trip odometer reset value out of the EEPROM
**                      and returns it.
**
**************************************************************************************/
{

	Split32 Trip_odometer;
	uint8_t tripodo_chksum,chk_chksum;

    eeprom_readEeprom(PARAM_TRIP_AC_ODOMETER_BUFFER, 4, Trip_odometer.u8);
    eeprom_readEeprom(PARAM_TRIP_AC_ODOMETER_BUFFER + 4, 1, &tripodo_chksum);

    chk_chksum = Trip_odometer.u8[0] + Trip_odometer.u8[1] + Trip_odometer.u8[2] + Trip_odometer.u8[3] + tripodo_chksum;

         if(chk_chksum != 0)
         {
        	 Trip_odometer.u32=0;
         }

   return Trip_odometer.u32;
}


PUBLIC int32_t eeprom_readTrip_AC_Odometer1(void)
/**************************************************************************************
**
**  Parameter(s):       -
**
**  Return value:       stored Trip odometer value
**
**  Var dependencies:
**
**  Description:        Reads out the last stored Trip odometer reset value out of the EEPROM
**                      and returns it.
**
**************************************************************************************/
{

	Split32 Trip_odometer;
	uint8_t tripodo_chksum,chk_chksum;

    eeprom_readEeprom(PARAM_TRIP_AC_ODOMETER_BUFFER1, 4, Trip_odometer.u8);
    eeprom_readEeprom(PARAM_TRIP_AC_ODOMETER_BUFFER1 + 4, 1, &tripodo_chksum);

    chk_chksum = Trip_odometer.u8[0] + Trip_odometer.u8[1] + Trip_odometer.u8[2] + Trip_odometer.u8[3] + tripodo_chksum;

         if(chk_chksum != 0)
         {
        	 Trip_odometer.u32=0;
         }

   return Trip_odometer.u32;
}


PUBLIC int32_t eeprom_readTrip_AC_Offset(void)
/**************************************************************************************
**
**  Parameter(s):       -
**
**  Return value:       stored Trip odometer value
**
**  Var dependencies:
**
**  Description:        Reads out the last stored Trip odometer reset value out of the EEPROM
**                      and returns it.
**
**************************************************************************************/
{

	Split32 Trip_odometer;
	uint8_t tripodo_chksum,chk_chksum;

    eeprom_readEeprom(PARAM_TRIP_AC_TIMER_ODOMETER_BUFFER, 4, Trip_odometer.u8);
    eeprom_readEeprom(PARAM_TRIP_AC_TIMER_ODOMETER_BUFFER + 4, 1, &tripodo_chksum);

    chk_chksum = Trip_odometer.u8[0] + Trip_odometer.u8[1] + Trip_odometer.u8[2] + Trip_odometer.u8[3] + tripodo_chksum;

         if(chk_chksum != 0)
         {
        	 Trip_odometer.u32=0;
         }

   return Trip_odometer.u32;
}
/****************************************************************************************/

PUBLIC void eeprom_readEeprom(int16_t ee_addr, uint8_t len, uint8_t *ptr)
{
	eeprom_memread_32bytes(ee_addr, ptr,len);
}

PUBLIC void eeprom_writeEeprom(int16_t ee_addr, uint8_t len, uint8_t *ptr)
{
	eeprom_memwrite_32bytes(ee_addr, ptr,len);
}


PUBLIC void DID_ODO_Update(void)
{
	uint32_t Present_ODO = 0U;
	Split32 New_ODO;
	uint8_t j=0;
	Present_ODO = eeprom_readOdometer();
	New_ODO.u32     = 0x00;		//Clear New_ODO union
	for(uint8_t i=3;i>=1;i--)
	{
//r		if(DID_ODOMETER_0xF1B0_arr[i-1] != 0)
		{
			New_ODO.u8[j] = DID_ODOMETER_0xF1B0_arr[i-1];
			j++;
		}
	}
	if(New_ODO.u32 < 4294967)	/*max odo value allow to store in U32 after multiplication by 1000*/
	{
		New_ODO.u32 = New_ODO.u32 * 1000;
		if((Present_ODO < New_ODO.u32)&&(New_ODO.u32<(ODO_MAX+1)))
		{
			final_odomter_distance = New_ODO.u32;

			eeprom_writeOdometer(final_odomter_distance);		//New odo update
			eeprom_writeOdometer(final_odomter_distance);		//New odo update
			final_odomter_distance = eeprom_readOdometer();
			final_odomter_distance = eeprom_readOdometer();
			reset_tripmeterA();
			reset_tripmeterB();
			reset_AC_tripmeter();	//Need to add to maintain Correct calculation.
			Flag_AFE_reset= 1;
			U8_Reset_DTE = 1U;
			invalid_ODO_input = 0;
			ODO_write = 1;
			Init_2WD_odo_distance = final_odomter_distance;	/*reset 4WD not engaged distance in drive summary  - 27-05-20*/
			Prev_2WD_odo_distance = 0;
		}
		else
		{
			invalid_ODO_input = 1;
		}
	}
	else
	{
		invalid_ODO_input = 1;
	}
}

