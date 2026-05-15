/*
****************************************************************************
PROJECT : VLIB
FILE    : $Id: r_typedefs.h 10 2018-12-03 11:36:39Z stoneridge $
============================================================================ 
DESCRIPTION
Predefined data types
============================================================================
                            C O P Y R I G H T                                    
============================================================================
                           Copyright (c) 2012 
                                  by 
                       Renesas Electronics (Europe) GmbH. 
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing

DISCLAIMER                                                                   
This software is supplied by Renesas Electronics Corporation and is only     
intended for use with Renesas products. No other uses are authorized. This   
software is owned by Renesas Electronics Corporation and is protected under  
all applicable laws, including copyright laws.                               
THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING  
THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT      
LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE   
AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.          
TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS       
ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE  
FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR   
ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE  
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                             
Renesas reserves the right, without notice, to make changes to this software 
and to discontinue the availability of this software. By using this software,
you agree to the additional terms and conditions found by accessing the      
following link:                                                              
http://www.renesas.com/disclaimer *                                          
Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.     

****************************************************************************
*/

#ifndef TYPEDEF_H_
#define TYPEDEF_H_

/*******************************************************************************
*
*                   Defines
*
************************************************************
*/
#pragma ghs startnomisra

#define R_STR(s)           # s
#define R_XSTR(s)          R_STR(s)
#define R_DOT(a,b)         a ## . ## b
#define R_XDOT(a,b)        R_DOT(a,b)
        
#define R_SPACE            " "
#define R_VERSIONPREFIX    "@(#)Version "
#define R_VERSIONPOSTFIX   " - "

#define R_VERSION(macro, hi, lo)        { "@(#)" # macro " driver version "  R_XSTR(hi) "."  R_XSTR(lo)}

#if defined (R_DBG_PRINT_MSG)
#ifndef R_DBG_MSG_LEVEL
    #define R_DBG_MSG_LEVEL 0
#endif

#define R_HALT_ON_ERROR(txt)    {R_DBG_PRINT(R_DBG_MSG_ERR, txt); for ( ; ; ){}}
#define R_DISPLAY_ERROR(txt)     R_DBG_PRINT(R_DBG_MSG_ERR, txt)
#define R_LOG(...)              printf(__VA_ARGS__)

#else
#define R_HALT_ON_ERROR(txt)    {for ( ; ; ){}}
#define R_DISPLAY_ERROR(txt)    
#define R_LOG(...)             
#endif

#define R_INLINE inline

#ifndef UNIT_TEST
#define Private   static
#define Public
#else 
#define Private
#define Public
#endif

#define NOT_USED(_x_) (void)(_x_)

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#pragma ghs endnomisra



/*******************************************************************************
*
*                   Enumerators 
*
************************************************************
*/

#ifdef _WIN32
   /* Necessary since the WIN32 compiler makes enum's 16-bits */
#ifndef UNIT_TEST
   typedef unsigned char Boolean;
   #define TRUE 1
   #define FALSE 0
#else
   typedef enum { FALSE=0U, TRUE=1U }  Boolean; /**< Boolean values */
#endif
#else
   typedef enum { FALSE=0U, TRUE=1U }  Boolean; /**< Boolean values */
#endif


typedef enum
{
  R_ERR_OK = 0u,
  R_NG,
  R_PARAMETER_MISSING,
  R_PARAMETER_RANGE,
  R_PARAMETER_INCORRECT,
  R_DATA_MISMATCH
} r_Error_t;

#define REFRESH_RATE 31
#define REFRESH_RATE_FAST 20
/*******************************************************************************
*
*                   Types
*
************************************************************
*/
typedef unsigned char         U8;      /**<  8-bits, 0..+255 */
typedef signed char           S8;      /**<  8-bits, -128..+127 */
typedef unsigned short        U16;     /**< 16-bits, 0..+65535 */
typedef signed short          S16;     /**< 16-bits, -32768..+32767 */
typedef unsigned long         U32;     /**< 32-bits, 0..+4294967295 */
typedef signed long           S32;     /**< 32-bits, -2147483648..+2147483647 */
typedef unsigned long long    U64;   /**< 64-bit unsigned long long */
typedef signed long long      S64;

