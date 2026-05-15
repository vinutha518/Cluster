/*
***************************************************************************************
***************************************************************************************
***                                                                                 ***
***                                    drmathif.h                                   ***
***                                                                                 ***
***                                   Project Drome                                 ***
***                                                                                 ***
***          Copyright (C) Stoneridge Electronics AB 1990 - $Date:: 2018#$          ***
***                                                                                 ***
***************************************************************************************
**************************************************************************************/
/** \file
  \ingroup Math

  \Subject         DROME - Math.

  \Reference       None

  \Abstract
     This file implements XX.

  \Version history record:

  $Id: drmathif.h 10 2018-12-03 11:36:39Z stoneridge $

\{
***************************************************************************************
*/

#ifndef DRMATHIF_H_
#define DRMATHIF_H_

/*
**=====================================================================================
** Imported definitions
**=====================================================================================
*/

#include "drome.h"
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

typedef S32 FP1616;

#ifdef REAL_AS_FLOATS
typedef float Real;
#else
typedef FP1616 Real;
#endif

/**
 *************************************************************************************
 **
 **   A 3x3 matrix (last row is 0 0 1) used to transform homogeneous two
 **   dimensional vectors.
 **
 **************************************************************************************
 **/
typedef struct{
   Real m[6];        ///< The first and second row
} drmathif_Matrix;

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

/* Use following macros when dealing with Real types in case the representation of
 * the Real type will be changed. */
#ifdef REAL_AS_FLOATS

#include <math.h>

#define REAL_ADD(d0, d1)      ((d0) + (d1))
#define REAL_SUB(d0, d1)      ((d0) - (d1))
#define REAL_MUL(d0, d1)      ((d0) * (d1))
#define REAL_DIV(d0, d1)      ((d0) / (d1))
#define REAL_SIN(d)           (sinf(((d) * 3.1415f) / 180.0f))
#define REAL_COS(d)           (cosf(((d) * 3.1415f) / 180.0f))
#define REAL_SQRT(d)          (sqrt(d))
#define REAL_2                (2.0f)
#define REAL_1                (1.0f)
#define REAL_0_5              (0.5f)
#define REAL_0                (0.0f)
#define REAL_S32TOREAL(d)     ((Real)(d))
#define REAL_U32TOREAL(d)     ((Real)(S32)(d))
#define REAL_S16TOREAL(d)     ((Real)(d))
#define REAL_U16TOREAL(d)     ((Real)(d))
#define REAL_FP1616TOREAL(d)  (((Real)(d)) / 65536.0f)
#define REAL_REALTOS16(d)     ((S16)(d))

#define REAL_REALTOFLOAT(d)   (d)


#else

/*lint -emacro(703,REAL_S32TOREAL) // Shift left of signed quantity (long) */
/*lint -emacro(703,REAL_U16TOREAL) // Shift left of signed quantity (long) */
/*lint -emacro(703,REAL_S16TOREAL) // Shift left of signed quantity (long) */
/*lint -emacro(704,REAL_REALTOS16) // Shift right of signed quantity (long) */

#define REAL_ADD(d0, d1) ((d0) + (d1))
#define REAL_SUB(d0, d1) ((d0) - (d1))
#define REAL_MUL(d0, d1) (drmathif_mulFP1616((d0), (d1)))
#define REAL_DIV(d0, d1) (drmathif_divFP1616((d0), (d1)))
#define REAL_SIN(d)      (drmathif_sinFP1616(d))
#define REAL_COS(d)      (drmathif_cosFP1616(d))
#define REAL_SQRT(d)     (drmathif_sqrtFP1616(d))
#define REAL_2           0x20000L
#define REAL_1           0x10000L
#define REAL_0_25        0x04000L
#define REAL_0_5         0x08000L
#define REAL_0           0
#define REAL_S32TOREAL(d)     (((d))<<16)
#define REAL_U32TOREAL(d)     (((S32)(d))<<16)
#define REAL_S16TOREAL(d)     (((S32)(d))<<16)
#define REAL_U16TOREAL(d)     (((S32)(d))<<16)
#define REAL_FP1616TOREAL(d)  ((Real)(d))
#define REAL_REALTOS16(d)     ((S16)((d)>>16))

