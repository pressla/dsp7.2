/*
 * adc.h
 *
 *  Created on: 21.09.2017
 *      Author: kurt
 */

#ifndef ADC_H_
#define ADC_H_

void dsp7_ADC_Init();
void dsp7_ADC2_Init();
void dsp7_ADC3_Init();
int isAdcConvReady();
int isAdc2ConvReady();
int isADC3ConvReady();
int ui16_Read_ADC1_ConvertedValue(int channel);
int ui16_Read_ADC_ConvertedPeakValue(int channel);
int ui16_Read_ADC3_ConvertedValue();
void adc_calculate_values();
void clear_peakvals();
void clear_peakvals2();
int ui16_Read_ADC1_ConvertedValueSmooth(int channel);
int ui16_Read_ADC_ConvertedPeakValue_noHold(int channel);
uint16_t removeu16Peaks(uint16_t buf[], int len);

/*
Zuordnung zu den ADCs:
======================

ADC1: ScanMode: für allgemeine unkritische Messungen
PA0	Uband	ADC123_IN0
PA4	UMESS	ADC12_IN4
PA6	UREVANT	ADC12_IN6
PA7	UTEMP	ADC12_IN7
PB0	IMESS	ADC12_IN8
PB1	UFWDIN	ADC12_IN9
PC0	UFWDFLT	ADC123_IN10
PC2	UREVIN	ADC123_IN12
PC3	UTEMP2	ADC123_IN13
PA5	UFWDANT	ADC12_IN5

ADC3: SingleMode: für Sicherheitsabschaltungen
PC1	UREVFLT	ADC123_IN11
*/

enum _ADCVALS {
	// ADC1
	UBAND = 0,
	UMESS,
	UREVANT,
	UTEMP,
	IMESS,
	UFWDIN,
	UFWDFLT,
	UREVIN,
	UTEMP2,
	UFWDANT,

	// ADC3
	UREVFLT,

	// extra eingefügte Spitzenwerte
	UANTFWDPEAK,
	UFLTFWDPEAK,
	UDRVFWDPEAK,

	// und die Indizes für die berechneten Werte
	CALC_POWER,
	CALC_EFF,
	MFREQUENCY,
	NUMADCVALS
};

extern int adc_rawvals[];
extern float adc_realvals[];


#define ADC_DR_OFFSET	0x4c

/*
 * DMA Streams und Channels sind im uC fest verdrahtet,
 * siehe Tabelle: DMA2 request mapping
 *
 * ADC1 ... Stream0, Channel0
 * ADC2 ... Stream2, Channel1
 * ADC3 ... Stream1, Channel2
 */

// Mittelwertbildung über die letzten x Messungen
#define SMOOTHLEN	20

#endif /* ADC_H_ */
