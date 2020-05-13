/*
 * fan.c
 *
 *  Created on: 07.07.2018
 *      Author: kurt
 */

#include <main.h>

void fan_auto();

int fanhyst = 1;
int fan_switch = 0;
int fan_ontime = 0;
int fan_minruntime = 20000;	// ms
int starttime = 10000;

// Aufruf alle 1ms
void fan_timer()
{
	// Im Falle einer Überhitzung lasse den Fan laufen
	// Nach dem Einschalten für 10s ignorieren solange bis die ADC Daten gültig sind
	if(adc_realvals[UTEMP] > pcfgdata->Temp_limit)
	{
		if(starttime) starttime--;
		else gpio_stat[FAN_ONOFF] = 1;
		return;
	}

	if(fan_ontime) fan_ontime--;

	if(fan_switch == 1)
	{
		// immer sofort einschalten
		gpio_stat[FAN_ONOFF] = 1;
		fan_ontime = fan_minruntime;
	}
	else
	{
		// ausschalten, aber erst wenn die min Laufzeit abgelaufen ist
		if(fan_ontime == 0)
			gpio_stat[FAN_ONOFF] = 0;
	}
}

void do_fan()
{
	if(pcfgdata->fan_temp == 7)
	{
		// Automatik
		fan_auto();
	}
	else if(pcfgdata->fan_temp == 8)
	{
		// Dauer EIN
		fan_switch = 1;
	}
	else if(pcfgdata->fan_temp == 9)
	{
		// EIN bei TX
		if(gpio_stat[PTTOUT] != 0)
			fan_switch = 1;
		else
			fan_switch = 0;
	}
	else
	{
		// nach Temperatur
		// schalte Lüfter ein
		if((adc_realvals[UTEMP] > pcfgdata->fan_temp) || (adc_realvals[UTEMP2] > pcfgdata->fan_temp))
			fan_switch = 1;

		// oder aus
		if((adc_realvals[UTEMP] < (pcfgdata->fan_temp - fanhyst)) && (adc_realvals[UTEMP2] < (pcfgdata->fan_temp - fanhyst)))
			fan_switch = 0;
	}
}

void fan_auto()
{
static int lasttemp = 9999;
int rise_fall=2;
int difftemp = 5;	// in 1/10 Grad

	int temp = adc_realvals[UTEMP] * 10;
	int temp2 = adc_realvals[UTEMP2] * 10;
	if(temp2 > temp) temp = temp2;

	// temp = Temperatur*10, also auf 1/10 Grad genau

	// bei > 45 Grad schalte ein
	if(temp > 450)
	{
		fan_switch = 1;
		lasttemp = temp;
		return;
	}

	// bei < 30 Grad schalte aus
	if(temp < 300)
	{
		fan_switch = 0;
		lasttemp = temp;
		return;
	}

	// Bewegungsrichtung der Temperatur
	if(temp < (lasttemp - difftemp)) rise_fall = 0;
	if(temp > (lasttemp + difftemp)) rise_fall = 1;
	if(rise_fall != 2) lasttemp = temp;

	// wenn die Temperatur ansteigt, schalte ein
	if(rise_fall == 1)
		fan_switch = 1;

	// wenn die Temperatur sinkt, schalte aus
	if(rise_fall == 0) fan_switch = 0;

	// bei unter 35 Grad schalte sofort aus
	if(temp < 350)
		fan_ontime = 0;
}
