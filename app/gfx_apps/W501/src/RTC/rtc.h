/*
****************************************************************************
PROJECT : Audio generation demo
FILE    : $Id: rtc.h 3922 2014-09-22 08:40:10Z golczewskim $
============================================================================
DESCRIPTION
Audio generation Test Application
============================================================================
                            C O P Y R I G H T
============================================================================
                           Copyright (c) 2011
                                  by
                       NEC Electronics (Europe) GmbH.
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing

Warranty Disclaimer

Because the Product(s) is licensed free of charge, there is no warranty
of any kind whatsoever and expressly disclaimed and excluded by NEC,
either expressed or implied, including but not limited to those for
non-infringement of intellectual property, merchantability and/or
fitness for the particular purpose.
NEC shall not have any obligation to maintain, service or provide bug
fixes for the supplied Product(s) and/or the Application.

Each User is solely responsible for determining the appropriateness of
using the Product(s) and assumes all risks associated with its exercise
of rights under this Agreement, including, but not limited to the risks
and costs of program errors, compliance with applicable laws, damage to
or loss of data, programs or equipment, and unavailability or
interruption of operations.

Limitation of Liability

In no event shall NEC be liable to the User for any incidental,
consequential, indirect, or punitive damage (including but not limited
to lost profits) regardless of whether such liability is based on breach
of contract, tort, strict liability, breach of warranties, failure of
essential purpose or otherwise and even if advised of the possibility of
such damages. NEC shall not be liable for any services or products
provided by third party vendors, developers or consultants identified or
referred to the User by NEC in connection with the Product(s) and/or the
Application.

****************************************************************************
*/

#ifndef __RTC_H
#define __RTC_H

#include "r_rtca_api.h"


/***********************************************************
  Section: Defines
*/


/***********************************************************
  Define: SATURDAY - SUNDAY

  Days of the week value for the alarm setting
*/

#define SATURDAY    0x01
#define MONDAY      0x02
#define TUSEDAY     0x04
#define WEDNESDAY   0x08
#define THURSDAY    0x10
#define FRIDAY      0x20
#define SUNDAY      0x40


/***********************************************************
  Section: Enumerators

  RTC time and alarm options
*/

typedef enum {
 TIME_DAY,
 TIME_MONTH,
 TIME_YEAR,
 TIME_HOUR,
 TIME_MINUTE,
 TIME_SECOND,
 TIME_STATUS,
 ALARM_DAY,
 ALARM_HOUR,
 ALARM_MINUTE,
 ALARM_ON_OFF,
 ALARM_STATUS,
}ee_Time_t;


/***********************************************************
  Section: Types

  Complete structure to store to the backup RAM
*/

typedef struct{
    r_rtca_Data_t RTCData;
    r_rtca_Alarm_t RTCAlarm;
    r_rtca_IntConf_t RTCIntConfig;
    r_rtca_Parameter_t RTCConfig;
    uint8_t bAlarmOn;
    uint8_t bRTCClockAccelerateEnable;
}eeRtc_Config_t;

typedef union{
    eeRtc_Config_t RTCConfig;
    uint32_t Data[100];
}eeRtc_Backup;



extern uint8_t disp_sec,disp_min,disp_hour,disp_day,disp_month,disp_year,time_format,rtc_pm;
extern uint8_t changed_hr,changed_min;
extern _Bool format_changed;
/***********************************************************
  Section: Functions
*/


/***********************************************************
  Function: r_rtca_Init

  Application initialisation for the RTC, limited init for wakeup conditions

  Parameters:
  Wakeup       - TRUE if the device is comming out of standby
                  FALSE from a reset

  Returns:
  none
*/
void eeRtc_Init(uint8_t Wakeup);


/***********************************************************
  Function: eeRtc_SetUpdateCallback

  Set a user function as a handler for the RTC

  Parameters:
  *UpdateCallback - a pointer to a user function (callback)
                    with two parameters (unit and *Data)
                    see <r_rtca_Data_t>
  Returns:
  none
*/
void eeRtc_SetUpdateCallback(void ( *UpdateCallback )(int Unit, r_rtca_Data_t *Data));


/***********************************************************
  Function: eeRtc_SetEditTimeCallback

  Set a user function as a handler for the RTC edit data

  Parameters:
  *UpdateCallback - a pointer to a user function (callback)
                    with two parameters (unit and *Data)
                    see <r_rtca_Data_t>
  Returns:
  none
*/
void eeRtc_SetEditTimeCallback(void ( *UpdateCallback )(int Unit, r_rtca_Data_t *Data));


