char aprs_debmsg[200] = {0};
char wifi_msg[100] = {0};
char eeprom_msg[100] = {0};
char wx_msg[100] = {0};

void aprs_printf(char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);

  vsnprintf(aprs_debmsg,200,fmt,ap);
  va_end(ap);
}

void wifi_printf(char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);

  vsnprintf(wifi_msg,200,fmt,ap);
  va_end(ap);
}

void eeprom_printf(char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);

  vsnprintf(eeprom_msg,200,fmt,ap);
  va_end(ap);
}

void wx_printf(char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);

  vsnprintf(wx_msg,200,fmt,ap);
  va_end(ap);
}

