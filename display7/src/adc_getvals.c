/*
 * adc_getvals.c
 *
 *  Created on: 25.09.2017
 *      Author: kurt
 */

#include <main.h>

int adc_rawvals[NUMADCVALS];
float adc_realvals[NUMADCVALS];

void adc_calculate_raw_values()
{
	if(isAdcConvReady())
	{
		for(int i=0; i<=UTEMP2; i++)
		{
			if(i == UFWDIN || i == UFWDFLT)
				adc_rawvals[i] = ui16_Read_ADC_ConvertedPeakValue_noHold(i);
			else if(i == UREVANT)
				adc_rawvals[i] = ui16_Read_ADC1_ConvertedValue(i);
			else
				adc_rawvals[i] = ui16_Read_ADC1_ConvertedValueSmooth(i);
		}
		adc_rawvals[UFLTFWDPEAK] = ui16_Read_ADC_ConvertedPeakValue(UFWDFLT);
		adc_rawvals[UDRVFWDPEAK] = ui16_Read_ADC_ConvertedPeakValue(UFWDIN);

		adc_rawvals[UFWDANT] = ui16_Read_ADC_ConvertedPeakValue_noHold(UFWDANT);
		adc_rawvals[UANTFWDPEAK] = ui16_Read_ADC_ConvertedPeakValue(UFWDANT);
	}

	if(isADC3ConvReady())
		adc_rawvals[UREVFLT] = ui16_Read_ADC3_ConvertedValue();
}

void adc_calculate_real_values()
{
	// Calculate PS Voltage
	// Spannungsteiler 390k : 10k
	adc_realvals[UMESS] = (float)(adc_rawvals[UMESS] *(390+10)) / 10.0;
	adc_realvals[UMESS] /= 1000.0;
	if(adc_realvals[UMESS] < 4.0) adc_realvals[UMESS] = 0;

	// Strommessung
	// beim INA193A ist die Ausgangsspannung = 20 x Shutspannung
	// bei einem Shunt von 1mOhm: Ushunt = Ilast * 0,001
	// Umess = Ushunt * 20
	// Umess = Ilast * 0,001 * 20 = Ilast * 0,02
	// Ilast(=Ianzeige) = Umess[mV] / 20

	// der Rohwert des INA193 kann einen 0-Offset von bis zu xxx (gemessen 49mV, 19mV usw)  haben
	// dieser Offset ist bei richtigen Messungen scheinbar nicht mehr vorhanden
	if(adc_rawvals[IMESS] < 45) adc_rawvals[IMESS] = 0;	// alles unter xxx mV ignorieren (kan bei verschiedenen INAs variieren)
	if(adc_realvals[UMESS] == 0) adc_rawvals[IMESS] = 0;// wenn keine Spannung da, dann zeige auch keinen Strom an

	float R_SHUNT = shuntValue[pcfgdata->shuntR];	// mOhms
	adc_realvals[IMESS] = (float)adc_rawvals[IMESS] / (20.0*R_SHUNT);

	// Temperaturmessung
	adc_realvals[UTEMP] = calc_temp(adc_rawvals[UTEMP]);
	adc_realvals[UTEMP2] = calc_temp(adc_rawvals[UTEMP2]);

	// ICOM Banderkennung
	/*
		IC-7300: (Spannungen nach dem Vorteiler)
		< 2M .... 2,30
		2-4M .... 1,87
		4-8M .... 1,56
		8-11M ... 0
		11-15M .. 1,25
		15-22M .. 0,98
		22-30M .. 0,68
		> 30M ... 0,58
	*/
	if(adc_rawvals[UBAND] < 300) adc_realvals[UBAND] = 30;
	else if(adc_rawvals[UBAND] < 630) adc_realvals[UBAND] = 6;
	else if(adc_rawvals[UBAND] < 830) adc_realvals[UBAND] = 1210;
	else if(adc_rawvals[UBAND] < 1115) adc_realvals[UBAND] = 1715;
	else if(adc_rawvals[UBAND] < 1405) adc_realvals[UBAND] = 20;
	else if(adc_rawvals[UBAND] < 1715) adc_realvals[UBAND] = 6040;
	else if(adc_rawvals[UBAND] < 2085) adc_realvals[UBAND] = 80;
	else adc_realvals[UBAND] = 160;

	calc_PwrBridges();	// die Ergebnisse stehen in dent_pwrswr Strukturen

	// nicht gemessene sondern berechnete Werte
	adc_realvals[CALC_POWER] = adc_realvals[IMESS] * adc_realvals[UMESS];

	if(adc_realvals[CALC_POWER] < 0.01)
		adc_realvals[CALC_EFF] = 0;
	else
		adc_realvals[CALC_EFF] = 100 * pwrswr_antenna.fwd_watt / adc_realvals[CALC_POWER];

	if(adc_realvals[CALC_EFF] > 100.0) adc_realvals[CALC_EFF] = 100.0;

}

void adc_calculate_values()
{
	adc_calculate_raw_values();
	adc_calculate_real_values();
}
