/***********************************************************************************************
PROJECT : simple_draw
FILE   : $Id: main.h 11006 2016-11-07 16:32:44Z michael.golczewski $
============================================================================ 
DESCRIPTION
header for simple_draw
============================================================================
                            C O P Y R I G H T
============================================================================
                       Copyright (c) 2013 - 2014
                                  by 
                       Renesas Electronics (Europe) GmbH.
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing, not for mass production

DISCLAIMER

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

SAMPLE CODE is not part of the licensed software, as such it must not be used in
mass-production applications. It can only be used for evaluation and
demonstration purposes at customer's premises listed in the signed license
agreement.
****************************************************************************
*/
#ifndef MAIN_H_
#define MAIN_H_


#ifdef __cplusplus
extern "C" {
#endif



#define LOC_JCUA_UNIT (0u)
#define LOC_SFMA_UNIT (0u)
#define LOC_VDCE_UNIT (0u)
/*******************************************************************************
  Section: Local Functions
*/

void loc_Error(int32_t Err);

void loc_StartSFMA(uint32_t Adr);

void loc_StopSFMA(void);



#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */
