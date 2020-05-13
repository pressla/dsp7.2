#define MAXSETUPSIZE  MAXHTMLLEN

int ax = 0, bx = 0;

char sprache = 1;
unsigned long html_refreshtime = 10;

// send Setup Webpage to client
void handle_setupwebpage()
{
  // read data from url
  if (server.hasArg("secret"))
  {
    String s_secret = server.arg("secret");
    String s_accesscode = accesscode;

    if (s_secret == s_accesscode || apmode == 1)
    {
      if(apmode == 1)
      {
        snprintf(accesscode, 49, const_cast<char*>(s_secret.c_str()));
      }
      
      if (server.hasArg("ssid") && server.hasArg("password"))
      {
        String s_ssid = server.arg("ssid");
        String s_password = server.arg("password");

        if (s_ssid.length() > 0 && s_password.length() > 0)
        {
          snprintf(ssid, 49, const_cast<char*>(s_ssid.c_str()));
          snprintf(password, 49, const_cast<char*>(s_password.c_str()));
        }
      }

      if (server.hasArg("htmlinterval")) {
        String s_html_refreshtime = server.arg("htmlinterval");
        html_refreshtime = s_html_refreshtime.toInt();
      }

      if (server.hasArg("sprache"))
      {
        sprache = 1;
      }
      else
      {
        sprache = 0;
      }

      writeEEPROM();

      makeSetupHTML();
    }
    else
    {
      server.send(404, "text/plain", "Zugang verweigert / Access denied");
    }
  }
  else
  {
    makeSetupHTML();
  }
}

// generate the HTML code of the web page
char *makeSetupHTML()
{
char text[500+1];

  html_StartPage();
  
  html_send_progmem(htmlpages_begin);
  html_send_progmem(setup_tit1);
  html_send_progmem(wximage);
  html_send_progmem(sprache?setup_tit2:setup_tit2_ger);
  html_send_ram(ssid);
  html_send_progmem(setup_tit3);
  html_send_progmem(sprache?setup_tit4:setup_tit4_ger);

snprintf(text,500, 
"<br><input type=\"checkbox\" name=\"sprache\" value=\"1\" %s> Deutsch/English<br>\
<p><input class=\"bt\" type=\"submit\" target=\"_self\" value=\"%s\"/></p>\
<a href=\"/\" target=\"_self\" class=\"bt\">%s</a><br>\
</form>\
</body>\
</html>",

  (sprache?"checked":""),
  (sprache?"SEND":"SENDEN"),
  (sprache?"RETURN":"ZUR&#220;CK")
          );

  html_send_ram(text);

  html_EndPage();
}


