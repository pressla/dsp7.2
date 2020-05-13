/*
 * gpio.h
 *
 *  Created on: 26.09.2017
 *      Author: kurt
 */

#ifndef GPIO_H_
#define GPIO_H_

void init_GPIOs();
void read_GPIOs();
void set_GPIO();
char rotaryswitch_installed();
void process_extbuttons();
void fan_onoff(int onoff);

#define SW_ONOFF_IN_PA8			GPIO_Pin_8
#define	SW_RESET_IN_PA11		GPIO_Pin_11
#define SW_STANDBY_IN_PA12		GPIO_Pin_12
#define ROTS_AUTO_IN_PA15		GPIO_Pin_15

#define ANT_SEL1_OUT_PB6		GPIO_Pin_6
#define BANDREL_1715_OUT_PB7	GPIO_Pin_7
#define ROTS_80M_IN_PB8			GPIO_Pin_8
#define BANDREL_1210_OUT_PB9	GPIO_Pin_9
#define BANDREL_6_OUT_PB12		GPIO_Pin_12

#define ROTS_6040M_IN_PC4		GPIO_Pin_4
#define ROTS_3020M_IN_PC5		GPIO_Pin_5
#define ROTS_1715M_IN_PC6		GPIO_Pin_6
#define ROTS_1210M_IN_PC7		GPIO_Pin_7
#define ROTS_6M_IN_PC8			GPIO_Pin_8
#define ROTS_160M_IN_PC13		GPIO_Pin_13
#define BANDREL_80_OUT_PC14		GPIO_Pin_14
#define BANDREL_6040_OUT_PC15	GPIO_Pin_15

#define PSOFF_OUT_PD2			GPIO_Pin_2
#define ICOM_TKEY_IN_PD6		GPIO_Pin_6
#define ANT_SEL2_OUT_PD13		GPIO_Pin_13

#define BANDREL_3020_OUT_PE0	GPIO_Pin_0
#define LCD_BACKLIGHT_OUT_PE1	GPIO_Pin_1
#define PTTIN_IN_PE2			GPIO_Pin_2
#define PSON_OUT_PE3			GPIO_Pin_3
#define FAN_OUT_PE4				GPIO_Pin_4
#define BIAS_OUT_PE5			GPIO_Pin_5
#define PTTOUT_OUT_PE6			GPIO_Pin_6

enum _GPIO {
	// Inputs
	SW_RESET = 0,
	SW_STANDBY,
	SW_ONOFF,
	ROTS_B1,
	ROTS_B2,
	ROTS_B3,
	ROTS_B4,
	ROTS_B5,
	ROTS_B6,
	ROTS_B7,
	ROTS_AUTO,
	TKEY,
	PTTIN,
	// Outputs
	LCD_BACKLIGHT_ONOFF,
	ANTSEL1,
	ANTSEL2,
	PWRREL_OFF,
	PWRREL_ON,
	FAN_ONOFF,
	BIAS_ONOFF,
	PTTOUT,
	BANDREL_1,
	BANDREL_2,
	BANDREL_3,
	BANDREL_4,
	BANDREL_5,
	BANDREL_6,
	MAXGPIOS
};


extern uint8_t gpio_stat[MAXGPIOS];

#endif /* GPIO_H_ */
