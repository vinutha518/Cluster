/*
****************************************************************************
PROJECT : WM driver
FILE    : $Id: r_wm_sys.h 15280 2017-11-29 07:28:19Z shinya.tomari $
============================================================================ 
DESCRIPTION
Window Manager support functions
============================================================================
                            C O P Y R I G H T
============================================================================
                       Copyright (c) 2013 - 2017
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

/***************************************************************************
  Title: WM support functions for VDCE (internal)
  
  WM (Window Manager) driver support functions

  The generic WM driver uses these functions. They have to be implemented 
  within the driver library for a concrete device. (e.g. D1L, D1M)

  Support functions are functions that are not part of the driver itself 
  but they must be provided to integrate the driver on a particular board.

*/

#ifndef R_WM_SYS_H
#define R_WM_SYS_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************
  Section: Global Defines
*/

/***************************************************************************
  Type: r_wm_DevSys_t

  Device specific features.
  
  Values:
    R_WM_SYS_FEATURE_RLE_LAYER_NO           - Get number of layers that support RLE decoding
    R_WM_SYS_FEATURE_SPRITE_LAYER_NO        - Get number of layers that support Sprite handling
    R_WM_SYS_FEATURE_SWITCH_CAPABILITIES    - Video Output is capable to switch the layers between 
                                              RLE and Sprite mode
    R_WM_SYS_FEATURE_GAMMA_CORRECTION       - Video Output has gamma correction feature
    R_WM_SYS_FEATURE_SCALING                - Video Output has scaling feature
    R_WM_SYS_FEATURE_LAST                   - Delimiter, must be last element

*/
typedef enum
{
    R_WM_SYS_FEATURE_RLE_LAYER_NO,
    R_WM_SYS_FEATURE_SPRITE_LAYER_NO,
    R_WM_SYS_FEATURE_SWITCH_CAPABILITIES,
    R_WM_SYS_FEATURE_GAMMA_CORRECTION,
    R_WM_SYS_FEATURE_SCALING,
    R_WM_SYS_FEATURE_LAST,
} r_wm_sys_DevFeature_t;


/***************************************************************************
  Group: WM Basic interface functions
*/

/***************************************************************************
  Function: R_WM_Sys_DevInit
  
  Description:
    Intitializes the platform specific portion of the device.
    This function is called from <R_WM_DevInit>.

    This function executes the following processing.
      * Initialisation of OS synchronisation variables (if available)
      * Initialisation of internal variables
      * Initialisaiton of Video Output and Sprite Engine

  Customizing Points:
    If a OS is used, please change the OS synchronisation variables accordingly.
    
  Parameter:
    Unit    - The VOUT unit number.
    EventCb - Callback function for receiving events.
    
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_DevInit   (   uint32_t Unit,
                                void (*EventCb)(uint32_t Unit, const r_wm_Event_t *Event)
                            );


/***************************************************************************
  Function: R_WM_Sys_DevDeinit
  
  Description:
    Deinitializes the SYS portion of the device.
    This function is called from <R_WM_DevDeinit>.

    This function executes the following processing.
      * Unlock all possible synchronisation mechnisms that maybe in use by other threads
      * Deinitialisation of Video Output
      * Deinitialisation of internal variables

  Customizing Points:
    If a OS is used, please change the OS synchronisation variables accordingly.
    
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_DevDeinit(uint32_t Unit);


/***************************************************************************
  Function: R_WM_Sys_DevEventRegister
  
  Description:
    Registers for the notification on the external events, i.e.
    interrupts.
    This function is called from <R_WM_DevEventRegister>.

    This function executes the following processing.
      * Configure the parameters of  the respective interrupt identified by 
        <r_wm_EventId_t>. (If applicable)
      * Enable the respective interrupt identified by <r_wm_EventId_t>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
    
  Parameter:
    Unit        - See <R_WM_Sys_DevInit>
    EventId     - The ID of the event
    Arg         - Generic argument (if applicable for the event)
    
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_DevEventRegister  (   uint32_t        Unit,
                                        r_wm_EventId_t  EventId,
                                        uint32_t        Arg
                                    );


/***************************************************************************
  Function: R_WM_Sys_IsShutdownActive
  
  Description:
    In case the driver is doing a time-consuming job, it can check the shutdown 
    status on order to early-exit the job handling.
    This function is called from <R_WM_FrameWait>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
    
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    
  Return value:
    1 if shutdown active, otherwise 0.
*/
uint32_t R_WM_Sys_IsShutdownActive(uint32_t Unit);


