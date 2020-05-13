// =========================================
// evaluate the data from dsp7's responses
// =========================================

// this structure holds all values
WXVALS wxval[MAXVALUES];

char s_acttime[30] = {"---"};
char s_actdate[11] = {""};

unsigned char *prxdata = ws_rxdata;

enum _SYSSTAT_ {
  STATE_POWERUP = 0,
  STATE_STANDBY,
  STATE_ACTIVE,
  STATE_EMERGENCYOFF,
};

enum _ADCVALS {
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

  UREVFLT,

  UANTFWDPEAK,
  UFLTFWDPEAK,
  UDRVFWDPEAK,
  // und die Indizes für die berechneten Werte
  CALC_POWER,
  CALC_EFF,
  MFREQUENCY,
  NUMADCVALS
};

// kopieren aus bandsel.c
#define BANDBUTTONS_ANZ      12
char bandbuttons_text[13][BANDBUTTONS_ANZ][10] =
{
    {
    "160m",
    " 80m",
    " 60m",
    " 40m",
    " 30m",
    " 20m",
    " 17m",
    " 15m",
    " 12m",
    " 10m",
    "  6m",
    "AUTO"
    },
    {
    "160m",
    " 80m",
    " 60m",
    " 40m",
    " 30m",
    " 20m",
    " 17m",
    " 15m",
    " 12m",
    " 10m",
    "  6m",
    "AUTO"
    },
    {
    "160m",
    " 80m",
    " 60m",
    " 40m",
    " 30m",
    " 20m",
    " 17m",
    " 15m",
    " 12m",
    " 10m",
    "  6m",
    "AUTO"
    },
    {
    "160m",
    " 80m",
    " 60m",
    " 40m",
    " 30m",
    " 20m",
    " 17m",
    " 15m",
    " 12m",
    " 10m",
    "  6m",
    "AUTO"
    },
    {
    "160m",
    " 80m",
    " 60m",
    " 40m",
    " 30m",
    " 20m",
    " 17m",
    " 15m",
    " 12m",
    " 10m",
    "  6m",
    "AUTO"
    },
    {
    " 144 MHz",
    " 145 MHz",
    " 145 SAT",
    " 220 MHz",
    " 430 MHz",
    " 435 MHz",
    "70cm SAT",
    " ",
    " ",
    " ",
    " ",
    "    AUTO"
    },
    {
    " 2200m",
    "630m-1",
    "630m-2",
    "  160m",
    "   80m",
    "   60m",
    "   40m",
    " ",
    " ",
    " ",
    " ",
    "  AUTO"
    },
    {
    "630m-1",
    "630m-2",
    "630m-3",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    "  AUTO"
    },
    {
    "6m-1",
    "6m-2",
    "6m-3",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    "  AUTO"
    },
    {
    "4m-1",
    "4m-2",
    "4m-3",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    "  AUTO"
    },
    {
    "2m-1",
    "2m-2",
    "2m-3",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    "  AUTO"
    },
    {
    "70cm-1",
    "70cm-2",
    "70cm-3",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    "  AUTO"
    },
    {
    "23cm-1",
    "23cm-2",
    "23cm-3",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    "  AUTO"
    },

};


typedef struct
{
  long fwd_dBm;      // gemessene Leistung in dBm
  long fwd_dBmpeak;      // gemessene Spitzenleistung in dBm
  long fwd_watt;     // gemessene Leistung in Watt
  long fwd_peakwatt;   // gemessene Spitzenleistung in Watt
  long rev_dBm;      // gemessene Leistung in dBm
  long rev_watt;     // gemessene Leistung in Watt
  long swr;        // das berechnete SWR
  long imp_min;      // minimale Lastimpedanz
  long imp_max;      // maximale Lastimpedanz
  long refl_attenuation; // Reflexionsdämpfung
} t_pwrswr;

unsigned char sysmode; //0=PA, 1=PWRSWR
unsigned char state;
unsigned char antsw;
long band;
unsigned char ptt;
unsigned char fan;
unsigned char tempunits;
unsigned char bandmode=0;
long adc_realvals[NUMADCVALS];
long uptime;
t_pwrswr pwrswr_input;
t_pwrswr pwrswr_filter;
t_pwrswr pwrswr_antenna;

// Werte als Text
char t_vals[MAXVALUES][25];

//char debugtxt[400];

// ====== CRC Funktionen ======

unsigned int reg16 = 0xffff;         // Schieberegister

unsigned int crc16_bytecalc(unsigned char byte)
{
  int i;
  unsigned int polynom = 0x8408;        // Generatorpolynom

  for (i = 0; i < 8; ++i)
  {
    if ((reg16 & 1) != (byte & 1))
      reg16 = (reg16 >> 1)^polynom;
    else
      reg16 >>= 1;
    byte >>= 1;
  }
  return reg16;             // inverses Ergebnis, MSB zuerst
}

