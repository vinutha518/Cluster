/*
***************************************************************************************
***************************************************************************************
***                                                                                 ***
***                                    DRFILEID.H                                     ***
***                                                                                 ***
***            Copyright (C) Stoneridge Electronics AB 1990 - $Date:: 2018#$        ***
***                                                                                 ***
***************************************************************************************
***************************************************************************************

    $Id: drfileid.h 10 2018-12-03 11:36:39Z stoneridge $

    Subject: File identification number definition.

    Document reference: 

    Description of this header file:
       This file contains definitions used for unique identification of all source
       files, e.g. for the purpose of error handling, printing and logging.

***************************************************************************************
*/

/*
**=====================================================================================
** This part is for use by all modules
**=====================================================================================
*/

#ifndef DRFILEID_H
#define DRFILEID_H

/*** File identification numbers ***/
#define drfileid_NO_FILE           0        /* No file can have number 0! */
#define drfileid_DBG_C             1
#define drfileid_HWGCFLS_C         2
#define drfileid_HWLLINIT_C        3
#define drfileid_HWMONIF_C         4
#define drfileid_HWFPGA_C          5
#define drfileid_HWTIMER_C         6
#define drfileid_HWMAIN_C          7
#define drfileid_DROME_C           8
#define drfileid_DRGODBIF_C        9
#define drfileid_DRWIDBIF_C        10
#define drfileid_DRSMDBIF_C        11
#define drfileid_DRGLIF_C          12
#define drfileid_DRSMIF_C          13
#define drfileid_DRWDIF_C          14
#define drfileid_HWHOSTIF_C        15
#define drfileid_HWILLUM_C         16
#define drfileid_HWI2C_C           17
#define drfileid_DRDB_C            18
#define drfileid_DRGLGC_C          19
#define drfileid_DRGLGMM_C         20
#define drfileid_DRSM_C            21
#define drfileid_DRGLDISP_C        22
#define drfileid_HWDOTLCD_C        23
#define drfileid_DRWD_C            24
#define drfileid_DRWDVID_C         25
#define drfileid_DRGLVCIF_C        26
#define drfileid_DRWDCTT_C         27
#define drfileid_DRDBIF_C          28
#define drfileid_DRDPIF_C          29
#define drfileid_DRSMEX_C          30
#define drfileid_DRWDTXT_C         31
#define drfileid_DRWDSTW_C         32
#define drfileid_DRWDLST_C         33
#define drfileid_DRSMEVH_C         34
#define drfileid_DRGLPOLY_C        35
#define drfileid_DRWDSPW_C         36
#define drfileid_HWVIDEO_C         37
#define drfileid_HWCSI_C           38
#define drfileid_HWSPI_C           39
#define drfileid_DRGLPR_DHD_C    40
#define drfileid_DRSMHS_C          41
#define drfileid_DRWDSPRITE_C      42
#define drfileid_DRPROPSTACK_C     43
#define drfileid_DRWDANIMCONT_C    44
#define drfileid_DRANIMIF_C        45
#define drfileid_DRSMCM_C          46
#define drfileid_DRGLERRHANDLER_DHD_C 47
#define drfileid_DRGLDISPTIMING_C  48
#define drfileid_DRGLVID_DHD_C   49
#define drfileid_DRWDVIRTUALGAUGE_C 50
#define drfileid_DRWDLAYOUTMANAGER_C 51
#define drfileid_DRWDBBOXMANAGER_C 52

/* Don't forget to update the tables below too! */
/* NOTE: Fileid may not be bigger than one byte with current implementation. */

#endif /* #ifdef DRFILEID_H */

/*
**=====================================================================================
** This part is for use by dbg.c only
**=====================================================================================
*/

#ifdef DRFILEID_DEFINE_INTERPRETATION_TABLE

/*** File identification number interpretation ***/

