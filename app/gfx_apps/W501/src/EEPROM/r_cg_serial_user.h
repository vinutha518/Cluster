/*
 * r_cg_serial_user.h
 *
 *  Created on: Jan 28, 2019
 *      Author: MSWELT12
 */

#ifndef R_CG_SERIAL_USER_H_
#define R_CG_SERIAL_USER_H_


typedef union
{
	uint8_t Bytes_value[32];
	uint32_t whole_value;
} afe_value;

extern void eeprom_write(uint16_t Address, uint8_t Write_Buffer);
void eeprom_writebt(uint16_t Address, uint8_t Write_Buffer);
extern uint8_t eeprom_read(uint16_t Address );
extern void EEPROM_init (void);
extern void delay(void);
extern void I2C_Release_Bus(void);
extern void IIC_Start_Condition(void);
extern void IIC_Stop_Condition (void);
extern uint8_t IIC_Master_Send(uint8_t output_data);
extern uint8_t IIC_Master_Receive(void);
extern void I2C_Release_Bus(void);
extern void delay_time(uint16_t time_end);
extern void EEPROM_init(void);
extern void I2C_Init(void);
extern void eeprom_memwrite_32bytes(unsigned int memorylocation, unsigned char *Address,unsigned char Number_Of_bytes);
extern void eeprom_memread_32bytes(unsigned int memorylocation, unsigned char *Address,unsigned char Number_Of_bytes);
void eeprom_write_AFE_data(unsigned int memorylocation,uint32_t value);
uint32_t eeprom_read_AFE_data(unsigned int memorylocation);
void eeprom_write_IVN_FC_data(unsigned int memorylocation,uint32_t value);
uint32_t eeprom_read_IVN_FC_data(unsigned int memorylocation);
void Delay ( unsigned short  DelatT);
void E_Delay_i2c(void);
extern void New_eeprom_memwrite_32bytes(unsigned int memorylocation, unsigned char *Address,unsigned char Number_Of_bytes);
void eeprom_DTC_write(uint16_t Address, uint8_t Write_Buffer);
uint8_t eeprom_DTC_read(uint16_t Address);

#endif /* R_CG_SERIAL_USER_H_ */