unsigned int crc16_messagecalc(unsigned char *data, int len)
{
  int i;

  reg16 = 0xffff;          // Initialisiere Shift-Register mit Startwert
  for (i = 0; i < len; i++) {
    reg16 = crc16_bytecalc(data[i]);   // Berechne fuer jeweils 8 Bit der Nachricht
  }
  return reg16;
}

int checkData(unsigned char *ws_rxdata)
{
  int crc16, crc16rx, len;

  len = ws_rxdata[4];
  len <<= 8;
  len += ws_rxdata[5];

  crc16rx = ws_rxdata[len];
  crc16rx <<= 8;
  crc16rx += ws_rxdata[len + 1];

  crc16 = crc16_messagecalc(ws_rxdata, len);

  //sprintf(debugtxt,"len:%d crcrx:%d crc:%d",len,crc16rx,crc16);
  //snprintf(debugtxt+strlen(debugtxt),len-6,"%s",ws_rxdata+6);

  if (crc16 == crc16rx)
  {

    return len;
  }

  return 0;
}

void get32bit(long *pv)
{
  long v;

  v = *prxdata++;
  v <<= 8;
  v += *prxdata++;
  v <<= 8;
  v += *prxdata++;
  v <<= 8;
  v += *prxdata++;

  *pv = v;
}

void get8bit(unsigned char *pv)
{
  *pv = *prxdata++;
}

void getpwrswr(void *pt)
{
  t_pwrswr *p = (t_pwrswr *)pt;

  get32bit(&(p->fwd_watt));
  get32bit(&(p->fwd_peakwatt));
  get32bit(&(p->fwd_dBm));
  get32bit(&(p->fwd_dBmpeak));
  get32bit(&(p->rev_watt));
  get32bit(&(p->rev_dBm));
  get32bit(&(p->swr));
}

void print32(char *ps, long v)
{
  char sign = ' ';

  if (v < 0) sign = '-';

  v = labs(v);
  sprintf(ps, "%c%d.%1d", sign, v / 10, v - (v / 10) * 10);
}

void printuptime()
{
long v = uptime / 1000;
    
    int seconds = v % 60;
    v -= seconds;
    v /= 60;

    int minutes = v % 60;
    v -= minutes;
    v /= 60;

    int hours = v % 24;
    v -= hours;
    v /= 24;
  
    int days = v;

    sprintf(s_acttime, "%dd %02d:%02d:%02d (h:m:s)", days, hours, minutes, seconds);
}

void evaluate_DSP7data0(unsigned char *ws_rxdata)
{
  int i, idx, len;
  static int f = 1;

  // schreibe Werte des Datensatzes in die entsprechenden Variablen
  len = checkData(ws_rxdata);

  if (len > 0)
  {
    led_dataRXed(0);

    prxdata = ws_rxdata + 6; // ohne Header und Länge

    get8bit(&state);
    get8bit(&antsw);
    get32bit(&band);
    get8bit(&ptt);
    get8bit(&fan);
    get8bit(&tempunits);
    get8bit(&bandmode);
    get32bit(&uptime);

    for (i = 0; i < NUMADCVALS; i++)
    {
      get32bit(&adc_realvals[i]);
    }

    // Umsetzung in Text
    if (f)
    {
      f = 0;
      for (i = 0; i < MAXVALUES; i++)
        *t_vals[i] = 0;
    }

    printuptime();

    switch (state)
    {
      case STATE_POWERUP: sprintf(t_vals[OPSTATE], "POWERUP"); break;
      case STATE_STANDBY: sprintf(t_vals[OPSTATE], "STANDBY"); break;
      case STATE_ACTIVE: sprintf(t_vals[OPSTATE], "OPERATION"); break;
      case STATE_EMERGENCYOFF: sprintf(t_vals[OPSTATE], "EMERG.OFF"); break;
      default: sprintf(t_vals[OPSTATE], "unknown: %d", state); break;
    }

    switch (ptt)
    {
      case 0: sprintf(t_vals[PTT], "RX"); break;
      case 1: sprintf(t_vals[PTT], "TX"); break;
      default: sprintf(t_vals[PTT], "unknown: %d", ptt); break;
    }

    sprintf(t_vals[BAND_SELECTED], "%s", bandbuttons_text[bandmode][band]);

    sprintf(t_vals[ANTENNA_SELECTED], "%d", antsw + 1);

    unsigned long temp = adc_realvals[UTEMP];
    if (temp > 0 && temp < 1590)
      sprintf(t_vals[TEMPERATURE1], "%d.%1d", temp / 10, temp - (temp / 10) * 10);
    else
      sprintf(t_vals[TEMPERATURE1], "?");

    temp = adc_realvals[UTEMP2];
    if (temp > 0 && temp < 1590)
      sprintf(t_vals[TEMPERATURE2], "%d.%1d", temp / 10, temp - (temp / 10) * 10);
    else
      sprintf(t_vals[TEMPERATURE2], "?");

    switch (fan)
    {
      case 0: sprintf(t_vals[FAN], "OFF"); break;
      case 1: sprintf(t_vals[FAN], "ON"); break;
      default: sprintf(t_vals[FAN], "unknown: %d", fan); break;
    }

    print32(t_vals[DCVOLT], adc_realvals[UMESS]);
    print32(t_vals[DCAMP], adc_realvals[IMESS]);
    print32(t_vals[DCPWR], adc_realvals[CALC_POWER]);
    print32(t_vals[EFFICIENCY], adc_realvals[CALC_EFF]);
  }
}

