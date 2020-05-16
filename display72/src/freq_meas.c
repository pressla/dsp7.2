/*
 * freq_meas.c
 *
 *  Created on: 21.09.2017
 *      Author: kurt
 */

#include <main.h>

void TIM6_DAC_IRQHandler(void);
void getFrequency(uint32_t frequency);

#define TIM5_DR      (*((volatile uint32_t *)(TIM5_BASE + 0x24)))
#define TIM5_CCER    (*((volatile uint32_t *)(TIM5_BASE + 0x20)))
#define TIM5_CCMR1   (*((volatile uint32_t *)(TIM5_BASE + 0x18)))
#define TIM5_SMCR    (*((volatile uint32_t *)(TIM5_BASE + 0x08)))

void init_freq_meas()
{
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	// stelle PA1 auf Alternativen Input TIM5_Ch2
	GPIO_InitTypeDef      GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);

	// stelle TIM5 auf maximale Geschwindigkeit ein
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 0xffffffff;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStruct);

	// schalte auf ext. Clock an PA1 (TIM5_Ch2) um
	// TIM5 läuft jetzt mit dem ext. Clock
	TIM_Cmd(TIM5, DISABLE);
	//TIM5->CCMR1 = 0x0500;
	TIM5->SMCR = 0x67;

	TIM_Cmd(TIM5, ENABLE);

	// mache mit TIM6 einen Timeout von 1s (Torzeit)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	// Enable the TIM6 gloabal Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// APB1 hat 90 MHz
	TIM_TimeBaseInitStruct.TIM_Period = 9000;				// hier noch 10kHz
	TIM_TimeBaseInitStruct.TIM_Prescaler = 2500;			// und hier 1Hz
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);

	// Interrupt enable
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

	// TIM3 einschalten
	TIM_Cmd(TIM6, ENABLE);
}

void TIM6_DAC_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
	{
		getFrequency(TIM5->CNT);
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}

float fcorr = 1000000.0/1000520.0;

void getFrequency(uint32_t frequency)
{
static uint32_t lastf = 0;

	float val = frequency - lastf;
	lastf = frequency;

	float f=val*8;
	f = f*fcorr;

	adc_realvals[MFREQUENCY] = f / 1000.0;
}