/***************************************************************************
  Function: R_WM_Sys_DevCountGet
  
  Description:
    Gets the number of available VOUT units.
    This function is called from several WM APIs.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
    
  Parameter:
    None.
    
  Return value:
    Number of VOUT units.
*/
uint32_t R_WM_Sys_DevCountGet(void);


/***************************************************************************
  Function: R_WM_Sys_DevInfoGet
  
  Description:
    Gets the platform specific information.
    This function is called from <R_WM_DevInit>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
    
  Parameter:
    Unit                    - See <R_WM_Sys_DevInit>
    MaxLayers               -   Maximum number of layers supported
    MaxPitch                -   Maximum layer pitch (stride) supported 
    MaxWidth                -   Maximum layer width supported
    MaxHeight               -   Maximum layer height supported
    SpritesOrderAscending   -   Sprites Z-order ascending or descending.
                                Ascending means that the sprite with the greater index
                                has the greater Z-order priority (comes on top of the sprites
                                with the lower index).
*/
void  R_WM_Sys_DevInfoGet   (   uint32_t    Unit,
                                uint32_t    *MaxLayers,
                                uint32_t    *MaxPitch,
                                uint32_t    *MaxWidth,
                                uint32_t    *MaxHeight,
                                uint32_t    *SpritesOrderAscending
                            );


/***************************************************************************
  Function: R_WM_Sys_DeviceFeature
  
  Description:
    Gets device specific information about certain features.
    This function is called from <R_WM_WindowCapabilitiesSet>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
    
  Parameter:
    Feature               -   Feature support to be checked
    
  Returns:
    Zero if not available or not supports.
    Greater zero if supporterd. Value will number of supported instances if applicable.
*/
uint32_t R_WM_Sys_DeviceFeature(r_wm_sys_DevFeature_t Feature);


/***************************************************************************
  Group: WM Basic internal frame synchronous control functions
*/

/***************************************************************************
  Function: R_WM_Sys_DevFrameStarted
  
  Description:
    Will be called immediately before the HW update for one frame
    starts taking place.
    This function is called from <R_WM_FrameExecuteNext>.

    This function executes the following processing.
      * Reset the per-frame status flags to collect new stati from the current frame

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
  
  Return value:
    None.
*/
void R_WM_Sys_DevFrameStarted(uint32_t Unit);


/***************************************************************************
  Function: R_WM_Sys_DevFrameFinished
  
  Description:
    Will be called immediately after the HW update for one frame
    has been finished.
    This function is called from <R_WM_FrameExecuteNext>.

    This function executes the following processing.
      * Finalize assignment of sprites to HW units and update the registers
      * Finalize assignment of RLE data to HW units and update the registers
      * Update of CLUT registers
      * Update global status flags

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
  
  Return value:
    None.
*/
void R_WM_Sys_DevFrameFinished(uint32_t Unit);


/***************************************************************************
  Function: R_WM_Sys_DevWaitForHwWriteReady
    
  Description:
    Wait until it is safe (in terms of time to finish) to start
    writing the HW registers.
    This function is called from <R_WM_FrameWait>.

    This function executes the following processing.
      * If a OS is used, this function will do a semaphore-wait, otherwise a busy-wait
      * A shutdown request will abort waiting

  Customizing Points:
    If a OS is used, please change the OS synchronisation variables accordingly.
    
  Parameter:
    Unit                - See <R_WM_Sys_DevInit>
  
  Return value:
    None.
*/
void R_WM_Sys_DevWaitForHwWriteReady(uint32_t Unit);


/***************************************************************************
  Function: R_WM_Sys_DevWaitForHwUpdated
  
  Description:
    Wait until the HW registers update is finished.
    This function is called from <R_WM_FrameWait>.

    This function executes the following processing.
      * If a OS is used, this function will do a semaphore-wait, otherwise a busy-wait
      * A shutdown request will abort waiting

  Customizing Points:
    If a OS is used, please change the OS synchronisation variables accordingly.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
  
  Return value:
    None.
*/
void R_WM_Sys_DevWaitForHwUpdated(uint32_t Unit);


/***************************************************************************
  Function: R_WM_Sys_DevRootWindowSet
  
  Description:
    Sets the root window in SYS. This way the SYS layer
    can access the windows chain.
    This function is called from several WM APIs.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
    
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    RootWin - Root window
    
  Return value:
    None.
*/
void R_WM_Sys_DevRootWindowSet(uint32_t Unit, r_wm_Window_t* RootWin); 


