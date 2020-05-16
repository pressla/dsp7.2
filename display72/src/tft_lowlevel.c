/*
 * tft_lowlevel.c
 *
 *  Created on: 13.08.2017
 *      Author: kurt
 */

#include <main.h>

void TFT_CtrlLinesConfig(void);
void TFT_FMCConfig();
void SSD1963_Init1();

// Initialisiert das komplette TFT Display (800x480 Pixel)
void init_TFT_and_Perif()
{
	TFT_CtrlLinesConfig();	// stelle die GPIOs ein
	delay_1ms(10);
	TFT_FMCConfig();		// aktiviere FMC
	SSD1963_Init1();		// initialisiere das Display

	TFT_Fill_Screen(BLACK);
}

// ========== GPIO für das TFT =================

 void TFT_CtrlLinesConfig(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;

		/* Enable GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, ENABLE);

		/*-- GPIO Configuration ------------------------------------------------------*/
		/* SRAM Data lines,  NOE (/RD) and NWE (/WR) configuration */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOD, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FMC);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FMC);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FMC);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FMC);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FMC);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FMC);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FMC);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FMC);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FMC);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_Init(GPIOE, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FMC);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FMC);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FMC);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FMC);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FMC);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FMC);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FMC);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FMC);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FMC);

		/* SRAM Address lines configuration (/RS)*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FMC);

		/* NE3 configuration (/CS)*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_Init(GPIOD, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FMC);

		/*/RESET */
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
}

// ========== FMC =============================

void TFT_FMCConfig(void)
{
	FMC_NORSRAMInitTypeDef  FMC_NORSRAMInitStructure;
	FMC_NORSRAMTimingInitTypeDef  p;

	/* Enable FMC clock */
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE);

	/*-- FMC Configuration ------------------------------------------------------*/
	/*----------------------- SRAM Bank 1 ----------------------------------------*/
	/* FMC_Bank1_NORSRAM1 configuration */
	// langsame Version: flackert leicht, ist immer stabil
	p.FMC_AddressSetupTime = 10;
	p.FMC_AddressHoldTime = 10;
	p.FMC_BusTurnAroundDuration = 20;

	p.FMC_DataSetupTime = 20;
	p.FMC_CLKDivision = 1;
	p.FMC_DataLatency = 0;
	p.FMC_AccessMode = FMC_AccessMode_A;

	/* Color LCD configuration ------------------------------------
	LCD configured as follow:
	- Data/Address MUX = Disable
	- Memory Type = SRAM
	- Data Width = 16bit
	- Write Operation = Enable
	- Extended Mode = Enable
	- Asynchronous Wait = Disable */

	FMC_NORSRAMInitStructure.FMC_Bank = FMC_Bank1_NORSRAM1;
	FMC_NORSRAMInitStructure.FMC_DataAddressMux = FMC_DataAddressMux_Disable;
	FMC_NORSRAMInitStructure.FMC_MemoryType = FMC_MemoryType_SRAM;
	FMC_NORSRAMInitStructure.FMC_MemoryDataWidth = FMC_NORSRAM_MemoryDataWidth_16b;
	FMC_NORSRAMInitStructure.FMC_BurstAccessMode = FMC_BurstAccessMode_Disable;
	FMC_NORSRAMInitStructure.FMC_AsynchronousWait = FMC_AsynchronousWait_Disable;
	FMC_NORSRAMInitStructure.FMC_WaitSignalPolarity = FMC_WaitSignalPolarity_Low;
	FMC_NORSRAMInitStructure.FMC_WrapMode = FMC_WrapMode_Disable;
	FMC_NORSRAMInitStructure.FMC_WaitSignalActive = FMC_WaitSignalActive_BeforeWaitState;
	FMC_NORSRAMInitStructure.FMC_WriteOperation = FMC_WriteOperation_Enable;
	FMC_NORSRAMInitStructure.FMC_WaitSignal = FMC_WaitSignal_Disable;
	FMC_NORSRAMInitStructure.FMC_ExtendedMode = FMC_ExtendedMode_Disable;
	FMC_NORSRAMInitStructure.FMC_WriteBurst = FMC_WriteBurst_Disable;
	FMC_NORSRAMInitStructure.FMC_ReadWriteTimingStruct = &p;
	FMC_NORSRAMInitStructure.FMC_WriteTimingStruct = &p;

	FMC_NORSRAMInit(&FMC_NORSRAMInitStructure);

	/* Enable FMC NOR/SRAM Bank1 */
	FMC_NORSRAMCmd(FMC_Bank1_NORSRAM1, ENABLE);
}

