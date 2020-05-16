/*
 * gpio.c
 *
 *  Created on: 26.09.2017
 *      Author: kurt
 */

#include <main.h>

// enthält den Status der Ein- und Ausgänge
// der Status eines Eingangs kann hier direkt ausgelesen werden
// beschreibt man hier einen Ausgang, so wird er automatisch gesetzt
uint8_t gpio_stat[MAXGPIOS];
uint8_t gpio_oldstat[MAXGPIOS];

void init_GPIOs()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	// GPIO A
	GPIO_InitStructure.GPIO_Pin=SW_ONOFF_IN_PA8 | SW_RESET_IN_PA11 | SW_STANDBY_IN_PA12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=ROTS_AUTO_IN_PA15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// GPIO B
	GPIO_InitStructure.GPIO_Pin=ANT_SEL1_OUT_PB6 | BANDREL_1715_OUT_PB7 | BANDREL_1210_OUT_PB9 | BANDREL_6_OUT_PB12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin= ROTS_80M_IN_PB8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// GPIO C
	GPIO_InitStructure.GPIO_Pin= ROTS_6040M_IN_PC4 | ROTS_3020M_IN_PC5 | ROTS_1715M_IN_PC6 | ROTS_1210M_IN_PC7 | ROTS_6M_IN_PC8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin= ROTS_160M_IN_PC13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=BANDREL_80_OUT_PC14 | BANDREL_6040_OUT_PC15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// GPIO D
	GPIO_InitStructure.GPIO_Pin=ICOM_TKEY_IN_PD6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=PSOFF_OUT_PD2 | ANT_SEL2_OUT_PD13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// GPIO E
	GPIO_InitStructure.GPIO_Pin=PTTIN_IN_PE2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=BANDREL_3020_OUT_PE0 | LCD_BACKLIGHT_OUT_PE1 | PSON_OUT_PE3 | FAN_OUT_PE4 | BIAS_OUT_PE5 | PTTOUT_OUT_PE6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	for(int i=0; i<MAXGPIOS; i++)
		gpio_stat[i] = 0;
}