/***************************************************************************
  Function: R_WM_Sys_DevRootCaptureSet
  
  Description:
    Sets the capture device root in SYS. This way the SYS layer
    can access the capture devices chain.
    This function is called from several WM APIs.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.

  Parameter:
    Unit        - See <R_WM_Sys_DevInit>
    RootCapt    - Capture root
    
  Return value:
    None.
*/
void R_WM_Sys_DevRootCaptureSet(uint32_t Unit, r_wm_Capture_t* RootCapt);


/***************************************************************************
  Group: WM Video Output Screen interface functions
*/

/***************************************************************************
  Function: R_WM_Sys_ScreenTimingSet
  
  Description:
    Sets the screen timings manually. 
    This function is called from <R_WM_ScreenTimingSet>.

    This function executes the following processing.
      * ScreenTiming is forwared to Video Output
      * Video Output interrupts SCANLINE and VBLANK are configured.
        For reconfiguration of SCANLINE or usage of either interrupt in 
        application code, call <R_WM_DevEventRegister>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Timing  - Timings
    
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_ScreenTimingSet(uint32_t Unit, r_ddb_Timing_t *timing);

    
/***************************************************************************
  Function: R_WM_Sys_ScreenTimingSetByName
  
  Description:
    Sets the video timings by referencing the display database.
    This function is called from <R_WM_ScreenTimingSetByName>.

    This function executes the following processing.
      * <R_WM_Sys_ScreenTimingSet> is called with the respective timing from the DDB.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit                - See <R_WM_Sys_DevInit>
    Name                - Name of the display database. 

  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_ScreenTimingSetByName     (   uint32_t        Unit,
                                                const int8_t    * Name
                                            );


/***************************************************************************
  Function: R_WM_Sys_ScreenEnable
  
  Description:
    Enables/disables the display.
    This function is called from <R_WM_DevDeinit> and <R_WM_FrameExecuteNext>.

    This function executes the following processing.
      * Enabling/Disabling of the Video Output
      * Enabling/Disabling of the respective interrupts

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit                - See <R_WM_Sys_DevInit>
    Enabled             - 0 : disabled, anything else : enabled
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_ScreenEnable(uint32_t Unit, uint32_t Enabled);


/***************************************************************************
  Function: R_WM_Sys_ScreenBgColorSet
  
  Description:
    Sets the background color of the display. The background color 
    can be thought as the lowest Z-order constant color layer.
    This function is called from <R_WM_FrameExecuteNext>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
    
  Parameter:
    Unit                - See <R_WM_Sys_DevInit>
    Red, Green, Blue    - The components of the background color
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_ScreenBgColorSet  (   uint32_t Unit,
                                        uint8_t  Red,
                                        uint8_t  Green,
                                        uint8_t  Blue
                                    );


/***************************************************************************
  Function: R_WM_Sys_ScreenColorCurveSet
  
  Description:
    Set a curve to be used as custom gamma or color correction curve. 
    Using this curve, each RGB color channel is individually corrected according 
    to the given curve.
   
    For the correction in VDCE, the incoming color information of each channel 
    is split into 32 equal segments each covering 8 color values.
    For these 8 values, the same gain factor applies.
   
    To configure the segments, a start and an end value need to be given. 
    This requires 33 reference points to be passed to this function.
   
    For each segment of each color, the gain factor must be in range [0.0 .. 2.0], 
    thus the values between two reference points may have a difference in range of [0 .. 16].
   
    This function will overwrite the settings of <R_WM_ScreenGammaSet>.

    This function is called from <R_WM_FrameExecuteNext>.

    This function executes the following processing.
      * Color correction is forwared to Video Output

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit         - See description of <R_WM_DevInit>
    ColorCurve   - Pointer to Table of reference points <r_wm_ClutEntry_t>. 
                   ALPHA value of data type is unused!
                   Note: The data pointed to by the pointer <Clut> must be avalilable 
                         as long as the Window using the CLUT color format is active.
    NumEntries   - Number of reference points
    
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_ScreenColorCurveSet(  uint32_t Unit,
                                        const r_wm_ClutEntry_t *ColorCurve,
                                        uint32_t NumEntries
                                      );


/***************************************************************************
  Function: R_WM_Sys_ScreenGammaSet

  Description:
    This function sets the output gamma correction.
    
    This function will overwrite the settings of <R_WM_ScreenColorCurveSet>.
    This function is called from <R_WM_FrameExecuteNext>.

    This function executes the following processing.
      * Gamma correction is forwared to Video Output

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.

  Parameter:
    Unit       - See description of <R_WM_DevInit>
    GammaRed   - Gamma correction factor (0..255 => 0.25..2.0, 128=1.0)
    GammaGreen - Gamma correction factor (0..255 => 0.25..2.0, 128=1.0)
    GammaBlue  - Gamma correction factor (0..255 => 0.25..2.0, 128=1.0)    

  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_ScreenGammaSet(   uint32_t Unit, 
                                    const uint8_t GammaRed, 
                                    const uint8_t GammaGreen, 
                                    const uint8_t GammaBlue 
                                );
    
    
/***************************************************************************
  Function: R_WM_Sys_ScreenColorFormatSet

  Description:
    Set the color format for the signals of the specified video output.
    Default format for the video output is RGB888.
    This function is called from <R_WM_ScreenColorFormatSet>.

    This function executes the following processing.
      * Configuration of color bits and dithering for lower bit configuration
      * Configuration of pin order and endianness representing the color bits

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.

  Parameter:
    Unit   - See <R_WM_Sys_DevInit>
    OutFmt - A color format of <r_wm_OutColorFmt_t> plus optional bit flags.

  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_ScreenColorFormatSet  (   uint32_t            Unit,
                                            r_wm_OutColorFmt_t  OutFmt
                                        );


/***************************************************************************
  Group: WM Video Output Windows interface functions
*/

