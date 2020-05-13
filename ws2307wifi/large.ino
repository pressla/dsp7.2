/*
   Original Webseite, formatiert
*/

extern unsigned char sysmode;
extern String javaScript;
extern const char largepage_css_style[] PROGMEM;
extern const char html_ltail[] PROGMEM;
extern const char html_ltail_german[] PROGMEM;
extern const char html_lsep[] PROGMEM;
extern const char html_picture[] PROGMEM;


char *titles_deutsch_pwrswr[MAXVALUES] =
{
  "Betriebsstatus", // OPSTATE = 0,
  "PTT (PA)", // PTT_OUT,
  "Band", // BAND_SELECTED,
  "Antenne", // ANTENNA_SELECTED,
  "Temperatur-1", // TEMPERATURE1,
  "Temperatur-2", // TEMPERATURE2,
  "L&#252;fterstatus", // FAN,
  "DC Spannung", // DCVOLT,
  "DC Strom", // DCAMP,
  "DC Leistung", // DCPWR,
  "Wirkungsgrad", // EFFICIENCY,
  "K-1: Ausgangsleistung", // B1_PWR,
  "K-1: SWR",              // B1_SWR,
  "K-2: Ausgangsleistung", // B2_PWR,
  "K-2: SWR",              // B2_SWR,
  "K-3: Ausgangsleistung", // B3_PWR,
  "K-3: SWR",              // B3_SWR,
  "",              // Separator
  "lokale IP Adresse", // IPADDRESS,
  "WiFi Status", // WIFISTATUS,
  "Signalst&#228;rke" // RSSIVAL,
};

char *titles_deutsch_pa[MAXVALUES] =
{
  "Betriebsstatus", // OPSTATE = 0,
  "PTT (PA)", // PTT_OUT,
  "Band", // BAND_SELECTED,
  "Antenne", // ANTENNA_SELECTED,
  "Temperatur-1", // TEMPERATURE1,
  "Temperatur-2", // TEMPERATURE2,
  "L&#252;fterstatus", // FAN,
  "DC Spannung", // DCVOLT,
  "DC Strom", // DCAMP,
  "DC Leistung", // DCPWR,
  "Wirkungsgrad", // EFFICIENCY,
  "Ausgangsleistung", // B1_PWR,
  "Antennen-SWR",     // B1_SWR,
  "Filter Vorlauf", // B2_PWR,
  "Filter-SWR",              // B2_SWR,
  "Eingangsleistung", // B3_PWR,
  "Eingangs-SWR",              // B3_SWR,
  "",
  "lokale IP Adresse", // IPADDRESS,
  "WiFi Status", // WIFISTATUS,
  "Signalst&#228;rke" // RSSIVAL,
};


char *titles_english_pwrswr[MAXVALUES] =
{
  "Operating State", // OPSTATE = 0,
  "PTT (PA)", // PTT_OUT,
  "Band", // BAND_SELECTED,
  "Antenna", // ANTENNA_SELECTED,
  "Temperature-1", // TEMPERATURE1,
  "Temperature-2", // TEMPERATURE2,
  "Fan state", // FAN,
  "DC Voltage", // DCVOLT,
  "DC Current", // DCAMP,
  "DC Power", // DCPWR,
  "Efficiency", // EFFICIENCY,
  "K-1: Output Power", // B1_PWR,
  "K-1: SWR",              // B1_SWR,
  "K-2: Output Power", // B2_PWR,
  "K-2: SWR",              // B2_SWR,
  "K-3: Output Power", // B3_PWR,
  "K-3: SWR",              // B3_SWR,
  "",
  "local IP Address", // IPADDRESS,
  "WiFi Status", // WIFISTATUS,
  "Signal strength" // RSSIVAL,
};