void read_GPIOs()
{
static int oldptt4=255;
int ptt4;
static int f = 1;

	gpio_stat[SW_RESET] = GPIO_ReadInputDataBit(GPIOA, SW_RESET_IN_PA11);
	gpio_stat[SW_STANDBY] = GPIO_ReadInputDataBit(GPIOA, SW_STANDBY_IN_PA12);
	gpio_stat[SW_ONOFF] = GPIO_ReadInputDataBit(GPIOA, SW_ONOFF_IN_PA8);
	gpio_stat[ROTS_B5] = GPIO_ReadInputDataBit(GPIOC, ROTS_1715M_IN_PC6);
	gpio_stat[ROTS_B6] = GPIO_ReadInputDataBit(GPIOC, ROTS_1210M_IN_PC7);
	gpio_stat[ROTS_B7] = GPIO_ReadInputDataBit(GPIOC, ROTS_6M_IN_PC8);
	gpio_stat[ROTS_AUTO] = GPIO_ReadInputDataBit(GPIOA,ROTS_AUTO_IN_PA15);
	gpio_stat[TKEY] = GPIO_ReadInputDataBit(GPIOD, ICOM_TKEY_IN_PD6);
	gpio_stat[PTTIN] = GPIO_ReadInputDataBit(GPIOE, PTTIN_IN_PE2)?0:1;

	// das Board 72 unterstützt keinen Rotary Switch
	#if BOARDVERSION == 72
	pcfgdata->auxInputMode = PTT4x;
	gpio_stat[SW_RESET] = 1;
	gpio_stat[SW_STANDBY] = 1;
	gpio_stat[SW_ONOFF] = 1;
	gpio_stat[ROTS_B5] = 1;
	gpio_stat[ROTS_B6] = 1;
	gpio_stat[ROTS_B7] = 1;
	gpio_stat[ROTS_AUTO] = 1;
	gpio_stat[TKEY] = 1;
	#endif

	if(pcfgdata->auxInputMode == ROTARYSWITCH)
	{
		gpio_stat[ROTS_B1] = GPIO_ReadInputDataBit(GPIOC,ROTS_160M_IN_PC13);
		gpio_stat[ROTS_B2] = GPIO_ReadInputDataBit(GPIOB, ROTS_80M_IN_PB8);
		gpio_stat[ROTS_B3] = GPIO_ReadInputDataBit(GPIOC, ROTS_6040M_IN_PC4);
		gpio_stat[ROTS_B4] = GPIO_ReadInputDataBit(GPIOC, ROTS_3020M_IN_PC5);
		f = 1;
	}

	if(pcfgdata->auxInputMode == PTT4x)
	{
		// simuliere das bistabile Relais
		// wenn ein PTT Eingang auf 0 geht, setze und LASSE ihn auf 0, solange bis ein anderer kommt
		if(GPIO_ReadInputDataBit(GPIOC,ROTS_160M_IN_PC13) == 0)
		{
			gpio_stat[ROTS_B1] = 0;
			gpio_stat[ROTS_B2] = 1;
			gpio_stat[ROTS_B3] = 1;
			gpio_stat[ROTS_B4] = 1;
		}
		else if(GPIO_ReadInputDataBit(GPIOB, ROTS_80M_IN_PB8) == 0)
		{
			gpio_stat[ROTS_B1] = 1;
			gpio_stat[ROTS_B2] = 0;
			gpio_stat[ROTS_B3] = 1;
			gpio_stat[ROTS_B4] = 1;
		}
		else if(GPIO_ReadInputDataBit(GPIOC,ROTS_6040M_IN_PC4) == 0)
		{
			gpio_stat[ROTS_B1] = 1;
			gpio_stat[ROTS_B2] = 1;
			gpio_stat[ROTS_B3] = 0;
			gpio_stat[ROTS_B4] = 1;
		}
		else if(GPIO_ReadInputDataBit(GPIOC,ROTS_3020M_IN_PC5) == 0)
		{
			gpio_stat[ROTS_B1] = 1;
			gpio_stat[ROTS_B2] = 1;
			gpio_stat[ROTS_B3] = 1;
			gpio_stat[ROTS_B4] = 0;
		}
		else
		{
			if(f)
			{
				f = 0;
				gpio_stat[ROTS_B1] = 1;
				gpio_stat[ROTS_B2] = 1;
				gpio_stat[ROTS_B3] = 1;
				gpio_stat[ROTS_B4] = 1;
			}
		}

		// prüfe ob sich was geändert hat
		ptt4 = gpio_stat[ROTS_B1] + (gpio_stat[ROTS_B2] << 1) + (gpio_stat[ROTS_B3] << 2) + (gpio_stat[ROTS_B4] << 3);
		if(ptt4 != oldptt4)
		{
			showstatus = 1;
			oldptt4 = ptt4;
		}
	}
}

