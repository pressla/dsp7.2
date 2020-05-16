/*
	Display 7
	Version 1.0
	19.9.2017
	Kurt Moraw
	DJ0ABR
*/

#include <main.h>

int main(void)
{
int res = 1;

	TIM3_Initialization();	// 1ms Tick, delay usw.
	init_LEDs();			// onboard RBG led
	init_GPIOs();
GPIO_WriteBit(GPIOE,LCD_BACKLIGHT_OUT_PE1,1);	// warum braucht man das ??????????
	res = copy_flash_to_ram();
	restore_smallflash_data();
	init_TFT_and_Perif();
	touch_init();
	dsp7_ADC_Init();
	dsp7_ADC3_Init();
	init_wifi_uart();
	init_CIV_uart();
	init_freq_meas();
	init_touchbuttons();
	init_bars();
	//setDefaults(); // einfügen um Speicher auf Defaultwerte zu setzen
	copy_bmp_to_flash();
	/*
	 * manchmal sprint die SD Karte nicht an, geht man mit Pullup (Finger) auf SDO,
	 * so gehts. Programmierter Pullup hilft nicht.
	 * Evt. einen "SD-Starter" einbauen in fatfs_sd.c  Zeile 283-287
	 */

	if(!res)
	{
		// keine korrekten Daten gefunden
	    messagebox(getLang(no_data_set_DEFAULTs),3000);
	    setDefaults();
	    TFT_Fill_Screen(BLACK);
	}

	// Startfenster
	draw_menu_startup();


	while(1)
	{
		mainloop();

	}
}
