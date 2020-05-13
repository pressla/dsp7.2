
extern const char XML_ScriptBegin[] PROGMEM;
extern const char XML_ScriptEnd[] PROGMEM;

String XML;
char kopplernummer = 1;
int xml_mode = 0; // 0=large, 1=coupler

const char ajax_response[] PROGMEM = " xmldoc = xmlResponse.getElementsByTagName('";
const char ajax_message[] PROGMEM = "'); message = xmldoc[0].firstChild.nodeValue; document.getElementById('largetab').rows[";
const char ajax_element[] PROGMEM = "].cells[1].innerHTML=message;";

void javascript_send_element(char *response, int row)
{
char text[10];

  html_send_progmem(ajax_response);
  html_send_ram(response);
  html_send_progmem(ajax_message);
  sprintf(text,"%d",row);
  html_send_ram(text);
  html_send_progmem(ajax_element);
}

void buildJavascript() 
{
char text[20];
int rownum = 1;

  xml_mode = 0;
  
  html_send_progmem(XML_ScriptBegin);

  javascript_send_element("response_runtime",0);
  javascript_send_element("response_ip",19);
  javascript_send_element("response_wifi",20);
  javascript_send_element("response_signal",21);

  for(int valnum = 0; valnum < SEPARATOR; valnum++)
  {
    sprintf(text,"response_%d",rownum);
    javascript_send_element(text,rownum);
    
    rownum++;
  }

  html_send_progmem(XML_ScriptEnd);
}

void buildXML()
{
int rownum = 1;

  XML = "<?xml version='1.0' ?>";
  XML += "<xml>";

  if(xml_mode == 0)
  {
    // Werte für das große Display
    
    XML += "<response_runtime>";
    XML += s_actdate;
    XML += " ";
    XML += s_acttime;
    XML += "</response_runtime>";
  
    XML += "<response_ip>";
    XML += wxval[IPADDRESS].sval;
    XML += "</response_ip>";
  
    XML += "<response_wifi>";
    XML += wxval[WIFISTATUS].sval;
    XML += "</response_wifi>";
  
    sprintf(wxval[RSSIVAL].sval, "%ld dBm", WiFi.RSSI());
  
    XML += "<response_signal>";
    XML += wxval[RSSIVAL].sval;
    XML += "</response_signal>";
  
    for(int valnum = 0; valnum < SEPARATOR; valnum++)
    {
      XML += "<response_";
      XML += String(rownum);
      XML += ">";
      XML += t_vals[valnum];
      XML += "</response_";
      XML += String(rownum);
      XML += ">";
      rownum++;
    }
  }

  if(xml_mode == 1)
  {
    // Werte für die Kopplerdisplays

    XML += "<response_power>";
    XML += t_vals[B1_PWR + (kopplernummer - 1)*2];
    XML += "</response_power>";

    XML += "<response_swr>";
    XML += t_vals[B1_SWR + (kopplernummer - 1)*2];
    XML += "</response_swr>";
  }

  XML += "</xml>";
}

const char ajax_resp_pwr[] PROGMEM = " xmldoc = xmlResponse.getElementsByTagName('response_power'); message = xmldoc[0].firstChild.nodeValue; document.getElementById('runtime_power').innerHTML=message;";
const char ajax_resp_swr[] PROGMEM = " xmldoc = xmlResponse.getElementsByTagName('response_swr'); message = xmldoc[0].firstChild.nodeValue; document.getElementById('runtime_swr').innerHTML=message;";

void buildJavascript_coupler(char coupnum)
{
  xml_mode = 1;
  kopplernummer = coupnum; 
  
  html_send_progmem(XML_ScriptBegin);

  html_send_progmem(ajax_resp_pwr);
  html_send_progmem(ajax_resp_swr);

  html_send_progmem(XML_ScriptEnd);
}


