/*
 * led.c
 *
 *  Created on: 19.09.2017
 *      Author: kurt
 */


#include <main.h>

void init_LEDs()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB, GPIO_Pin_4);
	GPIO_SetBits(GPIOB, GPIO_Pin_5);

	GPIO_ResetBits(GPIOB, GPIO_Pin_4);
	GPIO_SetBits(GPIOB, GPIO_Pin_4);

	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

void switch_LED(int led, int onoff)
{
	switch (led)
	{
	case LEDRD:
		if(onoff == LEDOFF) GPIO_SetBits(GPIOB, GPIO_Pin_4);
		else GPIO_ResetBits(GPIOB, GPIO_Pin_4);
		break;
	case LEDGN:
		if(onoff == LEDOFF) GPIO_SetBits(GPIOB, GPIO_Pin_5);
		else GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		break;
	}
}

void set_LED()
{
	if(state == STATE_EMERGENCYOFF || state == STATE_POWERUP)
	{
		switch_LED(LEDRD, LEDON);
		switch_LED(LEDGN, LEDOFF);
	}
	else if(state == STATE_ACTIVE)
	{
		switch_LED(LEDRD, LEDON);
		switch_LED(LEDGN, LEDON);
	}
	else
	{
		switch_LED(LEDRD, LEDOFF);
		switch_LED(LEDGN, LEDON);
	}
}