char *titles_english_pa[MAXVALUES] =
{
  "Operating State", // OPSTATE = 0,
  "PTT (PA)", // PTT_OUT,
  "Band", // BAND_SELECTED,
  "Antenna", // ANTENNA_SELECTED,
  "Temperature-1", // TEMPERATURE1,
  "Temperature-2", // TEMPERATURE2,
  "Fan state", // FAN,
  "DC Voltage", // DCVOLT,
  "DC Current", // DCAMP,
  "DC Power", // DCPWR,
  "Efficiency", // EFFICIENCY,
  "Output Power", // B1_PWR,
  "Antenna SWR",              // B1_SWR,
  "Filter Forward", // B2_PWR,
  "Filter SWR",              // B2_SWR,
  "Input Power", // B3_PWR,
  "Input SWR",              // B3_SWR,
  "",
  "local IP Address", // IPADDRESS,
  "WiFi Status", // WIFISTATUS,
  "Signal strength" // RSSIVAL,
};

char *units[MAXVALUES] =
{
  "", // OPSTATE = 0,
  "", // PTT_OUT,
  "", // BAND_SELECTED,
  "", // ANTENNA_SELECTED,
  "degrees", // TEMPERATURE1,
  "degrees", // TEMPERATURE2,
  "", // FAN,
  "V", // DCVOLT,
  "A", // DCAMP,
  "W", // DCPWR,
  "%", // EFFICIENCY,
  "W",
  ":1",
  "W",
  ":1",
  "W",
  ":1",
  "",
  "",
  "",
  ""
};

// ================================================================================

// Start des Headers und Refreshintervall
void make_largeheader()
{
  html_send_progmem(htmlpages_begin);
  html_send_progmem(largepage_css_style);

  buildJavascript();

  html_send_ram("<body onload='process()'>");
  html_send_progmem(html_picture);
  html_send_ram("<table id=\"largetab\">");
}

void insert_3column_line(char *title, char *c1, char *c2)
{
  char text[100];
  
  snprintf(text,100, "<tr><td>%s</td><td>%s</td><td colspan=\"2\">%s</td></tr>", title, c1, c2);
  html_send_ram(text);
}

void insert_2column_line(char *title, char *c1, char *c2)
{
  char text[100];
  
  snprintf(text,100, "<tr><td>%s</td><td colspan=\"3\">%s%s</td></tr>", title, c1, c2);
  html_send_ram(text);
}


void insert_datatable()
{
  int i;
  char fahrenheit[20] = "&#176; F";
  char celsius[20] = "&#176; C";
  char text[100];

  // Uptime
  snprintf(text,100, "<tr><td>%s</td><td colspan=\"3\">--- ---</td></tr>", (char *)(sprache ? "Uptime" : "Laufzeit"));
  html_send_ram(text);

  // die Tabelleneinträge
  for (i = 0; i < MAXVALUES; i++)
  {
    if (i == SEPARATOR)
      html_send_progmem(html_lsep);
      
    else if (i == TEMPERATURE1 || i == TEMPERATURE2)
      insert_3column_line((sysmode ? (sprache ? titles_english_pwrswr[i] : titles_deutsch_pwrswr[i]) : (sprache ? titles_english_pa[i] : titles_deutsch_pa[i])), "---"/*t_vals[i]*/, tempunits ? fahrenheit : celsius);
      
    else if (i > SEPARATOR)
      insert_2column_line((sysmode ? (sprache ? titles_english_pwrswr[i] : titles_deutsch_pwrswr[i]) : (sprache ? titles_english_pa[i] : titles_deutsch_pa[i])), "---" /*wxval[i].sval*/, " ");
      
    else
      insert_3column_line((sysmode ? (sprache ? titles_english_pwrswr[i] : titles_deutsch_pwrswr[i]) : (sprache ? titles_english_pa[i] : titles_deutsch_pa[i])), "---"/*t_vals[i]*/, units[i]);
  }
}

char *make_large()
{
  strcpy(wxval[IPADDRESS].sval, const_cast<char*>(WiFi.localIP().toString().c_str()));
  sprintf(wxval[WIFISTATUS].sval, "AP: %s", ssid);

  // Header mit Titel, CSS und Bildertitel
  make_largeheader();

  // Datentabelle
  insert_datatable();

  // Tail
  html_send_progmem(sprache ? html_ltail : html_ltail_german);
}

