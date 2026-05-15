/*
***************************************************************************************
***************************************************************************************
***                                                                                 ***
***                                     drsmif.h                                    ***
***                                                                                 ***
***                                   Project Drome                                 ***
***                                                                                 ***
***          Copyright (C) Stoneridge Electronics AB 1990 - $Date:: 2018#$          ***
***                                                                                 ***
***************************************************************************************
**************************************************************************************/
/** \file
  \ingroup SMIF

  \Subject     Drome state machine interface

  \Reference   None

  \Abstract

     This header file contains the public declarations of the state machine
     interface (DRSMIF), see drsmif.c for details.


  \Version history record:

  $Id: drsmif.h 10 2018-12-03 11:36:39Z stoneridge $

\{ 
***************************************************************************************
*/
#ifndef DRSMIF_H
#define DRSMIF_H

/*
**=====================================================================================
** Imported definitions
**=====================================================================================
*/

#include "drhost.h"
#include "dr.h"

/*
**=====================================================================================
** Public defines
**=====================================================================================
*/


/*
**=====================================================================================
** Public type definitions
**=====================================================================================
*/

/*
**=====================================================================================
** Public data declarations
**=====================================================================================
*/

/*
**=====================================================================================
** Public function declarations and macros
**=====================================================================================
*/

Public void    drsmif_init( void );
Public dr_ViewHandle drsmif_getCurrentView( void );
Public Boolean drsmif_sendEventToStateMachine( dr_EventHandle event );
Public Boolean drsmif_lastTransitionWasDownTransition( void );
Public Boolean drsmif_checkActivationOfGlobalCondition (dr_EventHandle event);

#ifdef HOST_WIN32
Public dr_StateHandle drsmif_getCurrentState( void );
#endif

#endif // DRSMIF
/** \} 
*********************************** End of file **************************************/