// ================= TFT Initialisierung ===================

#define mHIGH(x) (x >> 8)
#define mLOW(x) (x & 0xFF)

void SSD1963_Init1 (void)
{
	GPIO_ResetBits(GPIOD, GPIO_Pin_12);
	delay_1ms(10);
	GPIO_SetBits(GPIOD, GPIO_Pin_12);
	delay_1ms(100);

		TFT_REG = (0xE2);		//PLL multiplier, set PLL clock to 120M
		TFT_RAM = (0x23);//23);	    //N=0x36 for 6.5M, 0x23 for 10M crystal (mit 23 hat ein Display gegrieselt, während andere gingen)
		TFT_RAM = (0x02);
		TFT_RAM = (0x54);

		TFT_REG = (0xE0);		// PLL enable
		TFT_RAM = (0x01);

		delay_1ms(10);

		TFT_REG = (0xE0);
		TFT_RAM = (0x03);

		delay_1ms(10);

		TFT_REG = (0x01);		// software reset

		delay_1ms(100);

		TFT_REG = (0xE6);		//PLL setting for PCLK, depends on resolution
		TFT_RAM = (0x03);
		TFT_RAM = (0xFF);
		TFT_RAM = (0xFF);

		TFT_REG = (0xB0);		//LCD SPECIFICATION
		TFT_RAM = (0x0c);
		TFT_RAM = (0x00);
		TFT_RAM = (0x03);		//Set HDP	799
		TFT_RAM = (0x1F);
		TFT_RAM = (0x01);		//Set VDP	479
		TFT_RAM = (0xDF);
		TFT_RAM = (0x00);

		TFT_REG = (0xB4);		//HSYNC
		TFT_RAM = (0x03);		//Set HT	928
		TFT_RAM = (0xA0);
		TFT_RAM = (0x00);		//Set HPS	46
		TFT_RAM = (0x2E);
		TFT_RAM = (0x30);		//Set HPW	48
		TFT_RAM = (0x00);		//Set LPS	15
		TFT_RAM = (0x0F);
		TFT_RAM = (0x00);

		TFT_REG = (0xB6);		//VSYNC
		TFT_RAM = (0x02);		//Set VT	525
		TFT_RAM = (0x0D);
		TFT_RAM = (0x00);		//Set VPS	16
		TFT_RAM = (0x10);
		TFT_RAM = (0x10);		//Set VPW	16
		TFT_RAM = (0x00);		//Set FPS	8
		TFT_RAM = (0x08);

		setDisplayamKopf();
		//TFT_REG = (0xBA);
		//TFT_RAM = (0x0F);		//GPIO[3:0] out 1

		TFT_REG = (0xB8);
		TFT_RAM = (0x07);	    //GPIO3=input, GPIO[2:0]=output
		TFT_RAM = (0x01);		//GPIO0 normal

		TFT_REG = (0x36);		//rotation
		TFT_RAM = (0x22);

		TFT_REG = (0xF0);		//pixel data interface
		TFT_RAM = (0x03);


		delay_1ms(1);

		TFT_setRegion(0, 0, 799, 479);
		TFT_REG = (0x29);		//display on

		TFT_REG = (0xBE);		//set PWM for B/L
		TFT_RAM = (0x06);
		TFT_RAM = (0xf0);
		TFT_RAM = (0x01);
		TFT_RAM = (0xf0);
		TFT_RAM = (0x00);
		TFT_RAM = (0x00);

		TFT_REG = (0xd0);
		TFT_RAM = (0x0d);

		TFT_REG = (0x2C);
		return;

	// Set the PLL
		// Crystal (RefClk) = 10 MHz
		// VCO = 300MHz
		// PLLfreq = 10*(35+1)/(2+1) = 120 MHz
		TFT_REG = (0xE2);		// Set the PLL (PLL multiplier, set PLL clock to 120M)
		TFT_RAM = (35);	    	// M, VCO=RefClk * (M+1)
		TFT_RAM = (2);			// N, PLLfrequ = VCO/(N+1)
		TFT_RAM = (0x54);		// Dummy Byte
		delay_1ms(10);

		TFT_REG = (0xE0);		// PLL enable
		TFT_RAM = (0x01);
		delay_1ms(10);

		TFT_REG = (0xE0);		// use PLL as Clock
		TFT_RAM = (0x03);
		delay_1ms(10);

		TFT_REG = (0x01);		// software reset
		delay_1ms(10);

		// Dot Clock Frequency
		// Dot clock Freq = PLL Freq x (LCDC_FPR + 1) / 2^20
		// Dot Clk sollte unter 30 MHz sein (siehe TFT Spec, falls vorhanden, sonst probieren)
		// 120 * (0x3ff6f+1)/2^20 = 30 MHz
		TFT_REG = (0xE6);		// PLL setting for PCLK, depends on resolution
		TFT_RAM = (0x01);		// LCDC_FPR (reduziert damit dünne Linien keinen roten Rand bekommen)
		TFT_RAM = (0xff);
		TFT_RAM = (0x6f);

		TFT_REG = (0xF0);		// pixel data interface
		TFT_RAM = (0x03);		// 565 format

		TFT_REG = (0xB0);		//LCD SPECIFICATION
		TFT_RAM = 0x0C;
		TFT_RAM = 0x00;
		TFT_RAM = mHIGH((TFT_WIDTH-1));
		TFT_RAM = mLOW((TFT_WIDTH-1));
		TFT_RAM = mHIGH((TFT_HEIGHT-1));
		TFT_RAM = mLOW((TFT_HEIGHT-1));
		TFT_RAM = 0b000000;

		TFT_REG = (0xB4);		//HSYNC
		TFT_RAM = (0x03);		//Set HT	928
		TFT_RAM = (0xA0);
		TFT_RAM = (0x00);		//Set HPS	46
		TFT_RAM = (0x2E);
		TFT_RAM = (0x30);		//Set HPW	48
		TFT_RAM = (0x00);		//Set LPS	15
		TFT_RAM = (0x0F);
		TFT_RAM = (0x00);

		TFT_REG = (0xB6);		//VSYNC
		TFT_RAM = (0x02);		//Set VT	525
		TFT_RAM = (0x0D);
		TFT_RAM = (0x00);		//Set VPS	16
		TFT_RAM = (0x10);
		TFT_RAM = (0x10);		//Set VPW	16
		TFT_RAM = (0x00);		//Set FPS	8
		TFT_RAM = (0x08);

		TFT_REG = 0x36;			// Frame Buffer Mode
		TFT_RAM = 0x02;			// links oben ist 0,0

		TFT_REG = (0xB8);
		TFT_RAM = (0x07);	    //GPIO3=input, GPIO[2:0]=output
		TFT_RAM = (0x01);		//GPIO0 normal

		setDisplayamKopf();

		delay_1ms(10);

		TFT_REG = (0x29);		//display on
		delay_1ms(10);
}

void setDisplayamKopf()
{
	TFT_REG = (0xBA);
	if(pcfgdata->display_reverse)
		TFT_RAM = (0x0F);		// Bild umdrehen mit 0 oder 0x0f
	else
		TFT_RAM = (0x00);
}

