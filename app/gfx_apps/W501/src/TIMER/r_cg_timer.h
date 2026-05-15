/*
 * r_cg_timer.h
 *
 *  Created on: Jan 19, 2019
 *      Author: MSWELT12
 */

#ifndef R_CG_TIMER_H_
#define R_CG_TIMER_H_

extern uint32_t R_TAU0_Create(void);
extern uint32_t R_TAU1_Create(void);

extern uint8_t TimerJ_Init(void);
extern uint32_t R_TAU2_Create(void);
extern void taub_grayscale_isr();

extern void my_tau0_channel0_interrupt();
extern void taub_isr();
extern void taub1_isr();
extern void locTauj0ch0Isr();
extern uint8_t TimerJ_Init();

extern void WatchDog_Clear(void);
extern void seat_belt_chime_condition(void);

#ifdef GRAYSCALE_TFT
extern uint32_t R_TAU2_PWM(void);
extern void taub2_isr();
extern void taub_isr_Master(void);
extern void taub_isr_Slave(void);
#endif 

extern uint32_t R_TAU2_PWM_TFT(void);
extern void taub_isr_Master_TFT(void);
extern void taub_isr_Slave_TFT(void);

extern uint8_t  U8_Reset_DTE;

extern uint32_t R_TAU2_PWM_GCLK(void);
extern void taub_isr_Master_GCLK(void);
extern void taub_isr_Slave_GCLK(void);

#define         COUNT_600uS                     6U
#define 		COUNT_25MS						250U
#define 		COUNT_15mS						150U
#define 		COUNT_100MS        				1000U
#define         COUNT_1MS						10U
/*AFE*/
#define AFE 	1
#endif /* R_CG_TIMER_H_ */