/***************************************************************************
  Function: R_WM_Sys_WindowSetFb
  
  Description:
    Sets the visible (front) buffer for the window.
    This function is called from <R_WM_FrameExecuteNext>.

    This function executes the following processing.
      * Update the image data visible on the Video Output

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit    -   See <R_WM_Sys_DevInit>
    Win     -   Window structure pointer, see <r_wm_Window_t>
    Fb      -   Buffer
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowSetFb(uint32_t Unit, const r_wm_Window_t* Win, const void* Fb);


/***************************************************************************
  Function: R_WM_Sys_WindowCapabilitiesSet

  Description:
    Configure the Video Output Layers to decode RLE compressed textures or to 
    show sprite windows. Only one feature may be selected for each layer.
    This function is called from <R_WM_WindowCapabilitiesSet>.

    D1M1A only! Only to be called if the video output is inactive (or both 
    Video Outputs, if applicable).

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.

  Parameter:
    Capability0  - Selects mode for VO0 Layer0 and VO1 Layer0
    Capability1  - Selects mode for VO0 Layer1 and VO1 Layer3
    Capability2  - Selects mode for VO0 Layer2 and VO1 Layer2
    Capability3  - Selects mode for VO0 Layer3 and VO1 Layer1

  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowCapabilitiesSet (   r_wm_WinCapbs_t Capability0,
                                            r_wm_WinCapbs_t Capability1,
                                            r_wm_WinCapbs_t Capability2,
                                            r_wm_WinCapbs_t Capability3
                                         );

    
/***************************************************************************
  Function: R_WM_Sys_WindowCreate
  
  Description:
    Creates a window.
    This function is called from <R_WM_FrameExecuteNext>.

    This function executes the following processing.
      * Update all layer related registers to configure the Video Ouput
        to display a layer and all its properties.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
    
  Parameter:
    Unit                - See <R_WM_Sys_DevInit>
    Window              - Window structure pointer, see <r_wm_Window_t>
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowCreate(uint32_t Unit, const r_wm_Window_t* Win);

    
/***************************************************************************
  Function: R_WM_Sys_WindowDelete
  
  Description:
    Deletes a window.
    This function is called from <R_WM_FrameExecuteNext>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
    
  Parameter:
    Unit                - See <R_WM_Sys_DevInit>
    Window              - Window structure pointer, see <r_wm_Window_t>
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowDelete(uint32_t Unit, const r_wm_Window_t* Win);

    
/***************************************************************************
  Function: R_WM_Sys_WindowEnable
  
  Description:
    Enables/disables the window.
    This function is called from <R_WM_FrameExecuteNext>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Win     - Window structure pointer, see <r_wm_Window_t>
    Enabled - Enabled/Disabled
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowEnable(uint32_t Unit, const r_wm_Window_t* Win, uint32_t Enabled);


/***************************************************************************
  Function: R_WM_Sys_WindowPosSet
  
  Description:
    Sets the window position.
    This function is called from <R_WM_FrameExecuteNext>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit                    - See <R_WM_Sys_DevInit>
    Win                     - Window structure pointer, see <r_wm_Window_t>
    PosX, PosY, and PosZ    - New window position
  
  Return value:
    1 if successful, otherwise 0.
    
  Note:
     Setting the PosZ can yield reconfiguring all HW layers.
*/
uint32_t R_WM_Sys_WindowPosSet(uint32_t Unit, const r_wm_Window_t* Win, int32_t PosX, int32_t PosY, int32_t PosZ);


