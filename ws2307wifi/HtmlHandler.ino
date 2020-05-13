// this string is used to prepare the full HTML page
// (tests with sending in fragments did not work with some firefox versions)

extern String XML;

// ======================================================
// if the client calls an URL not existing on this server
// ======================================================
void handleNotFound() 
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
}

void handleRoot()
{
  html_StartPage();
  make_large();
  html_EndPage();
}

void handleCoup1()
{
  html_StartPage();
  make_coupler(1);
  html_EndPage();
}

void handleCoup2()
{
  html_StartPage();
  make_coupler(2);
  html_EndPage();
}

void handleCoup3()
{
  html_StartPage();
  make_coupler(3);
  html_EndPage();
}

void handleXML()
{
  buildXML();
  server.send(200,"text/xml",XML);
}