#define REAL_REALTOFIXED12_4(d) ((S16)(((d) + 0x000) >> 12))

#define REAL_REALTOFLOAT(d)   ((d) / 65536.0f)


Public FP1616 drmathif_mulFP1616(const FP1616 fp0, const FP1616 fp1);
Public FP1616 drmathif_divFP1616(const FP1616 fp0, const FP1616 fp1);
Public FP1616 drmathif_addFP1616(const FP1616 fp0, const FP1616 fp1);
Public FP1616 drmathif_subFP1616(const FP1616 fp0, const FP1616 fp1);

Public FP1616 drmathif_sinFP1616(const FP1616 angle);
Public FP1616 drmathif_cosFP1616(const FP1616 angle);
Public FP1616 drmathif_sqrtFP1616(const FP1616 fp);

#endif

Public void drmathif_mtrxInit(drmathif_Matrix* matrix_ref, Real a, Real b, Real c, Real d, Real e, Real f);
Public void drmathif_mtrxInitIdentity(drmathif_Matrix* matrix_ref);
Public void drmathif_mtrxInitRotate(drmathif_Matrix* matrix_ref, Real angle);
Public void drmathif_mtrxInitRotateAroundPoint(drmathif_Matrix* matrix_ref, Real angle, Real rot_origin_x, Real rot_origin_y);
Public void drmathif_mtrxInitTranslate(drmathif_Matrix* matrix_ref, Real dx, Real dy);
Public void drmathif_mtrxInitScale(drmathif_Matrix* matrix_ref, Real sx, Real sy);
Public void drmathif_mtrxMul(const drmathif_Matrix* m0_ref, const drmathif_Matrix* m1_ref, drmathif_Matrix* result_ref);
Public void drmathif_mtrxMulRotate(const drmathif_Matrix* m_ref, const Real angle, drmathif_Matrix* result_ref);
Public void drmathif_mtrxMulRotateAroundPoint(const drmathif_Matrix* m_ref, const Real angle, const Real rot_origin_x, const Real rot_origin_y, drmathif_Matrix* result_ref);
Public void drmathif_mtrxMulTranslate(const drmathif_Matrix* m_ref, const Real dx, const Real dy, drmathif_Matrix* result_ref);
Public void drmathif_mtrxMulScale(const drmathif_Matrix* m_ref, const Real sx, const Real sy, drmathif_Matrix* result_ref);
Public void drmathif_mtrxMulScaleAroundPoint(const drmathif_Matrix* m_ref, const Real sx, const Real sy, const Real scale_origin_x, const Real scale_origin_y, drmathif_Matrix* result_ref);
Public void drmathif_mtrxTransf(const drmathif_Matrix* m_ref, const Real x, const Real y, Real* x_ref, Real* y_ref);
Public void drmathif_mtrxTransfArray(const drmathif_Matrix* m_ref, const Real* coords_ref, const U32 num_coord_pairs, Real* result_ref);
Public Boolean drmathif_mtrxEquals(const drmathif_Matrix* m_ref0, const drmathif_Matrix* m_ref1);
#if drome_GLIB_VARIANT == drome_GLIB_DHD
Public void drmathif_mtrxTransfAndConvert(const drmathif_Matrix* m_ref, const U16 x, const U16 y, S16* x_fixed_12_4_ref, S16* y_fixed_12_4_ref);
Public void drmathif_mtrxTransfAndConvertArray(const drmathif_Matrix* m_ref, const U16* coords_ref, const U32 num_coord_pairs, S16* result_fixed_12_4_ref);
#endif

#endif /* DRANIMIF_H_ */
/** \} 
*********************************** End of file **************************************/
