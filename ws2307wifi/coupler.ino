#define RODATA_ATTR  __attribute__((section(".irom.text"))) __attribute__((aligned(4)))

extern unsigned char sysmode;
extern String website;
extern const char htmlpages_begin[] PROGMEM;

const char html_bigcss[] RODATA_ATTR = "<style> body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; } a.km_button { background-color: #3333fa; border: none; color: white; padding: 12px 40px; text-align: center; text-decoration: none; display: inline-block; font-size: 18px; font-weight: bold; margin: 8px 2px; cursor: pointer; border-radius: 12px; } a.sb { background-color: #3333fa; color: white; font-size: 10px; font-weight: bold; text-decoration: none; border-radius: 8px; padding: 2px 20px; margin: auto; display: block; } .big { -webkit-box-sizing: content-box; -moz-box-sizing: content-box; box-sizing: content-box; border: none; font: normal 60px/1 \"days-one\", Helvetica, sans-serif; color: #0063ff; text-align: center; -o-text-overflow: ellipsis; text-overflow: ellipsis; text-shadow: 4px 4px 6px rgba(0,0,0,0.5) ; } .bigreg { -webkit-box-sizing: content-box; -moz-box-sizing: content-box; box-sizing: content-box; border: none; font: normal 80px/1 \"days-one\", Helvetica, sans-serif; color: #ff6300; text-align: center; -o-text-overflow: ellipsis; text-overflow: ellipsis; text-shadow: 4px 4px 6px rgba(0,0,0,0.5) ; } </style>";
 
const char html_bigmid[] RODATA_ATTR = "</p><p class=\"big\">SWR<br><p class=\"bigreg\"><a id = 'runtime_swr'></a> : 1</p>";

const char html_bigend[] RODATA_ATTR = "<p class=\"big\">\
<a href=\"/coup1.php\" target=\"_self\" class=\"km_button\">Coupler-1</a><br>\
<a href=\"/coup2.php\" target=\"_self\" class=\"km_button\">Coupler-2</a><br>\
<a href=\"/coup3.php\" target=\"_self\" class=\"km_button\">Coupler-3</a><br>\
<a href=\"/\" target=\"_self\" class=\"km_button\">RETURN</a><br></p></body></html>";

const char html_bigend_german[] RODATA_ATTR = "<p class=\"big\">\
<a href=\"/coup1.php\" target=\"_self\" class=\"km_button\">Koppler-1</a><br>\
<a href=\"/coup2.php\" target=\"_self\" class=\"km_button\">Koppler-2</a><br>\
<a href=\"/coup3.php\" target=\"_self\" class=\"km_button\">Koppler-3</a><br>\
<a href=\"/\" target=\"_self\" class=\"km_button\">RETURN</a><br></p></body></html>";

const char coup_pwrtext[] PROGMEM = "<a id = 'runtime_power'></a> W";

char couptext_de[3][20] =
{
  "Ausgangsleistung",
  "PA <-> Filter",
  "Eingangsleistung"
};

char couptext_en[3][20] =
{
  "Output Power",
  "PA <-> Filter",
  "Input Power"
};

void make_coupler(char coupnum)
{
char text[200+1];

  html_send_progmem(htmlpages_begin);
  html_send_progmem(html_bigcss);

  buildJavascript_coupler(coupnum);

  if(sysmode)
  {
    if(sprache)
      snprintf(text,200,"<body onload='process()'><p class=\"big\">RF-Power %d<br><p class=\"bigreg\">",coupnum);
     else
      snprintf(text,200,"<body onload='process()'><p class=\"big\">HF-Leistung %d<br><p class=\"bigreg\">",coupnum);
  }
  else
  {
    if(sprache)
      snprintf(text,200,"<body onload='process()'><p class=\"big\">%s<br><p class=\"bigreg\">",couptext_en[coupnum-1]);
     else
      snprintf(text,200,"<body onload='process()'><p class=\"big\">%s<br><p class=\"bigreg\">",couptext_de[coupnum-1]);
  }
  html_send_ram(text);
   
  html_send_progmem(coup_pwrtext);
  html_send_progmem(html_bigmid);
  html_send_progmem(sprache?html_bigend:html_bigend_german);
 }

