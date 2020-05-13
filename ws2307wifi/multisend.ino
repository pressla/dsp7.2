// Funktionen zum Senden einer Webseite in mehreren Fragmenten

void html_StartPage()
{
    server.sendHeader("Cache-Control","no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    server.setContentLength(CONTENT_LENGTH_UNKNOWN); 
    server.send(200, "text/html", "");
}

void html_send_ram(char *data)
{
    server.sendContent(data);
}

void html_send_progmem(PGM_P data)
{
    server.sendContent_P(data);
}

void html_EndPage()
{
    // durch diese zwei Zeilen wird das Ende markiert, da wir
    // am Anfang noch eine CONTENT_LENGTH_UNKNOWN hatten
    
    server.sendContent("");
    server.client().stop();
}


