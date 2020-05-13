/*
 * color codes:
 * 
 * blue .... WiFi not connected, use AP at 192.168.4.1
 * green .. WiFi connected
 * 
 * blue (short flash) ... data received from WS23xx
 * white ... EEPROM access running
 * 
 * 
 */
void init_leds()
{
  pinMode ( 0, INPUT_PULLUP );
  pinMode ( 4, OUTPUT );
  pinMode ( 5, OUTPUT );
  pinMode ( 12, OUTPUT );
}

char APmodeRequested()
{
  for(int i=0; i<10; i++)
  {
    if(digitalRead(0) == LOW) return 1;

    digitalWrite(LED_BL, LEDON);
    delay(125);
    digitalWrite(LED_BL, LEDOFF);
    delay(125);
    digitalWrite(LED_GN, LEDON);
    delay(125);
    digitalWrite(LED_GN, LEDOFF);
    delay(125);
  }
  return 0;
}

void led_wlan_connected(char onoff)
{
  if(!onoff)
  {
    digitalWrite(LED_GN, LEDOFF);
    digitalWrite(LED_RD, LEDOFF);
    digitalWrite(LED_BL, LEDON);
  }
  else if(onoff == 1)
  {
    digitalWrite(LED_BL, LEDOFF);
    digitalWrite(LED_RD, LEDOFF);
    digitalWrite(LED_GN, LEDON);
  }
  else
  {
    digitalWrite(LED_BL, LEDOFF);
    digitalWrite(LED_GN, LEDOFF);
    digitalWrite(LED_RD, LEDON);
  }
}

void led_dataRXed(char mode)
{
  if(mode == 0)  digitalWrite(LED_BL, LEDON);  
  else if(mode == 1) digitalWrite(LED_RD, LEDON);
  else if(mode == 2)
  {
    digitalWrite(LED_BL, LEDON);  
    digitalWrite(LED_RD, LEDON);
  }
}

void led_access_eeprom(char onoff)
{
  digitalWrite(LED_GN, LEDON);  
  digitalWrite(LED_BL, LEDON);  
}