/* Define ISO conform exact types, if the compiler does not
   support them,
*/

#ifndef uint8_t
typedef char               char_t;
typedef signed char        int8_t;
typedef signed short       int16_t;  
typedef signed int         int32_t; 
typedef signed long long   int64_t;  
typedef unsigned char      uint8_t;  
typedef unsigned short     uint16_t; 
typedef unsigned int       uint32_t; 
typedef unsigned long long uint64_t;  
typedef float              float32_t;  
typedef double             float64_t;  
#endif /* uint8_t */

/*lint ++flb Suppress global struct member not referenced by starting library section */
/* NB! It is important to end library section! */

/**
 ** 16-bit union that can be \ref U16 or \ref U8 array
 */
typedef union
{
   U16 u16;
   U8  u8[2];
} Split16;

/**
 ** 32-bit union that can be \ref U32, \ref U16 array or \ref U8 array
 */
typedef union
{
   U32 u32;
   U16 u16[2];
   U8  u8[4];
} Split32;

/**
 ** 64-bit union that can be \ref U64, \ref U32 array, \ref U16 array or \ref U8 array
 */
typedef union
{
   U64 u64;
   U32 u32[2];
   U16 u16[4];
   U8  u8[8];
} Split64;



typedef void* handle_t;  

/* Macros that handles bit manipulations of bit arrays */
/*lint -emacro( 572, RESET_ARRAY_BIT, SET_ARRAY_BIT, FLIP_ARRAY_BIT, CHECK_ARRAY_BIT ) */
/*lint -emacro( 778, RESET_ARRAY_BIT, SET_ARRAY_BIT, FLIP_ARRAY_BIT, CHECK_ARRAY_BIT ) */
#define RESET_ARRAY_BIT(array,bit)  ((void)((array)[(bit)>>3] &= ~(U8)(1 << ((bit)&7))))
#define SET_ARRAY_BIT(array,bit)    ((void)((array)[(bit)>>3] |=  (1 << ((bit)&7))))
#define FLIP_ARRAY_BIT(array,bit)   ((void)((array)[(bit)>>3] ^=  (1 << ((bit)&7))))
#define CHECK_ARRAY_BIT(array,bit)  ((array)[(bit)>>3]        &   (1 << ((bit)&7)))

/*
**=====================================================================================
** Bit operation macros
**=====================================================================================
*/

#define BIT_SET(arg, bits)        ((arg) |= (bits))
#define BIT_CLEAR(arg, bits)      ((arg) &= ~(bits))
#define BIT_FLIP(arg, bits)       ((arg) ^= (bits))
#define BIT_TEST(arg, bit)        (((arg) & (bit)) == (bit))
#define BIT_COPY(dest, dbit, src, sbit) (((BIT_TEST(src, sbit)) == 0x01) ? \
                                           BIT_SET(dest, dbit) : BIT_CLEAR(dest, dbit))

/* 
** Bit masks.
** Use these defines primarily to define new, properly named, purpose specific masks.
*/
#define BIT_0       (0x01U)
#define BIT_1       (0x02U)
#define BIT_2       (0x04U)
#define BIT_3       (0x08U)
#define BIT_4       (0x10U)
#define BIT_5       (0x20U)
#define BIT_6       (0x40U)
#define BIT_7       (0x80U)
#define BIT_8       (0x100U)
#define BIT_9       (0x200U)
#define BIT_10      (0x400U)
#define BIT_11      (0x800U)
#define BIT_12      (0x1000U)
#define BIT_13      (0x2000U)
#define BIT_14      (0x4000U)
#define BIT_15      (0x8000U)


/* Version info */
typedef struct 
{
    uint16_t major; 
    uint16_t minor; 
} r_version_t;     


#ifndef NULL
/** Null pointer address (32-bits native type) */
#define NULL            (0UL)
#endif

