/*
 * odometer.h
 *
 *  Created on: Feb 17, 2019
 *      Author: MSWELT12
 */

#include "Memmory_Map.h"
#ifndef ODOMETER_H_
#define ODOMETER_H_

#define ODO_MAX 999999000			//odo reset solve
#define NO_OF_ODOMETER_VALUES 25

#define PARAM_ODOMETER_BUFFER ODOMETER_OFFSET1
#define PARAM_TRIP1_ODOMETER_BUFFER TRIP1_METER_OFFSET
#define PARAM_TRIP2_ODOMETER_BUFFER TRIP2_METER_OFFSET
#define PARAM_TRIP_AC_ODOMETER_BUFFER TRIP_AC_METER_OFFSET
#define PARAM_TRIP_AC_ODOMETER_BUFFER1 TRIP_AC_METER_OFFSET1
#define PARAM_TRIP_AC_TIMER_ODOMETER_BUFFER AC_TIMER_OFFSET

extern void eeprom_writeOdometer(int32_t new_odometer);
extern int32_t eeprom_readOdometer(void);

void eeprom_readEeprom(int16_t ee_addr, uint8_t len, uint8_t *ptr);
void eeprom_writeEeprom(int16_t ee_addr, uint8_t len, uint8_t *ptr);

extern void eeprom_write_Trip1_Odometer(int32_t new_odometer);
extern void eeprom_write_Trip2_Odometer(int32_t new_odometer);
extern void eeprom_write_Trip_AC_Odometer(int32_t new_odometer);
extern void eeprom_write_Trip_AC_Odometer1(int32_t new_odometer);
extern void eeprom_write_Trip_AC_Offset(int32_t new_odometer);

extern int32_t eeprom_readTrip1_Odometer(void);
extern int32_t eeprom_readTrip2_Odometer(void);
extern int32_t eeprom_readTrip_AC_Odometer(void);
extern int32_t eeprom_readTrip_AC_Odometer1(void);
extern int32_t eeprom_readTrip_AC_Offset(void);

extern void DID_ODO_Update(void);

static uint8_t priv_odometer_index;

extern _Bool invalid_ODO_input;

#endif /* ODOMETER_H_ */
