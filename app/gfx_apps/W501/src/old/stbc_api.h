/*
****************************************************************************
PROJECT : Stand-by Controller Api
FILE    : $Id: stbc_api.h 7262 2016-01-15 13:34:37Z michael.golczewski $
============================================================================ 
DESCRIPTION
Driver for STBC macro
============================================================================
                            C O P Y R I G H T                                    
============================================================================
                           Copyright (c) 2011
                                  by 
                       Renesas Electronics (Europe) GmbH. 
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: template for STBC API driver

Warranty Disclaimer

Because the Product(s) is licensed free of charge, there is no warranty 
of any kind whatsoever and expressly disclaimed and excluded by Renesas, 
either expressed or implied, including but not limited to those for 
non-infringement of intellectual property, merchantability and/or 
fitness for the particular purpose. 
Renesas shall not have any obligation to maintain, service or provide bug 
fixes for the supplied Product(s) and/or the Application.

Each User is solely responsible for determining the appropriateness of 
using the Product(s) and assumes all risks associated with its exercise 
of rights under this Agreement, including, but not limited to the risks 
and costs of program errors, compliance with applicable laws, damage to 
or loss of data, programs or equipment, and unavailability or 
interruption of operations.

Limitation of Liability

In no event shall Renesas be liable to the User for any incidental, 
consequential, indirect, or punitive damage (including but not limited 
to lost profits) regardless of whether such liability is based on breach 
of contract, tort, strict liability, breach of warranties, failure of 
essential purpose or otherwise and even if advised of the possibility of 
such damages. Renesas shall not be liable for any services or products 
provided by third party vendors, developers or consultants identified or
referred to the User by Renesas in connection with the Product(s) and/or the 
Application.

****************************************************************************
*/

#ifndef _STBC_API_H_
#define _STBC_API_H_