/***********************************************************
  Function: eeRtc_SetAlarmCallback

  Set a user function as a handler for the RTC Alarm

  Parameters:
  *UpdateCallback - a pointer to a user function (callback) with three
                    parameters (unit and *Data) see <r_rtca_Data_t>
                    uint8_t bEnabled alarm state on (TRUE) or off (FALSE)
  Returns:
  none
*/
void eeRtc_SetAlarmCallback(void ( *UpdateCallback )(int Unit, r_rtca_Alarm_t *Data, uint8_t bEnabled));


/***********************************************************
  Function: eeRtc_SetIntCallback

  Set a user function as a handler for the RTC Interval

  Parameters:
  *UpdateCallback - a pointer to a user function (callback)
                    with one parameters (unit)
  Returns:
  none
*/
void eeRtc_SetIntCallback(void ( *UpdateCallback )(int Unit));


/***********************************************************
  Function: eeRtc_Processing

  Application processing function provides information for the
  RTC

  Parameters:
  none

  Returns:
  none
*/
void eeRtc_Processing(void);


/***********************************************************
  Function: eeRtc_DisableAlarm

  Disables the alarm

  Parameters:
  none

  Returns:
  none
*/
void eeRtc_DisableAlarm(void);


/***********************************************************
  Function: eeRtc_EnableAlarm

  Enables the alarm

  Parameters:
  none

  Returns:
  none
*/
void eeRtc_EnableAlarm(void);


/***********************************************************
  Function: eeRtc_AlarmUpdate

  Updates the new alarm setting

  Parameters:
  none

  Returns:
  none
*/
void eeRtc_AlarmUpdate(void);


/***********************************************************
  Function: eeRtc_TimeUpdate

  Updates the new time setting

  Parameters:
  none

  Returns:
  none
*/
void eeRtc_TimeUpdate(uint8_t Value);


/***********************************************************
  Function: eeRtc_ClockAccel

  Sets the clock for the RTC to use a high speed one

  Parameters:
  none

  Returns:
  none
*/
void eeRtc_ClockAccel(uint8_t Value);


/***********************************************************
  Function: eeRtc_WriteBURAM

  Writes RTC configuration data to the BURAM

  Parameters:
  none

  Returns:
  none
*/
void eeRtc_WriteBURAM(void);


/***********************************************************
  Function: eeRtc_ReadBURAM

  Reads RTC configuration data to the BURAM

  Parameters:
  none

  Returns:
  none
*/
void eeRtc_ReadBURAM(void);

uint8_t loc_GetDecimal_rtc(uint8_t BcdCode_eeprom, r_rtca_HourFormat_t Hour);

/***********************************************************
  Function: eeRtc_Time...

  Simple wrapper functions for the state manager to update
  the time information via local functions

  Parameters:
  none

  Returns:
  none
*/
void eeRtc_TimeMonthInc(uint8_t Value);
void eeRtc_TimeYearInc(uint8_t Value);
void eeRtc_TimeHourInc(uint8_t Value);
void eeRtc_TimeMinInc(uint8_t Value);
void eeRtc_TimeSecInc(uint8_t Value);
void eeRtc_TimeAlarmDayInc(uint8_t Value);
void eeRtc_TimeDayInc(uint8_t Value);
void eeRtc_TimeAlarmHourInc(uint8_t Value);
void eeRtc_TimeAlarmMinInc (uint8_t Value);
void eeRtc_TimeDayDec  (uint8_t Value);
void eeRtc_TimeMonthDec(uint8_t Value);
void eeRtc_TimeYearDec (uint8_t Value);
void eeRtc_TimeHourDec (uint8_t Value);
void eeRtc_TimeMinDec  (uint8_t Value);
void eeRtc_TimeSecDec  (uint8_t Value);
void eeRtc_TimeAlarmDayDec (uint8_t Value);
void eeRtc_TimeAlarmHourDec(uint8_t Value);
void eeRtc_TimeAlarmMinDec (uint8_t Value);
void eeRtc_TimeAlarmOn(uint8_t Value);
void eeRtc_TimeAlarmOff(uint8_t Value);
uint8_t BCDToDecimal (uint8_t bcdByte);
uint8_t DecimalToBcd(uint8_t decimal);

#endif /* __RTC_H */

