/*
 * language.c
 *
 *  Created on: 30.09.2017
 *      Author: kurt
 */

#include <main.h>

#define MAXTEXTANZ LASTLANGENTRY
#define MAXTEXTLEN 50

const char english[MAXTEXTANZ][MAXTEXTLEN] =
{
		"Output Power", // 0
		"W [RF]",
		"Output SWR",
		"Drive Power",
		"W [RFin]",
		"Efficiency", // 5
		"Temperature-1",
		"Deg.",
		"DC Voltage",
		"DC Current",
		"DC Power", // 10
		"POWER/SWR CALIBRATION",
		"no data, set DEFAULTs",
		"save values",
		"RAW ADC Values",
		"Real Values", // 15
		"Touch Top Left Corner",
		"Touch Bottom Right Corner",
		"touch and test position",
		"  EXIT  ",
		"SYSTEM Settings", // 20
		"copying icons from SD card to flash",
		"clear flash   ",
		"finished. Space left",
		"ACTIVE",
		"EMERG.OFF",
		"MENU",
		"IN       Status        OUT",
		"ON switch",
		"Emerg.OFF switch",
		"STANDBY switch",
		"PTT from TRX",
		"switch",
		"Antenna-1",
		"Antenna-2",
		"Power OFF",
		"Power ON",
		"FAN",
		"PTT -> PA",
		"Security",
		"! OFF !",
		"Touch Panel Calibration",
		"Power HISTORY",
		"Bridge-1 Forward Pwr",
		"Bridge-1 Reverse Pwr",
		"Bridge-2 Forward Pwr",
		"Bridge-2 Reverse Pwr",
		"Bridge-3 Forward Pwr",
		"Bridge-3 Reverse Pwr",
		"Security Limits",
		"Antenna Switch",
		"Forward:",
		"Reverse:",
		"ON",
		"Emergency OFF",
		"press ON to activate",
		"Reason of the shutdown:",
		"TEMPERATURE-1 EXCEEDS MAX LIMIT",
		"EMERGENCY OFF BY USER",
		"load current exceeded limit",
		"power supply voltage exceeded limit",
		"drive power from TRX exceeded limit",
		"TX time exceeds limit",
		"Temperature-2",
		"TEMPERATURE-2 EXCEEDS MAX LIMIT",
		"wrong value, please correct",
		"Brdg-1",
		"Brdg-2",
		"Brdg-3",
		"3-Channel",
		"Power/SWR Meter",
		"measure",
		"1. transmit",
		"2. press measure",
		"enter calibration",
		"power levels",
		"Peak",
		"Filter SWR too high",
		"Antenna SWR too high",
		"Antenna",
		"Filter bank",
		"New Config Installed",
};

const char deutsch[MAXTEXTANZ][MAXTEXTLEN] =
{
		"Ausgangsleistung",
		"W [HF]",
		"Ausgangs SWR",
		"Eingangsleistung",
		"W [HFein]",
		"Wirkungsgrad",
		"Temperatur-1",
		"Grad",
		"DC Spannung",
		"DC Strom",
		"DC Leistung",
		"Leistung/SWR KALIBRATION",
		"keine Daten, Grundeinstellungen",
		"Einstellungen sichern",
		"Rohdaten",
		"Ergebnisse",
		"Druecke linke obere Ecke",
		"Druecke rechte untere Ecke",
		"Teste Position durch Druecken",
		"  BEENDEN  ",
		"Systemeinstellungen",
		"Kopiere Bilder von SD ins Flash",
		"loesche Flash",
		"Fertig. Restspeicher",
		"AKTIV",
		"NOT AUS",
		"MENUE",
		"Eingang    Status    Ausgang",
		"EIN Schalter",
		"Notaus Schalter",
		"STANDBY Schalter",
		"PTT vom TRX",
		"Schalter",
		"Antenne-1",
		"Antenne-2",
		"DC-UB AUS",
		"DC-UB EIN",
		"Luefter",
		"PTT -> PA",
		"Sicherheit",
		"! AUS !",
		"Touch Panel Kalibrierung",
		"Leistungsdiagramm",
		"Bruecke-1 Vorwaerts",
		"Bruecke-1 Ruecklauf",
		"Bruecke-2 Vorwaerts",
		"Bruecke-2 Ruecklauf",
		"Bruecke-3 Vorwaerts",
		"Bruecke-3 Ruecklauf",
		"Grenzwerte",
		"Antennenschalter",
		"Vorlauf:",
		"Ruecklauf:",
		"EIN",
		"NOT-Abschaltung",
		"zum Aktivieren druecke EIN",
		"Ursache der Notabschaltung:",
		"TEMPERATUR-1 ZU HOCH",
		"NOTABSCHALTUNG DURCH BENUTZER",
		"Stromaufnahme zu hoch",
		"Netzteilspannung zu hoch",
		"Steuerleistung vom TRX zu hoch",
		"Sendezeit zu lang",
		"Temperatur-2",
		"TEMPERATUR-2 ZU HOCH",
		"ungueltiger Wert, bitte korrigieren",
		"Kopl-1",
		"Kopl-2",
		"Kopl-3",
		"3-Kanal",
		"Leistungs/SWR Meter",
		"messen",
		"1.Senden",
		"2.messen druecken",
		"gebe Leistungen zur",
		"Kalibrierung ein",
		"Spitze",
		"Filter SWR zu hoch",
		"Antennen SWR zu hoch",
		"Antenne",
		"Filterbank",
		"Neue Konfig. Installiert",
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
char *getLang(int textnr)
{
	if(textnr >= LASTLANGENTRY) return "???";

	if(pcfgdata->language == 0)
		return english[textnr];

	return deutsch[textnr];
}
#pragma GCC diagnostic pop

char addtext[50];
char *getLangadd(int textnr, char *addtxt)
{
	sprintf(addtext,"%s%s",getLang(textnr),addtxt);
	return addtext;
}