/***************************************************************************
  Function: R_WM_Sys_WindowGeomSet
  
  Description:
    Sets the window dimensions.
    This function is called from <R_WM_FrameExecuteNext>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Win     - Window structure pointer, see <r_wm_Window_t>
    Pitch   - The distance in pixels between subsequent rows in the framebuffer memory (>= Width)
    Width   - The window framebuffer width in pixels
    Height  - The window framebuffer height in pixels
  
  Return value:
    1 if successful, otherwise 0.
  
  D1Mx specific:
    The pitch must be 128 bytes aligned.
*/
uint32_t R_WM_Sys_WindowGeomSet (   uint32_t            Unit,
                                    const r_wm_Window_t *Win,
                                    uint32_t            Pitch,
                                    uint32_t            Width,
                                    uint32_t            Height
                                );


/***************************************************************************
  Function: R_WM_Sys_WindowColorFmtSet
  
  Description:
    Changes the color format of the window. Every window has an initial
    color format specified in its structure. It is set after issuing
    <R_WM_WindowCreate> and can be changed during window life-time
    by using this function.
    This function is called from <R_WM_FrameExecuteNext>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit        -   See <R_WM_Sys_DevInit>
    Win         -   Window structure pointer, see <r_wm_Window_t>
    ColorFmt    -   See <r_wm_WinColorFmt_t>
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowColorFmtSet (   uint32_t                Unit,
                                        const r_wm_Window_t     *Win,
                                        r_wm_WinColorFmt_t      ColorFmt
                                    );


/***************************************************************************
  Function: R_WM_Sys_WindowAlphaSet
  
  Description:
    Sets the constant (overall) alpha value for the window. This
    value is then used in alpha blending of the stacked windows.
    This function is called from <R_WM_FrameExecuteNext>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Win     - Window structure pointer, see <r_wm_Window_t>
    Alpha   - Alpha value (0-255)
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowAlphaSet(uint32_t Unit, const r_wm_Window_t* Win, uint8_t Alpha);

    
/***************************************************************************
  Function: R_WM_Sys_WindowPremultipliedAlphaEnable
  
  Description:
    Enables or disables the premultiplied alpha mode for the window. 
    This changes the formula used in alpha blending of the stacked windows.
    This function is called from <R_WM_FrameExecuteNext>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Win     - Window structure pointer, see <r_wm_Window_t>
    Enabled - True/False
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowPremultipliedAlphaEnable (uint32_t Unit, const r_wm_Window_t* Win, uint8_t Enabled);

    
/***************************************************************************
  Function: R_WM_Sys_WindowFlagsUpdate
  
  Description:
    Provides various on/off switches for different functionalities of a WM window.
    Check <r_wm_WinFlags_t> for further descriptions.
    This function is called from <R_WM_FrameExecuteNext>.

    This function executes the following processing.
      * Update various settings like screen mirroring

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit        - See <R_WM_Sys_DevInit>
    Win         - Window structure pointer, see <r_wm_Window_t>
    SetFlags    - New flags to be set
    ClearFlags  - New flags to be cleared
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowFlagsUpdate (uint32_t Unit, const r_wm_Window_t* Win, r_wm_WinFlags_t SetFlags, r_wm_WinFlags_t ClearFlags);

    
/***************************************************************************
  Function: R_WM_Sys_WindowClutSet
  
  Description:
    Sets the color lookup-table for the window. This is only meaningful
    in case of a CLUT window color format, see <r_wm_WinColorFmt_t>.
    This function is called from <R_WM_FrameExecuteNext>.

    This function executes the following processing.
      * The CLUT will be updated when processing of the current frame is finished.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit        - See <R_WM_Sys_DevInit>
    Win         - Window structure pointer, see <r_wm_Window_t>
    NumEntries  - The number of color lookup-table entries
    Clut        - The color lookup-table pointer
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowClutSet(uint32_t Unit, const r_wm_Window_t* Win, uint32_t NumEntries, const r_wm_ClutEntry_t* Clut);

    
/***************************************************************************
  Function: R_WM_Sys_WindowColorKeyEnable
  
  Description:
    Enables/disables the color keying for the window. The color key
    is set in the window structure during initialization.
    This function is called from <R_WM_FrameExecuteNext>.

  Parameter:
    Unit    -   See <R_WM_Sys_DevInit>
    Win     -   Window structure pointer, see <r_wm_Window_t>
    Enabled -   Enabled/Disabled
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowColorKeyEnable(uint32_t Unit, const r_wm_Window_t* Win, uint32_t Enabled);


/***************************************************************************
  Group: WM Video Output Sprite interface functions
*/

