/***********************************************************************************************
PROJECT : d1mx_mfd_demo app
FILE   : $Id: mem_init.h 4317 2014-11-10 15:03:50Z miodrag.stankovic $
============================================================================ 
DESCRIPTION
============================================================================
                            C O P Y R I G H T
============================================================================
                       Copyright (c) 2013 - 2014
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
#ifndef LOAD_TO_RAM_H_
#define LOAD_TO_RAM_H_

void Copy(r_drw2d_Texture_t *texture_p, r_drw2d_Framebuffer_t *frb_p);
extern void LoadToRam(void);

extern r_drw2d_Texture_t background_texture;
extern r_drw2d_Texture_t background_left_texture;
extern r_drw2d_Texture_t background_right_texture;
extern r_drw2d_Texture_t color_wheel_texture;
extern r_drw2d_Texture_t large_gear_texture;
extern r_drw2d_Texture_t gauge_needle_base_texture;
extern r_drw2d_Texture_t gauge_needle_texture;
extern r_drw2d_Texture_t gauge_shadow_texture;
extern r_drw2d_Texture_t gauge_needle1_texture;
extern r_drw2d_Texture_t gauge_needle2_texture;
extern r_drw2d_Texture_t gauge_needle_light_texture;
extern r_drw2d_Texture_t small_gear_texture;
extern r_drw2d_Texture_t abs_texture;
extern r_drw2d_Texture_t battery_texture;
extern r_drw2d_Texture_t damage_texture;
extern r_drw2d_Texture_t glow_plug_texture;
extern r_drw2d_Texture_t high_beam_texture;
extern r_drw2d_Texture_t low_beam_texture;
extern r_drw2d_Texture_t manual_brake_texture;
extern r_drw2d_Texture_t oil_texture;
extern r_drw2d_Texture_t tank_texture;
extern r_drw2d_Texture_t warning_texture;

extern r_drw2d_Texture_t my_lock_texture;

extern r_drw2d_Texture_t turn_left_off_texture;
extern r_drw2d_Texture_t turn_right_off_texture;
extern r_drw2d_Texture_t turn_left_on_texture;
extern r_drw2d_Texture_t turn_right_on_texture;

/* extern r_drw2d_Texture_t dest_texture; */
/* extern r_drw2d_Texture_t kompass_texture; */
/* extern r_drw2d_Texture_t radio_texture; */
/* extern r_drw2d_Texture_t trip_texture; */
/* extern r_drw2d_Texture_t dot_matrix_bkg_texture; */

extern r_drw2d_Texture_t anim_clock_texture[6];
extern r_drw2d_Texture_t anim_compass_texture[6];
extern r_drw2d_Texture_t anim_perf_texture[6];
extern r_drw2d_Texture_t anim_radio_texture[6];
extern r_drw2d_Texture_t anim_renesas_texture[6];

extern r_drw2d_Texture_t mileage_bkg_cut_texture;
extern r_drw2d_Texture_t mileage_font_texture;
extern r_drw2d_Texture_t mileage_font_orange_texture;

extern r_drw2d_Texture_t odoback_texture;

extern void *car_model_data;

#endif /* LOAD_TO_RAM_H_ */