#ifndef NULL_PTR
/** Null pointer */
#define NULL_PTR        ((void *)0)
#endif


#ifndef R_NULL
#define R_NULL ((void*)0)
#endif /* R_NULL */

#ifndef R_TRUE
#define R_TRUE 1
#endif /* R_TRUE */

#ifndef R_FALSE
#define R_FALSE 0
#endif /* R_FALSE */

#define R_EXTERN extern


#ifdef BIG_ENDIAN

#define SPLIT16_MS_U8      0U /* Most significant U8   */
#define SPLIT16_LS_U8      1U /* Least significant U8  */

#define SPLIT32_MS_U16     0U /* Most significant U16  */
#define SPLIT32_LS_U16     1U /* Least significant U16 */
#define SPLIT32_MS_U8      0U /* Most significant U8   */
#define SPLIT32_2S_U8      1U /* 2:nd significant U8   */
#define SPLIT32_3S_U8      2U /* 3:rd significant U8   */
#define SPLIT32_LS_U8      3U /* Least significant U8  */

#define SPLIT64_MS_U32     0U /* Most significant U32  */
#define SPLIT64_LS_U32     1U /* Least significant U32 */
#define SPLIT64_MS_U16     0U /* Most significant U16  */
#define SPLIT64_2S_U16     1U /* 2:nd significant U16  */
#define SPLIT64_3S_U16     2U /* 3:rd significant U16  */
#define SPLIT64_LS_U16     3U /* Least significant U16 */
#define SPLIT64_MS_U8      0U /* Most significant U8   */
#define SPLIT64_2S_U8      1U /* 2:nd significant U8   */
#define SPLIT64_3S_U8      2U /* 3:rd significant U8   */
#define SPLIT64_4S_U8      3U /* 4:th significant U8   */
#define SPLIT64_5S_U8      4U /* 5:th significant U8   */
#define SPLIT64_6S_U8      5U /* 6:th significant U8   */
#define SPLIT64_7S_U8      6U /* 7:th significant U8   */
#define SPLIT64_LS_U8      7U /* Least significant U8  */


#endif


/*
**====================================================================================
** Constants for use with split data types according to LITTLE_ENDIAN storage style
**====================================================================================
*/
#define LITTLE_ENDIAN

#ifdef LITTLE_ENDIAN

#define SPLIT16_MS_U8      1U /* Most significant U8   */
#define SPLIT16_LS_U8      0U /* Least significant U8  */

#define SPLIT32_MS_U16     1U /* Most significant U16  */
#define SPLIT32_LS_U16     0U /* Least significant U16 */
#define SPLIT32_MS_U8      3U /* Most significant U8   */
#define SPLIT32_2S_U8      2U /* 2:nd significant U8   */
#define SPLIT32_3S_U8      1U /* 3:rd significant U8   */
#define SPLIT32_LS_U8      0U /* Least significant U8  */

#define SPLIT64_MS_U32     1U /* Most significant U32  */
#define SPLIT64_LS_U32     0U /* Least significant U32 */
#define SPLIT64_MS_U16     3U /* Most significant U16  */
#define SPLIT64_2S_U16     2U /* 2:nd significant U16  */
#define SPLIT64_3S_U16     1U /* 3:rd significant U16  */
#define SPLIT64_LS_U16     0U /* Least significant U16 */
#define SPLIT64_MS_U8      7U /* Most significant U8   */
#define SPLIT64_2S_U8      6U /* 2:nd significant U8   */
#define SPLIT64_3S_U8      5U /* 3:rd significant U8   */
#define SPLIT64_4S_U8      4U /* 4:rd significant U8   */
#define SPLIT64_5S_U8      3U /* 5:rd significant U8   */
#define SPLIT64_6S_U8      2U /* 6:rd significant U8   */
#define SPLIT64_7S_U8      1U /* 7:rd significant U8   */
#define SPLIT64_LS_U8      0U /* Least significant U8  */
#endif



#endif /* TYPEDEF_H_ */

