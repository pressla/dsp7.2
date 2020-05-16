/*
 * uart.c
 *
 *  Created on: 28.10.2017
 *      Author: kurt
 */

#include <main.h>

/*
 *
Controller fordert Frequenz an:
fe, fe, 00, e0, 03, fd

wenn im TRX der "Transceive" modus aktiviert ist,
dann sendet der TRX bei jedem Frequenzwechsel von selbst.

TRX antwortet:
fe, fe, e0, adr, 03, freq, fd

Codierung von freq:

5 bytes:

12 34 56 78 90 bedeutet:
1.234.567.890 Hz

es muss die Länge geprüft werden, da das IC735 nur
4 bytes sendet:
12 34 56 78 bedeutet
12.345.678 Hz

die Reihenfolge der Übertragung ist beginnend mit dem LSB,
in obigem Beispiel ist frequ:
90 78 56 34 12

 *
 */

void init_USART_DMA_TX_CIV(int len);
void handle_civrx(uint8_t data);
uint32_t bcdToint32(uint8_t *d, int mode);

#define MAXCIVDATA 30
uint8_t civTXdata[MAXCIVDATA];
uint8_t civRXdata[MAXCIVDATA];
uint32_t civ_freq = 0;
int civ_requestanz = 0;
uint8_t civ_avail = 0;

// ========== UART für das CI/V Interface ===============

// USART-1: PA9=TX, PA10=RX
// TX: DMA-2, Stream-7, Channel-4
// (RX: DMA-2, Stream-5, Channel-4), not used

void init_CIV_uart()
{
	// Enable USART2, DMA2 and GPIO clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	// GPIO alternative function
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	// init UART-1
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 4800;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStruct);

	/*
	USART_Cmd(USART1,ENABLE);
	while(1)
	{
		USART_SendData(USART1,0x55); // OK
	}*/

	// Init DMA
	init_USART_DMA_TX_CIV(MAXWIFIDATA);

	// Init NVIC
	// DMA (TX) Transfer Complete IRQ
	NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE); // DMA_IT_TC ... transfer complete

	// USART-1 IRQ
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1,ENABLE);


	/*while(1)
	{
		civTXdata[0] = 0xfe;
		civTXdata[1] = 0xfe;
		civTXdata[2] = 0;
		civTXdata[3] = 0xe0;
		civTXdata[4] = 4;
		civTXdata[5] = 0xfd;
		civ_send(6);
		delay_1ms(1000);
	}*/
}

void init_USART_DMA_TX_CIV(int len)
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)civTXdata;
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
	DMA_Init(DMA2_Stream7, &DMA_InitStructure);
}

void USART1_IRQHandler(void)
{
	if ((USART1->SR & USART_FLAG_RXNE) != (u16)RESET)
	{
		handle_civrx(USART_ReceiveData(USART1) & 0xff);

		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}

	if ((USART1->SR & USART_FLAG_ORE) != (u16)RESET)
	{
		// Receiver overrun
		// führt beim Debuggen zu ununterbrochenem Auslösen des IRQs
		USART_ClearFlag(USART1, USART_FLAG_ORE);
	}
}

void DMA2_Stream7_IRQHandler()
{
	if(DMA_GetITStatus(DMA2_Stream7,DMA_IT_TCIF7) == SET)
	{
		// mach irgendwas
		DMA_ClearITPendingBit(DMA2_Stream7,DMA_IT_TCIF7);
	}
}

// sende Datensatz via CIV
void civ_send(uint8_t *pdata, int len)
{
	if(DMA_GetITStatus(DMA2_Stream7,DMA_IT_TCIF7) == SET)
		return;	// letztes Senden ist noch nicht fertig

	for(int i=0; i<len; i++)
	{
		civTXdata[i] = pdata[i];
	}

	if(!DMA_GetCmdStatus(DMA2_Stream7))
	{
		init_USART_DMA_TX_CIV(len);
		DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);
		DMA_Cmd(DMA2_Stream7, ENABLE);
		USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	}
}

uint8_t civ_adr = 0;

void civ_request_frequency()
{
uint8_t cmd0_getfreq[6] = {0xfe, 0xfe, 0x00, 0xe0, 0x03, 0xfd};

	// CI-V 1 ist die Default Adresse, da nicht alle TRX die 00 als Broadcast erkennen
	cmd0_getfreq[2] = pcfgdata->civ_adr1;

	if(pcfgdata->auxInputMode == PTT4x)
	{
		if(gpio_stat[ROTS_B2] == 0) cmd0_getfreq[2] = pcfgdata->civ_adr2;
		if(gpio_stat[ROTS_B3] == 0) cmd0_getfreq[2] = pcfgdata->civ_adr3;
		if(gpio_stat[ROTS_B4] == 0) cmd0_getfreq[2] = pcfgdata->civ_adr4;
	}

	civ_send(cmd0_getfreq, 6);
	civ_requestanz++;
}

void handle_civrx(uint8_t data)
{
	// mache vorne Platz
	for(int i=(MAXCIVDATA-1); i>0; i--)
		civRXdata[i] = civRXdata[i-1];

	// neues Byte
	civRXdata[0] = data;

	// der Datensatz steht verkehrt herum in civRXdata
	if(civRXdata[0] == 0xfd)
	{
		// Ende erkannt
		if(civRXdata[8] == 0xe0 && civRXdata[9] == 0xfe && civRXdata[10] == 0xfe)
		{
			// 5 byte Datensatz
			civ_adr = civRXdata[7];
			civ_freq = bcdToint32(civRXdata+1,5);
			civ_requestanz = 0;
		}
		if(civRXdata[7] == 0xe0 && civRXdata[8] == 0xfe && civRXdata[9] == 0xfe)
		{
			// 4 byte Datensatz
			civ_adr = civRXdata[6];
			civ_freq = bcdToint32(civRXdata+1,4);
			civ_requestanz = 0;
		}
	}
}

uint32_t bcdconv(uint8_t v, uint32_t mult)
{
uint32_t tmp,f;

	tmp = (v >> 4) & 0x0f;
	f = tmp * mult * 10;
	tmp = v & 0x0f;
	f += tmp * mult;

	return f;
}

// Wandle ICOM Frequenzangabe um
uint32_t bcdToint32(uint8_t *d, int mode)
{
uint32_t f=0;

	if(mode == 5)
	{
		f += bcdconv(d[0],100000000);
		f += bcdconv(d[1],1000000);
		f += bcdconv(d[2],10000);
		f += bcdconv(d[3],100);
		f += bcdconv(d[4],1);
	}

	if(mode == 4)
	{
		f += bcdconv(d[0],1000000);
		f += bcdconv(d[1],10000);
		f += bcdconv(d[2],100);
		f += bcdconv(d[3],1);
	}
	return f;
}

uint32_t last_civfreq = 0;

uint32_t getCIVfreq()
{
uint32_t freq;

	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	freq = civ_freq;
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	// wenn nach 3 Anforderungen keine Antwort kam, so ignoriere CIV Werte
	if(civ_requestanz > 3)
	{
		civ_freq = 0;
		civ_avail = 0;
	}
	else
	{
		if(freq > 0)
			civ_avail = 1;
	}

	return freq; // in Hz
}