/***************************************************************************
  Function: R_WM_Sys_SpriteCreate
  
  Description:
    Add a sprite to the the window. The window must have its 'Mode' set
    to R_WM_WINMODE_SPRITES. Upon creation sprites are disabled. Sprite origin
    are calculated relative to the parent window origin.
    After successful creation, the window's 'Status' field will be set to
    R_WM_SPRITESTATUS_ENABLED.
    This function is called from <R_WM_FrameExecuteNext>.

    This function executes the following processing.
      * Update all sprite related registers to configure the Video Ouput
        to display a sprite at the given position.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Sprite  - Sprite structure pointer, see <r_wm_Sprite_t>
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_SpriteCreate(uint32_t Unit, const r_wm_Sprite_t *Sprite);

    
/***************************************************************************
  Function: R_WM_Sys_SpriteEnable
  
  Description:
    Enables/disabled the sprite. Upon creation sprites are disabled.
    This function is called from <R_WM_FrameExecuteNext>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Sprite  - Sprite structure pointer, see <r_wm_Sprite_t>
    Enabled - Enabled/disabled
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_SpriteEnable(uint32_t Unit, const r_wm_Sprite_t *Sprite, uint32_t Enabled);

    
/***************************************************************************
  Function: R_WM_Sys_SpriteDelete
  
  Description:
    Deletes the sprite from the window. After successful deletion, 
    the sprite's 'Status' field will be changed to 
    R_WM_SPRITESTATUS_NOT_INITIALIZED.
    This function is called from <R_WM_FrameExecuteNext>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Sprite  - Sprite structure pointer, see <r_wm_Sprite_t>

  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_SpriteDelete(uint32_t Unit, const r_wm_Sprite_t *Sprite);


/***************************************************************************
  Function: R_WM_Sys_SpriteMove
  
  Description:
    Moves the sprite on the parent window.
    This function is called from <R_WM_FrameExecuteNext>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit                - See <R_WM_Sys_DevInit>
    Sprite              - Sprite structure pointer, see <r_wm_Sprite_t>
    PosX, PosY, PosZ    - New sprite position
  
  Return value:
    1 if successful, otherwise 0.
    
  Note:
    Changing PosZ can yield HW reconfiguration of all sprites on the same window.
*/
uint32_t R_WM_Sys_SpriteMove(uint32_t Unit, const r_wm_Sprite_t *Sprite, uint32_t PosX, uint32_t PosY, uint32_t PosZ);


/***************************************************************************
  Function: R_WM_Sys_SpriteBufSet
  
  Description:
    Sets the sprite's buffer.
    This function is called from <R_WM_FrameExecuteNext>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Window  - Window structure pointer, see <r_wm_Window_t>

  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_SpriteBufSet(uint32_t Unit, const r_wm_Sprite_t *Sprite, void* Buf);


/***************************************************************************
  Function: R_WM_Sys_WindowDeleteAllSprites
  
  Description:
    Deletes all sprites on the window. This function is guaranteed
    to be faster than deleting sprites by issuing <R_WM_SpriteDelete>
    on all sprites on the window.
    This function is called from <R_WM_FrameExecuteNext>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Win     - Window structure pointer, see <r_wm_Window_t>

  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_WindowDeleteAllSprites    (   uint32_t Unit,
                                                const r_wm_Window_t* Win
                                            );
    
    
/***************************************************************************
  Group: Video Capture interface functions
*/

/***************************************************************************
  Function: R_WM_Sys_CaptureCreate
  
  Description:
    The HW specific part of the <R_WM_CaptureCreate>.
    This function is called from <R_WM_FrameExecuteNext>.

    This function executes the following processing.
      * Reconfigure the Pins for Video Input in case of ITU mode selected
      * Configure memory geometry and addresses
      * Configure deinterlacing mode
      * Configure color space conversion in case of ITU mode selected
      * Configure VI_VBLANK interrupt

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Capt    - Capture device object pointer, see <r_wm_Capture_t>

  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_CaptureCreate(uint32_t Unit, const r_wm_Capture_t *Capt);

    
/***************************************************************************
  Function: R_WM_Sys_CaptureDelete
  
  Description:
    The HW specific part of the <R_WM_CaptureDelete>.
    This function is called from <R_WM_FrameExecuteNext>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.

  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Capt    - Capture device pointer, see <r_wm_Capture_t>
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_CaptureDelete(uint32_t Unit, const r_wm_Capture_t *Capt);

    
/***************************************************************************
  Function: R_WM_Sys_CaptureEnable
  
  Description:
    The HW specific part of the <R_WM_CaptureEnable> and <R_WM_CaptureDisable>.
    This function is called from <R_WM_FrameExecuteNext>.

    This function executes the following processing.
      * Enable VI_VBLANK interrupt handling
      * Enable capturing and writing to memory

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Window  - Window structure pointer, see <r_wm_Window_t>
    Enabled - Enabled/disabled flag
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_CaptureEnable(uint32_t Unit, const r_wm_Capture_t *Cap, uint32_t Enabled);


/***************************************************************************
  Group: WM Messaging interface functions.
*/

