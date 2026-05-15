/*
 * tachometer.h
 *
 *  Created on: 08-Feb-2019
 *      Author: MSWELT23
 */

#ifndef APP_GFX_APPS_W501_SRC_DIGITAL_GAUGE_TACHOMETER_H_
#define APP_GFX_APPS_W501_SRC_DIGITAL_GAUGE_TACHOMETER_H_
/*
typedef union
{
	uint16_t w;
	uint8_t b[2];
}SplitWord1;
*/
extern Split16 down_scale1;

extern uint32_t final_tachometer_speed;

extern _Bool Tacho_invalid_default;

extern void tachometer(void);

#endif /* APP_GFX_APPS_W501_SRC_DIGITAL_GAUGE_TACHOMETER_H_ */
