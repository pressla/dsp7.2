/*
 * pwrswr.c
 *
 *  Created on: 25.09.2017
 *      Author: kurt
 */

#include <main.h>
#include <math.h>

void calc_psvals(t_pwrswr *vals, float refWlow, float refWhigh, float refmVlow, float refmVhigh, float fwdvolt, float peakfwdvolt, float revvolt);
float swrValueSmooth(float *swrsmoothbuf, float swr);

// Mess- und berechnete Werte
t_pwrswr pwrswr_input;
t_pwrswr pwrswr_filter;
t_pwrswr pwrswr_antenna;

float WtoDBM(float watts)
{
	return 10.0*log10(watts*1000.0);
}

void calc_PwrBridges()
{
	calc_psvals(&pwrswr_antenna,pcfgdata->ant_W_low,pcfgdata->ant_W_high,pcfgdata->ant_mV_low,pcfgdata->ant_mV_high,adc_rawvals[UFWDANT],adc_rawvals[UANTFWDPEAK], adc_rawvals[UREVANT]);
	calc_psvals(&pwrswr_filter,pcfgdata->flt_W_low,pcfgdata->flt_W_high,pcfgdata->flt_mV_low,pcfgdata->flt_mV_high,adc_rawvals[UFWDFLT],adc_rawvals[UFLTFWDPEAK],adc_rawvals[UREVFLT]);
	calc_psvals(&pwrswr_input,pcfgdata->drv_W_low,pcfgdata->drv_W_high,pcfgdata->drv_mV_low,pcfgdata->drv_mV_high,adc_rawvals[UFWDIN],adc_rawvals[UDRVFWDPEAK],adc_rawvals[UREVIN]);
}

 void calc_psvals(t_pwrswr *vals, float refWlow, float refWhigh, float refmVlow, float refmVhigh, float fwdvolt, float peakfwdvolt, float revvolt)
 {
	 float variation,Vf,Vr;
	 float stepmV_per_dB;			// Millivolt pro dB
	 float ref_low_dBm = WtoDBM(refWlow);
	 float ref_high_dBm = WtoDBM(refWhigh);

	 stepmV_per_dB = (refmVhigh - refmVlow)/(ref_high_dBm - ref_low_dBm);

	 // die vom AD8307 ausgegebene Spannung steht in fwdvolt
	 // calculate dBm
	 variation =  fwdvolt - refmVlow;			// difference in volts to xwatts reference
	 variation =  variation/stepmV_per_dB;		// difference in dB to xwatts reference

	 vals->fwd_dBm =  variation + ref_low_dBm;	// real measured dBm value

	 // calculate real power in W
	 vals->fwd_watt = pow(10,vals->fwd_dBm/10.0) / 1000;
	 if(vals->fwd_watt > 9999) vals->fwd_watt = 9999;

	 // berechne noch die Spitzenleistung
	variation =  peakfwdvolt - refmVlow;
	variation =  variation/stepmV_per_dB;

	vals->fwd_dBmpeak =  variation + ref_low_dBm;

	// calculate real power in W
	vals->fwd_peakwatt = pow(10,vals->fwd_dBmpeak/10.0) / 1000;
	if(vals->fwd_peakwatt > 9999) vals->fwd_peakwatt = 9999;

	 // und das gleiche für die Rücklaufende
	 // calculate dBm
	 variation =  revvolt - refmVlow;			// difference in volts to xwatts reference
	 variation =  variation/stepmV_per_dB;		// difference in dB to xwatts reference

	 vals->rev_dBm =  variation + ref_low_dBm;   // real measured dBm value

	 // calculate real reverse power in W
	 vals->rev_watt = pow(10,vals->rev_dBm/10.0) / 1000;

	 // calculate SWR
	 // SWR = (Vf+Vr)/(Vf-Vr)

	 // calculate voltage from dBm value
	 // V = squareroot(P*R)
	 float fswr;
	 Vf = sqrt(vals->fwd_watt*50*1000);
	 Vr = sqrt(vals->rev_watt*50*1000);
	 if(Vf == Vr || Vf < Vr) fswr = 99;
	 else fswr = (Vf+Vr)/(Vf-Vr);

	 if(fswr<1.0001) fswr = 1.0001;
	 if(vals->fwd_watt < 1.0) fswr = 1.0001;

	 vals->swr = swrValueSmooth(vals->smoothbuf, fswr);

	 // und daraus die Impedanzen
	 vals->imp_min = 50.0 / vals->swr;
	 vals->imp_max = 50.0 * vals->swr;

	 if(vals->imp_min < 0) vals->imp_min = 0;
	 if(vals->imp_min > 999) vals->imp_min = 999;
	 if(vals->imp_max < 0) vals->imp_max = 0;
	 if(vals->imp_max > 999) vals->imp_max = 999;

	 vals->refl_attenuation =  vals->fwd_dBm - vals->rev_dBm;
 }

 // Bilde Mittelwert über die letzten SWR Messungen
 float swrValueSmooth(float *swrsmoothbuf, float swr)
 {
 	for(int i=(SWRSMOOTHLEN-1); i>0; i--)
 	{
 		swrsmoothbuf[i] = swrsmoothbuf[i-1];
 	}
 	swrsmoothbuf[0] = swr;

 	float midval = 0;
 	for(int i=0; i<SWRSMOOTHLEN; i++)
 	{
 		midval += swrsmoothbuf[i];
 	}
 	midval /= SWRSMOOTHLEN;

 	return midval;
 }
