/*
***************************************************************************************
***************************************************************************************
***                                                                                 ***
***                                     drsmevh.h                                    ***
***                                                                                 ***
***                                   Project Drome                                 ***
***                                                                                 ***
***          Copyright (C) Stoneridge Electronics AB 1990 - $Date:: 2018#$          ***
***                                                                                 ***
***************************************************************************************
**************************************************************************************/
/** \file
  \ingroup SM

  \Subject     Drome Event handler

  \Reference   None

  \Abstract

     This header file contains the public declarations of the event handler
     implementation, see drsmevh.c for details.


  \Version history record:

  $Id: drsmevh.h 10 2018-12-03 11:36:39Z stoneridge $

\{ 
***************************************************************************************
*/
#ifndef DRSMEVH_H
#define DRSMEVH_H

/*
**=====================================================================================
** Imported definitions
**=====================================================================================
*/

#include "drhost.h"
#include "dr.h"
#include "drwdif.h"

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
Public void      drsmevh_init( void );
Public Boolean   drsmevh_sendEventToWidget(dr_WidgetHandle widget, dr_EventHandle event);
Public Boolean   drsmevh_storeDromeEvent( dr_EventHandle event, Boolean report_error );
Public Boolean   drsmevh_addWidgetToEventHandler( dr_WidgetHandle widget );
Public dr_Status drsmevh_sendEventsToEventHandlers( CurrentViewCtrlT*  priv_view_request, Boolean* view_changed_ref );
Public Boolean   drsmevh_isWidgetInWidWEvent(dr_WidgetHandle widget);
Public void      drsmevh_restoreEventHandlingWidgetListsAfterAbort( void );
Public void      drsmevh_resetEventHandlingWidgetLists( Boolean new_view );

#endif
// DRSMEVH
/** \} 
*********************************** End of file **************************************/
