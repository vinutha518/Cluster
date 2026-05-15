/*
****************************************************************************
Project : Audio generation demo
FILE    : $Id: sound_pcm.h 3922 2014-09-22 08:40:10Z golczewskim $
============================================================================
DESCRIPTION
Audio generation demo Test Application
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

#ifndef __SOUND_PCM_H
#define __SOUND_PCM_H



/***********************************************************
  Section: Defines
*/


/***********************************************************
  Section: Functions
*/


/***********************************************************
  Function: SoundPCM_Init

  Initialisation for the PCM PWM

  Parameters:
  Wakeup - TRUE if wakeup, FALSE if reset

  Returns:
  none
*/
void SoundPCM_Init(uint8_t Wakeup);


/***********************************************************
  Function: SoundPCM_Enable

  Enables the PCM PWM to allow play requests

  Parameters:
  none

  Returns:
  none
*/
void SoundPCM_Enable(void);


/***********************************************************
  Function: SoundPCM_Disable

  Prevents any new play requests, used for shutdown deep sleep

  Parameters:
  none

  Returns:
  none
*/
void SoundPCM_Disable(void);


/***********************************************************
  Function: SoundPCM_Play

  Requests that the sound is played

  Parameters:
  none

  Returns:
  uint8_t    - TRUE if the sound can be requested, FALSE if not
                 i.e. busy playing another request
*/
uint8_t SoundPCM_Play(void);


/***********************************************************
  Function: SoundPCM_IsBusy

  Status of the PCM PWM, used for shutdown deep sleep

  Parameters:
  none

  Returns:
  uint8_t    - TRUE is the PCM PWM is busy, FALSE if not
*/
uint8_t SoundPCM_IsBusy(void);


/***********************************************************
  Function: SoundPCM_Processing

  Round robin processing for the PCM PWM, general housekeeping

  Parameters:
  none

  Returns:
  none
*/
void SoundPCM_Processing(void);
void SoundPCM_Welcome(void);
void SoundPCM_message(void);
void SoundPCM_RPAS_St3(void);
void SoundPCM_Seatbelt_Lv0cont(uint8_t cycles);
void SoundPCM_Seatbelt_Lv0cont_STOP(void);
void SoundPCM_OverSpeed(void);
void SoundPCM_Reverse(void);
void SoundPCM_tick(void);
void SoundPCM_tock(void);
void SoundPCM_RPAS_ST1_ST2(void);
void SoundPCM_KEYIN_SUNROOF(void);
void SoundPCM_4_rec_RPAS_St1_St2_cont(void);
void SoundPCM_4_rec_RPAS_St1_St2_cont_STOP(void);

void SoundPCM_RPAS_LV2(void);
void SoundPCM_RPAS_LV2_STOP(void);

void SoundPCM_RPAS_LV3(void);
void SoundPCM_RPAS_LV3_STOP(void);

void SoundPCM_RPAS_LV4(void);
void SoundPCM_RPAS_LV4_STOP(void);
void SoundPCM_FIRST(void);

void SoundPCM_Reverse_cont(uint8_t cycles);
void SoundPCM_Reverse_cont_STOP(void);

void SoundPCM_Turn_Buzz(void);

void SoundPCM_RPAS_LV1_Buzz(void);
void SoundPCM_RPAS_LV2_Buzz(void);
void SoundPCM_RPAS_LV3_Buzz(void);
void SoundPCM_RPAS_LV4_Buzz(void);
void SoundPCM_RPAS_buzz_LV1_STOP(void);
void SoundPCM_RPAS_buzz_LV2_STOP(void);
void SoundPCM_RPAS_buzz_LV3_STOP(void);
void SoundPCM_RPAS_buzz_LV4_STOP(void);

#endif /* __SOUND_PCM_H */