Private StrConst priv_file_id_000[] = "UNKNOWN";
Private StrConst priv_file_id_001[] = "DBG_C";
Private StrConst priv_file_id_002[] = "HWGCFLS_C";
Private StrConst priv_file_id_003[] = "HWLLINIT_C";
Private StrConst priv_file_id_004[] = "HWMONIF_C";
Private StrConst priv_file_id_005[] = "HWFPGA_C";
Private StrConst priv_file_id_006[] = "HWTIMER_C";
Private StrConst priv_file_id_007[] = "HWMAIN_C";
Private StrConst priv_file_id_008[] = "DROME_C";
Private StrConst priv_file_id_009[] = "DRGODBIF_C";
Private StrConst priv_file_id_010[] = "DRWIDBIF_C";
Private StrConst priv_file_id_011[] = "DRSMDBIF_C";
Private StrConst priv_file_id_012[] = "DRGLIF_C";
Private StrConst priv_file_id_013[] = "DRSMIF_C";
Private StrConst priv_file_id_014[] = "DRWDIF_C";
Private StrConst priv_file_id_015[] = "HWHOSTIF_C";
Private StrConst priv_file_id_016[] = "HWILLUM_C";
Private StrConst priv_file_id_017[] = "HWI2C_C";
Private StrConst priv_file_id_018[] = "DRDB_C";
Private StrConst priv_file_id_019[] = "DRGLGC_C";
Private StrConst priv_file_id_020[] = "DRGLGMM_C";
Private StrConst priv_file_id_021[] = "DRSM_C";
Private StrConst priv_file_id_022[] = "DRGLDISP_C";
Private StrConst priv_file_id_023[] = "HWDOTLCD_C";
Private StrConst priv_file_id_024[] = "DRWD_C";
Private StrConst priv_file_id_025[] = "DRWDVID_C";
Private StrConst priv_file_id_026[] = "DRGLVCIF_C";
Private StrConst priv_file_id_027[] = "DRWDCTT_C";
Private StrConst priv_file_id_028[] = "DRDBIF_C";
Private StrConst priv_file_id_029[] = "DRDPIF_C";
Private StrConst priv_file_id_030[] = "DRSMEX_C";
Private StrConst priv_file_id_031[] = "DRWDTXT_C";
Private StrConst priv_file_id_032[] = "DRWDSTW_C";
Private StrConst priv_file_id_033[] = "DRWDLST_C";
Private StrConst priv_file_id_034[] = "DRSMEVH_C";
Private StrConst priv_file_id_035[] = "DRGLPOLY_C";
Private StrConst priv_file_id_036[] = "DRWDSPW_C";
Private StrConst priv_file_id_037[] = "HWVIDEO_C";
Private StrConst priv_file_id_038[] = "HWCSI_C";
Private StrConst priv_file_id_039[] = "HWSPI_C";
Private StrConst priv_file_id_040[] = "DRGLPR_DHD_C";
Private StrConst priv_file_id_041[] = "DRSMHS_C";
Private StrConst priv_file_id_042[] = "DRWDSPRITE_C";
Private StrConst priv_file_id_043[] = "DRPROPSTACK_C";
Private StrConst priv_file_id_044[] = "DRWDANIMCONT_C";
Private StrConst priv_file_id_045[] = "DRANIMIF_C";
Private StrConst priv_file_id_046[] = "DRSMCM_C";
Private StrConst priv_file_id_047[] = "DRGLERRHANDLER_DHD_C";
Private StrConst priv_file_id_048[] = "DRGLDISPTIMING_C";
Private StrConst priv_file_id_049[] = "DRGLVID_DHD_C";
Private StrConst priv_file_id_050[] = "DRWDVIRTUALGAUGE_C";
Private StrConst priv_file_id_051[] = "DRWDLAYOUTMANAGER_C";
Private StrConst priv_file_id_052[] = "DRWDBBOXMANAGER_C";


/* Don't forget to update the table below too! */

Private const StrConstPtr priv_file_id_interpretation[] =
{
   priv_file_id_000,
   priv_file_id_001,
   priv_file_id_002,
   priv_file_id_003,
   priv_file_id_004,
   priv_file_id_005,
   priv_file_id_006,
   priv_file_id_007,
   priv_file_id_008,
   priv_file_id_009,
   priv_file_id_010,
   priv_file_id_011,
   priv_file_id_012,
   priv_file_id_013,
   priv_file_id_014,
   priv_file_id_015,
   priv_file_id_016,
   priv_file_id_017,
   priv_file_id_018,
   priv_file_id_019,
   priv_file_id_020,
   priv_file_id_021,
   priv_file_id_022,
   priv_file_id_023,
   priv_file_id_024,
   priv_file_id_025,
   priv_file_id_026,
   priv_file_id_027,
   priv_file_id_028,
   priv_file_id_029,
   priv_file_id_030,
   priv_file_id_031,
   priv_file_id_032,
   priv_file_id_033,
   priv_file_id_034,
   priv_file_id_035,
   priv_file_id_036,
   priv_file_id_037,
   priv_file_id_038,
   priv_file_id_039,
   priv_file_id_040,
   priv_file_id_041,
   priv_file_id_042,
   priv_file_id_043,
   priv_file_id_044,
   priv_file_id_045,
   priv_file_id_046,
   priv_file_id_047,
   priv_file_id_048,
   priv_file_id_049,
   priv_file_id_050,
   priv_file_id_051,
   priv_file_id_052
};

#endif /* #ifndef DRFILEID_DEFINE_INTERPRETATION_TABLE */

/********************************** End of file **************************************/