/***************************************************************************
  Function: R_WM_Sys_MsgQueueSetup
  
  Description:
    Setup the message queue.
    This function is called from <R_WM_DevInit>.

    This function executes the following processing.
      * Initialisation of internal ringbuffer for WM configuration messages

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit                -   See <R_WM_Sys_DevInit>
    MsgQueueStorage     -   Storage for the message queue
    Size                -   Number of elements in the message queue
    
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_MsgQueueSetup(uint32_t Unit, void* MsgQueueStorage, uint32_t Size);

    
/***************************************************************************
  Function: R_WM_Sys_MsgQueueRead
  
  Description:
    Reads one message and removes it from the queue.
    This function is called from <R_WM_FrameExecuteNext>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Msg     - Pointer to the message, see <r_wm_Msg_t>
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_MsgQueueRead(uint32_t Unit, r_wm_Msg_t *Msg);

    
/***************************************************************************
  Function: R_WM_Sys_MsgQueueWrite
  
  Description:
    Writes one message to the queue.
    This function is called from <R_WM_MsgEnqueue>.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    Msg     - Pointer to the message, see <r_wm_Msg_t>
  
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_MsgQueueWrite(uint32_t Unit, r_wm_Msg_t *Msg);

    
/***************************************************************************
  Group: WM Memory functions
*/

/***************************************************************************
  Function: R_WM_Sys_Heap_Set

  Description:
    This will set the platform specific heaps.
    This function is called from <R_WM_DevInit>.

    This function executes the following processing.
      * The referenced heaps will be stored driver-internal.
      * The heaps are valid for both instances

  Customizing Points:
    If a custom allocator is used, please update this function accordingly.

*/
void R_WM_Sys_Heap_Set(const void *Cpu, const void *Video);


/***************************************************************************
  Function: R_WM_Sys_Alloc
  
  Description:
    Allocates memory. This function will be used by the framework
    if the window FB mode is R_WM_WINBUF_ALLOC_INTERNAL.
    This function is called from <R_WM_Create>.

  Customizing Points:
    If a custom allocator is used, please update this function accordingly.
  
  Parameter:
    Size                - Size of the memory block
    MemType             - The type of the memory to be allocated, <see r_wm_Memory_t>
    
  Return value:
    Pointer to the allocated memory.
*/
void *R_WM_Sys_Alloc    (   uint32_t      Size,
                            r_wm_Memory_t MemType
                        );

    
/***************************************************************************
  Function: R_WM_Sys_Free
  
  Description:
    Deallocates memory allocated with <R_WM_Sys_Free>. This function will
    be used by the framework if the window FB mode is R_WM_WINBUF_ALLOC_INTERNAL.
    This function is called from several WM APIs.

  Customizing Points:
    If a custom allocator is used, please update this function accordingly.
    
  Parameter:
    Memory      - Memory pointer
    MemType     - The type of the memory to be allocated, <see r_wm_Memory_t>
    
  Return value:
    1 if successful, otherwise 0.
   
*/
uint32_t R_WM_Sys_Free  (   void         *Memory,
                            r_wm_Memory_t MemType
                        );


/***************************************************************************
  Group: WM OS interface functions
*/

/***************************************************************************
  Function: R_WM_Sys_LockWindows
  
  Description:
    This function locks the windows of WM driver access to the specified unit from other threads.
    This function is called from several WM APIs.

  Customizing Points:
    If a OS is used and the WM API is called from multiple threads, 
    please implement the lock process by mutex or semaphore.

    
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    
  Return value:
    None.
*/
void R_WM_Sys_LockWindows(uint32_t Unit);


/***************************************************************************
  Function: R_WM_Sys_UnlockWindows
  
  Description:
    This function unlocks the window of WM driver access to the specified unit from other threads.
    This function is called from several WM APIs.

  Customizing Points:
    If a OS is used and the WM API is called from multiple threads, 
    please implement the lock process by mutex or semaphore.
    
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    
  Return value:
    None.
*/
void R_WM_Sys_UnlockWindows(uint32_t Unit);


