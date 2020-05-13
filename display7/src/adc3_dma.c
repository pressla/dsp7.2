/*
 * adc_dma.c
 *
 *  Created on: 19.08.2017
 *      Author: kurt
 */

/*
PA0	Uband	ADC123_IN0
PA4	UMESS	ADC12_IN4
PA5	UFWDANT	ADC12_IN5
PA6	UREVANT	ADC12_IN6
PA7	UTEMP	ADC12_IN7

PB0	IMESS	ADC12_IN8
PB1	UFWDIN	ADC12_IN9

PC0	UFWDFLT	ADC123_IN10
PC1	UREVFLT	ADC123_IN11
*/

#include "main.h"

#define ADCMITTELANZ	300
volatile uint16_t ADC3ConvertedValue[ADCMITTELANZ];

void dsp7_ADC3_Init()
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef       DMA_InitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;

  // Enable ADC3, DMA2 and GPIO clocks
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

  // DMA2 Stream1 channel0 configuration
  DMA_InitStructure.DMA_Channel = DMA_Channel_2;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(ADC3_BASE + ADC_DR_OFFSET);
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC3ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = ADCMITTELANZ;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream1, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream1, ENABLE);

  NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE);

  // Configure ADC3 as analog input
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // ADC Common Init
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  // ADC3 Init
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC3, &ADC_InitStructure);

  // ADC3 regular channel0 configuration
  ADC_RegularChannelConfig(ADC3, ADC_Channel_11, 1, ADC_SampleTime_3Cycles);

 // Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

  // Enable ADC3 DMA
  ADC_DMACmd(ADC3, ENABLE);

  // Enable ADC3
  ADC_Cmd(ADC3, ENABLE);

  ADC_SoftwareStartConv(ADC3);   // A/D convert start
}

int ui16_Read_ADC3_ConvertedValue()
{
	unsigned long v=0;
	static unsigned long lastv = 999999;

	for(unsigned long i=0; i<ADCMITTELANZ; i++)
		v += ADC3ConvertedValue[i];

	v /= ADCMITTELANZ;

	// beruhige zitterndes Digit 0
	if(v == (lastv-1) || v == (lastv+1))
	{
		// nur eine unbedeutende Änderung, nehme letzten Wert
		v = lastv;
	}
	else
		lastv = v;

	return (int)((v*2500)/4096);      // Read and return conversion result
}

volatile int ADC3cnv_ready = 0;

void DMA2_Stream1_IRQHandler()
{
	if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1) == SET)
	{
		ADC3cnv_ready = 1;
		DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TCIF1);
	}
}

int isADC3ConvReady()
{
	int ret = ADC3cnv_ready;
	ADC3cnv_ready = 0;

	return ret;
}

