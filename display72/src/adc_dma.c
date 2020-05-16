/*
 * adc_dma.c
 *
 *  Created on: 19.08.2017
 *      Author: kurt
 */

#include "main.h"

#define ADCMITTELANZ	60
#define ADCCHANNELS		10
volatile uint16_t ADC1ConvertedValue[ADCMITTELANZ * ADCCHANNELS];
unsigned char st = ADC_SampleTime_480Cycles;

void dsp7_ADC_Init()
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef       DMA_InitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;

  // Enable ADC1, DMA2 and GPIO clocks
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  // DMA2 Stream0 channel0 configuration
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(ADC1_BASE + ADC_DR_OFFSET);
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC1ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = ADCMITTELANZ * ADCCHANNELS;
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
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE);

	NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);

  // Configure ADC1 as analog input
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // ADC Common Init
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //nur für Multi-ADC mode
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  // ADC1 Init
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = ADCCHANNELS;
  ADC_Init(ADC1, &ADC_InitStructure);

  // ADC1 regular channel0 configuration
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, st);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 2, st);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, st);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 4, st);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 5, st);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 6, st);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 7, st);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 8, st);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 9, st);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 10, st);

 // Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

  // Enable ADC1 DMA
  ADC_DMACmd(ADC1, ENABLE);

  // Enable ADC1
  ADC_Cmd(ADC1, ENABLE);

  ADC_SoftwareStartConv(ADC1);   // A/D convert start
}

// es werden per DMA genau ADCMITTELANZ Messungen pro ADC gemacht
// und im Array ADC1ConvertedValue abgelegt
// dann wird hier aus diesen Messungen der Mittelwert gebildet
int ui16_Read_ADC1_ConvertedValue(int channel)
{
	unsigned long v=0;
	static unsigned long lastv[ADCCHANNELS];

	for(unsigned long i=0; i<ADCMITTELANZ; i++)
		v += ADC1ConvertedValue[i * ADCCHANNELS +channel];

	v /= ADCMITTELANZ;

	// beruhige zitterndes Digit 0
	if(v == (lastv[channel]-1) || v == (lastv[channel]+1))
	{
		// nur eine unbedeutende Änderung, nehme letzten Wert
		v = lastv[channel];
	}
	else
		lastv[channel] = v;

	return (int)((v*2500)/4096);      // Read and return conversion result
}

// Bilde Mittelwert über die letzten SMOOTHLEN Messungen
uint16_t smoothbuf[ADCCHANNELS][SMOOTHLEN];
int ui16_Read_ADC1_ConvertedValueSmooth(int channel)
{
	for(int i=(SMOOTHLEN-1); i>0; i--)
	{
		smoothbuf[channel][i] = smoothbuf[channel][i-1];
	}
	smoothbuf[channel][0] = ui16_Read_ADC1_ConvertedValue(channel);

	// entferne höchsten und niedrigsten Wert
	// und bilde vom Rest den Mittelwert
	uint16_t midval = removeu16Peaks(smoothbuf[channel],SMOOTHLEN);

	return midval;
}

unsigned long peakval_ant = 0;
unsigned long peakval_flt = 0;
unsigned long peakval_drv = 0;

int ui16_Read_ADC_ConvertedPeakValue(int channel)
{
unsigned long *ppeakval = &peakval_flt;

	if(channel == UFWDFLT) ppeakval = &peakval_flt;
	if(channel == UFWDIN) ppeakval = &peakval_drv;
	if(channel == UFWDANT) ppeakval = &peakval_ant;

	// Peakvalue mit Haltezeit für den blauen Balken
	for(int i=0; i<ADCMITTELANZ; i++)
	{
		if(*ppeakval < ADC1ConvertedValue[i * ADCCHANNELS +channel])
			*ppeakval = ADC1ConvertedValue[i * ADCCHANNELS +channel];
	}

	return (int)((*ppeakval*2500)/4096);
}

// Peakvalue ohne Haltezeit für die normale Anzeige, falls das
// anstelle des normalen Mittelwerts gewünscht ist (z.B. für SSB)
int ui16_Read_ADC_ConvertedPeakValue_noHold(int channel)
{
unsigned long v = 0;

	for(int i=0; i<ADCMITTELANZ; i++)
	{
		if(v < ADC1ConvertedValue[i * ADCCHANNELS +channel])
			v = ADC1ConvertedValue[i * ADCCHANNELS +channel];
	}

	return (int)((v*2500)/4096);
}

void clear_peakvals()
{
	peakval_flt = 0;
	peakval_drv = 0;
	peakval_ant = 0;
}

volatile int adccnv_ready = 0;

void DMA2_Stream0_IRQHandler()
{
	if(DMA_GetITStatus(DMA2_Stream0,DMA_IT_TCIF0) == SET)
	{
		adccnv_ready = 1;
		DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
	}
}

int isAdcConvReady()
{
	if(adccnv_ready == 1)
	{
		adccnv_ready = 0;
		return 1;
	}

	return 0;
}

// entferne höchste und niedrigste Werte aus einem uint16_t Array
uint16_t removeu16Peaks(uint16_t buf[], int len)
{
	// bestimme die max und min Werte
	uint16_t min = 65000, max = 0;
	for(int i=0; i<len; i++)
	{
		if(buf[i] > max) max = buf[i];
		if(buf[i] < min) min = buf[i];
	}

	// gibt es einen Wert, der zwischen min und max liegt ?
	// wenn nicht, dann gibt es nur min/max Werte, in dem Fall entferne keine
	int valfound = 0;
	for(int i=0; i<len; i++)
	{
		if(buf[i] < max && buf[i] > min)
		{
			valfound = 1;
			break;
		}
	}

	if(valfound)
	{
		// markiere die min und max Werte Werte
		for(int i=0; i<len; i++)
		{
			if(buf[i] == max || buf[i] == min)
				buf[i] = 65000;
		}
	}

	// bilde Mittelwert über den Rest
	uint32_t mid = 0;
	int anz = 0;
	for(int i=0; i<len; i++)
	{
		if(buf[i] != 65000)
		{
			mid += buf[i];
			anz++;
		}
	}
	mid /= anz;

	// und fülle die markierten Zellen mit dem Mittelwert
	for(int i=0; i<len; i++)
	{
		if(buf[i] == 65000)
			buf[i] = mid;
	}

	return (uint16_t)mid;
}