/***************************************************************************
  Function: R_WM_Sys_LockMsgQueue
  
  Description:
    This function locks the message queue of WM driver access to the specified unit from other threads.
    This function is called from several WM APIs.

  Customizing Points:
    If a OS is used and the WM API is called from multiple threads, 
    please implement the lock process by mutex or semaphore.
    
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    
  Return value:
    None.
*/
void R_WM_Sys_LockMsgQueue(uint32_t Unit);

    
/***************************************************************************
  Function: R_WM_Sys_TryLockMsgQueue
  
  Description:
    This function try-locks the message queue of WM driver access to the specified unit from other threads.
    This function is called from several WM APIs.

  Customizing Points:
    If a OS is used and the WM API is called from multiple threads, 
    please implement the lock process by mutex or semaphore.
    
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    
  Return value:
    1 if successful, otherwise 0.
*/
uint32_t R_WM_Sys_TryLockMsgQueue(uint32_t Unit);


/***************************************************************************
  Function: R_WM_Sys_UnlockMsgQueue
  
  Description:
    This function unlocks the message queue of WM driver access to the specified unit from other threads.
    This function is called from several WM APIs.

  Customizing Points:
    If a OS is used and the WM API is called from multiple threads, 
    please implement the lock process by mutex or semaphore according to <R_WM_Sys_LockMsgQueue>.
    
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    
  Return value:
    None.
*/
void R_WM_Sys_UnlockMsgQueue(uint32_t Unit);


/***************************************************************************
  Function: R_WM_Sys_LockBuffers
  
  Description:
    This function locks the buffers of WM driver access to the specified unit from other threads.
    This function is called from several WM APIs.

  Customizing Points:
    If a OS is used and the WM API is called from multiple threads, 
    please implement the lock process by mutex or semaphore.
    
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    
  Return value:
    None.
*/
void R_WM_Sys_LockBuffers(uint32_t Unit);


/***************************************************************************
  Function: R_WM_Sys_UnlockBuffers
  
  Description:
    This function unlocks the buffers of WM driver access to the specified unit from other threads.
    This function is called from several WM APIs.

  Customizing Points:
    If a OS is used and the WM API is called from multiple threads, 
    please implement the lock process by mutex or semaphore according to <R_WM_Sys_LockBuffers>.
    
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    
  Return value:
    None.
*/
void R_WM_Sys_UnlockBuffers(uint32_t Unit);


/***************************************************************************
  Function: R_WM_Sys_LockDevice
  
  Description:
    This function locks the device of WM driver access to the specified unit from other threads.
    This function is called from several WM APIs.

  Customizing Points:
    Please implement the lock process by mutex or semaphore if WM API is called from 
    multi-thread.
    
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    
  Return value:
    None.
*/
void R_WM_Sys_LockDevice(uint32_t Unit);


/***************************************************************************
  Function: R_WM_Sys_UnlockDevice
  
  Description:
    This function unlocks the device of WM driver access to the specified unit from other threads.
    This function is called from several WM APIs.

  Customizing Points:
    If a OS is used and the WM API is called from multiple threads, 
    please implement the lock process by mutex or semaphore according to <R_WM_Sys_LockDevice>.
    
  Parameter:
    Unit    - See <R_WM_Sys_DevInit>
    
  Return value:
    None.
*/
void R_WM_Sys_UnlockDevice(uint32_t Unit);


/***************************************************************************
  Group: WM Other interface functions
*/

/***************************************************************************
  Function: R_WM_Sys_GetLastError

  Description:
    Due to technical reasons, the WM SYS layer only returns success/failure without specific error codes.
    Using this function, you can request information about the last error that happened in the SYS layer of the WM.
    Calling this function will reset the error flags and allows for a new error to be recorded.
    For further debugging, the function may be expanded to provide <File> and <Line> of the error.
  
    To use this function, the WM SYS layer must be recompiled with the flag R_WM_SYS_ERROR_TRACKING.

  Customizing Points:
    It is NOT necessary to modify this function in general use-case.
  
  Parameter:
  Unit              - Reference parameter to optionally get the WM Unit that caused the error
  AdditionalInfo    - Reference parameter to optionally get additional error codes from VDCE or SPEA
  Overflow          - Reference parameter to optionally get information if errors happened after this one occurred
  New               - Reference parameter to optionally check if this is a new error with respect to the last call to R_WM_Priv_GetLastError
  
  Returns:
  r_wm_Error_t      - Error code of the error.
*/

r_wm_Error_t R_WM_Sys_GetLastError( uint32_t *Unit, 
                                        uint32_t *AdditionalInfo, 
                                        uint32_t *Overflow, 
                                        uint32_t *New);




#ifdef __cplusplus
}
#endif  


#endif /* R_WM_SYS_H__ */
