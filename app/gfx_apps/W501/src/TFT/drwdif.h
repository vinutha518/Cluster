/*
***************************************************************************************
***************************************************************************************
***                                                                                 ***
***                                    DRWDIF.H                                     ***
***                                                                                 ***
***            Copyright (C) Stoneridge Electronics AB 1990 - $Date:: 2018#$        ***
***                                                                                 ***
***************************************************************************************
***************************************************************************************/
/** \file
    \ingroup WDIF 

    \Subject    DROME - Widget Drawing Interface header. 

    \Reference  1213_004-35133 01 Drome Software - Widget Specification.doc
    <A HREF="\\Brosrv003\Project\Stoneridge_Electronics\Drome\SW\1213 SW specification">
         Drome Software - Widget Specification.doc
    </A>
    \Abstract 
        The Widget Drawing interface provides high level control of widget drawing. The actual 
        low level rendering of graphics is performed by the Graphics Library (GL).

    \Version history record: 

    $Id: drwdif.h 10 2018-12-03 11:36:39Z stoneridge $
***************************************************************************************
*/

#ifndef DRWDIF_H
#define DRWDIF_H

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
typedef struct
{
   dr_ViewHandle     view_hdl;
   Boolean           visible;
} CurrentViewCtrlT;

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

Public void      drwdif_init( void );
Public dr_Status drwdif_doCyclic( void );
Public void      drwdif_shutDown(void);

Public Boolean   drwdif_drawViewWithKey(U32 view_key);
Public void      drwdif_drawViewWithIndex(U32 index);
Public Boolean   drwdif_forceRedraw(void);
Public void      drwdif_resetCyclicTime(void);
Public U32       drwdif_getCyclicTime(void);
Public U32       drwdif_getCurrentView( void );

#ifdef drome_CONF_WIDGET_VIDEO
Public void      drwdif_enableVideo(Boolean videoEnable);
Public Boolean drwdif_getEnableVideo(void);
#endif

#ifdef drome_CONF_USE_SM_AND_EVENTS
Public Boolean   drwdif_returnViewCtrlToSM(void);

#endif

#endif // DRWDIF_H