#ifdef __cplusplus
extern "C" {
#endif





/***********************************************************
  Title: Standby controller API

  An application using standby controller should include this header file.

*/


/***********************************************************
  Section: Global defines
*/


/***********************************************************
    wakeup factor

    The wake-up events for terminating a power save mode are controlled 
    and monitored by the following Stand-by Controller registers:
    WUF0, WUFMSK0, WUFC0
*/
#define R_STBC_WUL_INTDCUTDI_BIT    (1 << 0) /* OCD */
#define R_STBC_WUL_INTP0_BIT        (1 << 1) /* Port */
#define R_STBC_WUL_INTP1_BIT        (1 << 2) /* Port */
#define R_STBC_WUL_INTP2_BIT        (1 << 3) /* Port */
#define R_STBC_WUL_INTP3_BIT        (1 << 4) /* Port */
#define R_STBC_WUL_INTP4_BIT        (1 << 5) /* Port */
#define R_STBC_WUL_INTP5_BIT        (1 << 6) /* Port */
#define R_STBC_WUL_INTP6_BIT        (1 << 7) /* Port */
#define R_STBC_WUL_INTP7_BIT        (1 << 8) /* Port */
#define R_STBC_WUL_INTP8_BIT        (1 << 9) /* Port */
#define R_STBC_WUL_INTP9_BIT        (1 << 10) /* Port */
#define R_STBC_WUL_INTP10_BIT       (1 << 11) /* Port */
#define R_STBC_WUL_INTWDTA0_BIT     (1 << 12) /* WDTA0 */
#define R_STBC_WUL_INTRTCA0S__BIT   (1 << 13) /* RTCA0 second */  
#define R_STBC_WUL_INTRTCA0AL_BIT   (1 << 14) /* RTCA0 Alarm */   
#define R_STBC_WUL_INTRTCA0R_BIT    (1 << 15) /* RTCA0 interval */
#define R_STBC_WUL_INTAWOT0_BIT     (1 << 16) /* AWOT0 */
#define R_STBC_WUL_INTECM_BIT       (1 << 17) /* ECM */
#define R_STBC_WUL_NMIECM_BIT       (1 << 18) /* ECM */
                       
#define R_STBC_WUF_INTDCUTDI    (1 << 0) /* OCD */
#define R_STBC_WUF_INTP0        (1 << 1) /* Port */
#define R_STBC_WUF_INTP1        (1 << 2) /* Port */
#define R_STBC_WUF_INTP2        (1 << 3) /* Port */
#define R_STBC_WUF_INTP3        (1 << 4) /* Port */
#define R_STBC_WUF_INTP4        (1 << 5) /* Port */
#define R_STBC_WUF_INTP5        (1 << 6) /* Port */
#define R_STBC_WUF_INTP6        (1 << 7) /* Port */
#define R_STBC_WUF_INTP7        (1 << 8) /* Port */
#define R_STBC_WUF_INTP8        (1 << 9) /* Port */
#define R_STBC_WUF_INTP9        (1 << 10) /* Port */
#define R_STBC_WUF_INTP10       (1 << 11) /* Port */
#define R_STBC_WUF_INTWDTA0     (1 << 12) /* WDTA0 */
#define R_STBC_WUF_INTRTCA0S    (1 << 13) /* RTCA0 second */
#define R_STBC_WUF_INTRTCA0AL   (1 << 14) /* RTCA0 Alarm */
#define R_STBC_WUF_INTRTCA0R    (1 << 15) /* RTCA0 interval */
#define R_STBC_WUF_INTAWOT0     (1 << 16) /* AWOT0 */
#define R_STBC_WUF_INTECM       (1 << 17) /* ECM */
#define R_STBC_WUF_NMIECM       (1 << 18) /* ECM */

               
#define STBC_IO_HOLD0 (1 << 0) /* P0 */
#define STBC_IO_HOLD1 (1 << 1) /* P1 P2 */
#define STBC_IO_HOLD2 (1 << 2) /* P3 */
#define STBC_IO_HOLD3 (1 << 3) /* P43 0 - 1, P44 0 - 11, P45 0 - 13 */
#define STBC_IO_HOLD4 (1 << 4) /* P43 7 - 12 */  
#define STBC_IO_HOLD5 (1 << 5) /* P42 */  
#define STBC_IO_HOLD6 (1 << 6) /* P16 & P17 */ 


/*******************************************************************************
  Function: R_STBC_SetIoHold

  controls the IOHOLD state of the I/O buffer     

  Parameters:
  Events - each bit corresponds to a Port group
  Bit 0 P0 
  Bit 1 P1 P2 
  Bit 2 P3 
  Bit 3 P43 0 - 1, P44 0 - 11, P45 0 - 13 
  Bit 4 P43 7 - 12   
  Bit 5 P42   
  Bit 6 P16 & P17      
   
  Returns:
  == R_ERR_OK   -    Success  , see <r_Error_t>
  != R_ERR_OK   -     Fail     , see <r_Error_t>
*/
r_Error_t R_STBC_SetIoHold(uint32_t Events);


/*******************************************************************************
  Function: R_STBC_EnableWakeUpFactor

  Enables a wakeup factor (event) in deep stop mode      
     

  Parameters:
  WakeupEvents - each bit corresponds to a wakeup event  
 
  Returns:
  == R_ERR_OK   -    Success  , see <r_Error_t>
  != R_ERR_OK   -     Fail     , see <r_Error_t>
*/
r_Error_t R_STBC_EnableWakeUpFactor(uint32_t WakeupEvents);


/*******************************************************************************
  Function: R_STBC_EnableStandby

  Enables the device for standby     

  Parameters:
  void   
 
  Returns:
  == R_ERR_OK   -    Success  , see <r_Error_t>
  != R_ERR_OK  -     Fail     , see <r_Error_t>
*/
r_Error_t R_STBC_EnableStandby(void);


/*******************************************************************************
  Function: R_STBC_GetWakeUpEvent

  Provides the wakeup event     

  Parameters:
  void   
 
  Returns:
  uint32_t wakeup events see R_STBC_WUF_XXXX

*/
uint32_t R_STBC_GetWakeUpEvent(void);


#endif /* _STBC_API_H_  */
