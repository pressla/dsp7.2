/*
 * DSP-7 Interface and Webpage: Copyright (c) 2017, Helitron Electronics www.helitron.de
 *  
 * All rights reserved
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "ws2307.h"

int handle_control();

// personal unique access code
char accesscode[50] = {"1234"};

char callsign[15] = "";

// WiFi login data
char ssid[50] = {"MYSSID"};
char password[50] = {""};

char apmode = 0;

// this local WebServer
ESP8266WebServer server(80);

/* ===========================================
 *  SETUP: called once after Reset
 *  ==========================================
 */
void setup ( void ) 
{
unsigned char conncnt = 0;

  // configure the LED outputs
  init_leds();
  
  readEEPROM();
  //test_eeprom();

  // setup the serial UART for communication with DSP-7
  Serial.begin ( 38400 );

  // check for 5 seconds if the jumper is set to activate AP mode
  if(APmodeRequested())
  {
    WiFi.mode(WIFI_AP);
    // start the Ap (192.168.4.1)
    WiFi.softAP("WS23SETUP");
    apmode = 1;
    wifi_printf("interner AP aktiviert. SSID=WS23SETUP, IP=192.168.4.1");
    led_wlan_connected(2);
  }
  else
  {
    // connect this WiFi client to an accesspoint
    // http://www.esp8266.com/viewtopic.php?f=28&t=2430
    if(WiFi.SSID() != ssid)
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        //wifi_station_set_auto_connect(true);
        MDNS.begin("esp8266");
        WiFi.softAPdisconnect(true);
    }
    apmode = 0;
  }

  // setup the WebServer
  // set the functions called if a user requests this webpage or subpage
  server.on ("/", handleRoot );
  server.on ("/setup.php",handle_setupwebpage);
  server.on ("/coup1.php",handleCoup1);
  server.on ("/coup2.php",handleCoup2);
  server.on ("/coup3.php",handleCoup3);
  server.on ("/control.php",handle_control);
  server.on ("/config.php",handle_config);
  server.on ("/xml",handleXML);
  
  // if not existing page is called
  server.onNotFound ( handleNotFound );

  // start the Webserver
  server.begin();
  // HTTP server started

  //testvals(); // !!!!!!!!!!!!1 TEST ONLY
}

/*
 * MAIN LOOP: running forever// check if Jumper is set, if yes, enter AP mode
 */
void loop ( void ) 
{
static char connstat = 0;

  if(apmode)
  {
    server.handleClient();
    led_wlan_connected(2);
    return;
  }

  if(WiFi.status() == WL_CONNECTED)
  {
    led_wlan_connected(1);
    if(!connstat) wifi_printf("connected to %s",ssid);
    connstat = 1;
  }
  else
  {
    led_wlan_connected(0);
    if(connstat) wifi_printf("lost connection from %s",ssid);
    connstat = 0;
  }
  
  // handle client requests to this WebInterface
  server.handleClient();
  
  // if data received, process it immediately
  ws_getdata();

  // send Trigger for dynamic port routes (i.e. for Speedport Router)
  udp_trigger();

  // send to DSP-7
  dsp7_send();
}

/*
 * some routers have a dymamic port routing
 * they trigger to outgoing data on a specific port
 * and then route incoming data (on other ports) to this device
 * 
 * send some dummy data to somewhere, use port 49899 as trigger port
 * do this after reset and every 10 minutes
 */
void udp_trigger()
{
const char *trigger_ip = "8.8.8.8"; // dummy, use any IP
int trigger_port = 49901;  
static long lastsend = 0;

  if((millis()-lastsend)>(10*60*1000))
  //if((millis()-lastsend)>(2000))
  {
    lastsend = millis();

    WiFiUDP udp;

    udp.begin(49899);
    udp.beginPacket(trigger_ip,trigger_port);
    char buf[1] = {0};
    udp.write(buf,1);
    udp.endPacket();
    udp.stop();
  }
}




