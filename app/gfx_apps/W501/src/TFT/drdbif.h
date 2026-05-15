/*
***************************************************************************************
***************************************************************************************
***                                                                                 ***
***                                    drdbif.h                                     ***
***                                                                                 ***
***                                   Project Drome                                 ***
***                                                                                 ***
***          Copyright (C) Stoneridge Electronics AB 1990 - $Date:: 2018#$          ***
***                                                                                 ***
***************************************************************************************
**************************************************************************************/
/** \file
  \ingroup DW

  \Subject          DROME -  Drome database common interface.

  \Reference        None

  \Abstract

     This header file contains the public declarations of the database wrapper
     interface (DRDBIF), see drdbif.c for details.


  \Version history record:

  $Id: drdbif.h 10 2018-12-03 11:36:39Z stoneridge $

\{ 
***************************************************************************************
*/

#ifndef DRDBIF_H
#define DRDBIF_H

/*
**=====================================================================================
** Imported definitions
**=====================================================================================
*/

#include "drhost.h"

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

Public void drdbif_init( void * address, void * img_db );
Public void drdbif_shutDown( void );
Public void drdbif_saveNVParams( void );
Public U32 drdbif_getExpectedCRCOfImageDB( void );
Public const U32 * drdbif_getModelCRC( void );
Public const U32 * drdbif_getTextCRC( void );
Public const UTF8 * drdbif_getPartNumber( void );
Public const UTF8 * drdbif_getPartVersion( void );
Public const UTF8 * drdbif_getCompilerVersion( void );

#endif /* DRDBIF_H */
/**
\} // End DW
********************************** End of file **************************************/
