/*
 * delay.c
 *
 *  Created on: 13.08.2017
 *      Author: kurt
 */

// Mit TIM3 wird ein 1ms Takt erzeugt, zB für die Delayfunktion

#include "main.h"

void set_Downcounter(unsigned int ms);
int check_Downcounter();

volatile unsigned int tim3_cpt = 0;
volatile unsigned int downcnt = 0;
volatile unsigned int hist_1s = 1000;
volatile unsigned int hist_10ms = 10;
volatile unsigned int wifi_1s = 1000;
volatile unsigned int txtime = 0;
volatile unsigned int swr_filter_fail = 0;
volatile unsigned int swr_ant_fail = 0;
volatile unsigned int fwd_drv_fail = 0;
volatile uint32_t uptime = 0;
volatile int peakval_holdtime = 0;
volatile unsigned int civ_period;
int storeStartMenuFlag = 0;

// Handle TIM3 interrupt, einmal pro ms
void TIM3_IRQHandler(void)
{
static int w10s = 0;

	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		tim3_cpt++;
		uptime++;
		txtime++;
		swr_filter_fail++;
		swr_ant_fail++;
		fwd_drv_fail++;
		if(civ_period > 0) civ_period--;
		if(downcnt > 0) downcnt--;
		if(hist_1s > 0) hist_1s--;
		if(hist_10ms > 0) hist_10ms--;
		if(wifi_1s > 0) wifi_1s--;

		fan_timer();

		if(--peakval_holdtime <= 0)
		{
			clear_peakvals();
			peakval_holdtime = 2000;
		}

		// alle 20 Sekunden sichere aktuelles Menü, falls geändert
		if(++w10s >= 20000)
		{
			w10s = 0;
			storeStartMenuFlag = 1;
		}

		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

void TIM3_Initialization()
{
 	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	// Enable the TIM3 gloabal Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// TIM3 clock enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	// APB1 hat 90 MHz
	TIM_TimeBaseStructure.TIM_Period = 90;			// hier noch 1 MHz
	TIM_TimeBaseStructure.TIM_Prescaler = 1000;		// und hier 1ms
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	// Interrupt enable
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	// TIM3 einschalten
	TIM_Cmd(TIM3, ENABLE);
}

void delay_1ms(int ms)
{
	if(ms == 0) return;

	tim3_cpt=0;
	while(tim3_cpt < ms);
}

void set_Downcounter(unsigned int ms)
{
	downcnt = ms;
}

int check_Downcounter()
{
	return downcnt;
}