void evaluate_DSP7data1(unsigned char *ws_rxdata)
{
  int i, idx , len;

  // schreibe Werte des Datensatzes in die entsprechenden Variablen
  len = checkData(ws_rxdata);

  if (len > 0)
  {
    led_dataRXed(1);

    prxdata = ws_rxdata + 6; // ohne Header und Länge

    get8bit(&sysmode);

    getpwrswr(&pwrswr_antenna);
    getpwrswr(&pwrswr_filter);
    getpwrswr(&pwrswr_input);

    // Umsetzung in Text
    print32(t_vals[B1_PWR], pwrswr_antenna.fwd_watt);
    print32(t_vals[B1_SWR], pwrswr_antenna.swr);
    print32(t_vals[B2_PWR], pwrswr_filter.fwd_watt);
    print32(t_vals[B2_SWR], pwrswr_filter.swr);
    print32(t_vals[B3_PWR], pwrswr_input.fwd_watt);
    print32(t_vals[B3_SWR], pwrswr_input.swr);
  }
}

unsigned char stm32_config[500];
int stm32_cfglen = 0;

// Konfig vom STM32 kommt
void evaluate_DSP7data2(unsigned char *ws_rxdata)
{
  unsigned char *p = stm32_config;
  int i, len;
  int crc16rx, crc16;


  // schreibe Werte des Datensatzes in die entsprechenden Variablen
  len = checkData(ws_rxdata);

  if (len > 0)
  {
    stm32_cfglen = ws_rxdata[4];
    stm32_cfglen <<= 8;
    stm32_cfglen += ws_rxdata[5];
    stm32_cfglen += 2; // +2 ... inkl. CRC16

    prxdata = ws_rxdata;
    for (i = 0; i < stm32_cfglen; i++)  
    {
      *p++ = *prxdata++;
    }
  }
}

// Wandle die Config in einen Textstring um der in ein HTML Eingabefeld passt
// die Länge ist stm32_cfglen, Inhalt in stm32_config

char convbcd(unsigned char d)
{
  d &= 0x0f;

  if(d <= 9) return d + '0';
  return d + 'A' - 0xa;
}

int getConfig(char *pres)
{
unsigned char *psrc = stm32_config;
int i;

  for(i=0; i<stm32_cfglen; i++)
  {
    *pres++ = convbcd((*psrc) >> 4);
    *pres++ = convbcd(*psrc);
    psrc++;
  }
  *pres = 0;

  return stm32_cfglen * 2;
}

// Sende Config zu DSP7
unsigned char txcfg[300];
int txcfg_len = 0;

unsigned char BCD_to_HEX(char high, char low)
{
unsigned char res = 0;

  if(high >= '0' && high <= '9') res = high - '0';
  if(high >= 'A' && high <= 'F') res = high - 'A' + 0xa;
  res <<= 4;
  
  if(low >= '0' && low <= '9') res |= low - '0';
  if(low >= 'A' && low <= 'F') res |= low - 'A' + 0xa;

  return res;
}

void send_cfg_to_DSP7(String s_config)
{
char *p = const_cast<char*>(s_config.c_str());
int len;

  // ermittle zunächst die Länge  
  len = BCD_to_HEX(p[4*2],p[4*2+1]);
  len <<= 8;
  len |= BCD_to_HEX(p[5*2],p[5*2+1]);
  len += 8; // header, länge und crc16 dazu

  if(len < 100 || len > 300)
  {
    // da stimmt was nicht
    sprintf((char *)txcfg,"len:%d %02X %02X %02X %02X %02X",len, p[4*2],p[4*2+1],p[5*2],p[5*2+1],BCD_to_HEX(p[5*2],p[5*2+1]));
    txcfg_len = strlen((char *)txcfg);
    return;
  }

  for(int i=0; i<len; i++)
  {
    txcfg[i] =  BCD_to_HEX(p[i*2],p[i*2+1]);   
  }

  txcfg_len = len;
}