void set_GPIO()
{
static int first = 1;

	if(first)
	{
		for(int i=0; i<MAXGPIOS; i++)
			gpio_oldstat[i] = 99;
		first = 0;
	}

	for(int i=ANTSEL1; i<=BANDREL_6; i++)
	{
		if(gpio_stat[i] != gpio_oldstat[i])
		{
			if(i == ANTSEL1) 		GPIO_WriteBit(GPIOB,ANT_SEL1_OUT_PB6,gpio_stat[i]);
			if(i == ANTSEL2) 		GPIO_WriteBit(GPIOD,ANT_SEL2_OUT_PD13,gpio_stat[i]);
			if(i == PWRREL_OFF) 	GPIO_WriteBit(GPIOD,PSOFF_OUT_PD2,gpio_stat[i]);
			if(i == LCD_BACKLIGHT_ONOFF) GPIO_WriteBit(GPIOE,LCD_BACKLIGHT_OUT_PE1,gpio_stat[i]);
			if(i == PWRREL_ON) 		GPIO_WriteBit(GPIOE,PSON_OUT_PE3,gpio_stat[i]);
			if(i == FAN_ONOFF)		GPIO_WriteBit(GPIOE,FAN_OUT_PE4,gpio_stat[i]);
			if(i == BIAS_ONOFF) 	GPIO_WriteBit(GPIOE,BIAS_OUT_PE5,gpio_stat[i]);
			if(i == PTTOUT) 		GPIO_WriteBit(GPIOE,PTTOUT_OUT_PE6,gpio_stat[i]);
			if(i == BANDREL_1) 		GPIO_WriteBit(GPIOC,BANDREL_80_OUT_PC14,gpio_stat[i]);
			if(i == BANDREL_2) 		GPIO_WriteBit(GPIOC,BANDREL_6040_OUT_PC15,gpio_stat[i]);
			if(i == BANDREL_3) 		GPIO_WriteBit(GPIOE,BANDREL_3020_OUT_PE0,gpio_stat[i]);
			if(i == BANDREL_4) 		GPIO_WriteBit(GPIOB,BANDREL_1715_OUT_PB7,gpio_stat[i]);
			if(i == BANDREL_5) 		GPIO_WriteBit(GPIOB,BANDREL_1210_OUT_PB9,gpio_stat[i]);
			if(i == BANDREL_6) 		GPIO_WriteBit(GPIOB,BANDREL_6_OUT_PB12,gpio_stat[i]);

			gpio_oldstat[i] = gpio_stat[i];
		}
	}
}

char rotaryswitch_installed()
{
	#if BOARDVERSION == 72
		return 0;
	#endif

	if(pcfgdata->auxInputMode != ROTARYSWITCH) return 0;

	if(!gpio_stat[ROTS_B1]) return ROTS_B1;
	if(!gpio_stat[ROTS_B2]) return ROTS_B2;
	if(!gpio_stat[ROTS_B3]) return ROTS_B3;
	if(!gpio_stat[ROTS_B4]) return ROTS_B4;
	if(!gpio_stat[ROTS_B5]) return ROTS_B5;
	if(!gpio_stat[ROTS_B6]) return ROTS_B6;
	if(!gpio_stat[ROTS_B7]) return ROTS_B7;
	if(!gpio_stat[ROTS_AUTO])return ROTS_AUTO;

	return 0;
}

void process_extbuttons()
{
static uint8_t laststat[3] = {1,1,1};

	#if BOARDVERSION == 72
		return;
	#endif

	// der ON Taster hat die gleiche Funktion wie der ON-Touchbutton (aber nicht zum Ausschalten)
	if(gpio_stat[SW_ONOFF] != laststat[0])
	{
		// hier das Einschalten als ON Taster
		if(gpio_stat[SW_ONOFF] == 0 && menunum == MENU_STARTUP)
		{
			// Simuliere Touch-Druck
			process_button(TB_EMERG_OFF);
		}
		laststat[0] = gpio_stat[SW_ONOFF];
	}

	// der OFF (Reset) Taster hat die gleiche Funktion wie der Emerg-Off (=ON)-Touchbutton
	if(gpio_stat[SW_RESET] != laststat[2])
	{
		// hier das Ausschalten als OFF Taster
		if(gpio_stat[SW_RESET] == 0 && !(menunum == MENU_STARTUP))
		{
			// Simuliere Touch-Druck
			process_button(TB_EMERG_OFF);
		}
		laststat[2] = gpio_stat[SW_RESET];
	}

	// Standby/Aktiv Umschaltung
	if(gpio_stat[SW_STANDBY] != laststat[1])
	{
		if(gpio_stat[SW_STANDBY] == 0)
		{
			if(state == STATE_STANDBY)
			{
				process_button(TB_ACTIVE);
			}
			else if(state == STATE_ACTIVE)
			{
				process_button(TB_STANDBY);
			}
		}

		laststat[1] = gpio_stat[SW_STANDBY];
	}
}
