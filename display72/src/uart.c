/*
 * uart.c
 *
 *  Created on: 28.10.2017
 *      Author: kurt
 */

#include <main.h>

void init_USART_DMA_TX(int len);

// ========== UART für die WiFi Anbindung ESP8266 ===============

// USART-2: PA2=TX, PA3=RX
// TX: DMA-1, Stream-6, Channel-4

void init_wifi_uart()
{
	// Enable USART2, DMA2 and GPIO clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1 | RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// GPIO alternative function
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	// init UART-2
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 38400;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStruct);

	/*while(1)
	{
		USART_SendData(USART2,0x55); // OK
	}*/

	// Init DMA
	init_USART_DMA_TX(MAXWIFIDATA);

	// Init NVIC
	// DMA (TX) Transfer Complete IRQ
	NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE); // DMA_IT_TC ... transfer complete

	// USART-2 IRQ
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART2,ENABLE);

	/*while(1)
	{
		memset(wifidata,MAXWIFIDATA,0x55);
		wifi_send(MAXWIFIDATA);
		delay_1ms(1000);
	}*/
}

void init_USART_DMA_TX(int len)
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)wifidata;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = len;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream6, &DMA_InitStructure);
}

void DMA1_Stream6_IRQHandler()
{
	if(DMA_GetITStatus(DMA1_Stream6,DMA_IT_TCIF6) == SET)
	{
		// mach irgendwas
		DMA_ClearITPendingBit(DMA1_Stream6,DMA_IT_TCIF6);
	}
}

void USART2_IRQHandler(void)
{
	if ((USART2->SR & USART_FLAG_RXNE) != (u16)RESET)
	{
		handle_wifi(USART_ReceiveData(USART2) & 0xff);

		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}

	if ((USART2->SR & USART_FLAG_ORE) != (u16)RESET)
	{
		// Receiver overrun
		// führt beim Debuggen zu ununterbrochenem Auslösen des IRQs
		USART_ClearFlag(USART2, USART_FLAG_ORE);
	}
}

// sende Datensatz via DMA zum ESP8266
void wifi_send(int len)
{
	if(!DMA_GetCmdStatus(DMA1_Stream6))
	{
		init_USART_DMA_TX(len);
		DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);
		DMA_Cmd(DMA1_Stream6, ENABLE);
		USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
	}
}

