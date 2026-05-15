/*
****************************************************************************
Project : Standby demo
FILE    : $Id: standby.h 3922 2014-09-22 08:40:10Z golczewskim $
============================================================================
DESCRIPTION
Standby demo Test Application
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


#ifndef __STANDBY_H
#define __STANDBY_H

/***********************************************************
  Section: Enumerators
*/

/* wakeup flags */
typedef enum {
  E_WAKEUP_NONE = 0,
  E_WAKEUP_INTERVAL,
  E_WAKEUP_ALARM,
  E_WAKEUP_EXPIN,
  E_WAKEUP_OTHER /* debugger or something else for testing */
}ee_Wakeup_t;


/***********************************************************
  Section: Functions
*/

/***********************************************************
  Function: eeStandby_Init

  Initialisation for the standby module

  Parameters:
  none

  Returns:
  none
*/
void eeStandby_Init(void);


/***********************************************************
  Function: eeStandby_Processing

  processing for the standby module

  Parameters:
  none

  Returns:
  none
*/
void eeStandby_Processing(void);


/***********************************************************
  Function: eeStandby_Processing_Immediate

  processing for the standby module immetiate shutdown request

  Parameters:
  uint8_t    - unused just a place holder 

  Returns:
  none
*/
void eeStandby_Processing_Immediate(uint8_t unused);


/***********************************************************
  Function: eeStandby_CheckWakeUp

  Provides details of the wake up event, if more than one event occurred then the
  highest proirity one will be returned

  Parameters:
  none

  Returns:
  e_Wakeup_t  -  highest priority wake that occurred see <e_Wakeup_t>
*/
ee_Wakeup_t eeStandby_CheckWakeUp(void);


#endif /* __STANDBY_H */

