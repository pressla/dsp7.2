extern const char wximage[] PROGMEM;
void makeConfigHTML(String s_secret);

int handle_config()
{
  // read data from url
  if (server.hasArg("secret"))
  {
    String s_secret = server.arg("secret");
    String s_config = server.arg("CONFIG");
    String s_accesscode = accesscode;

    if (s_secret == s_accesscode)
    {
      if(s_config.length() < 100)
        makeConfigHTML(s_secret);
      else
      {
        send_cfg_to_DSP7(s_config);
        handleRoot();
      }
      
      return 1;
    }
    else
    {
      server.send(404, "text/plain", "Zugang verweigert / Access denied");
    }
  }
  else
  {
    makeConfigHTML("");
  }
  return 0;
}


const char config_tit1[] PROGMEM = R"=====(
<!doctype html><head><meta name="viewport" content="width=device-width, initial-scale=1"><title>DSP-7 CONFIG</title><style>
body{background-color:#cccccc;font-family:Arial,Helvetica,Sans-Serif;Color:#000088;}

.greentitle{
padding:8px;
margin-bottom:8px;
background-color:#3333fa;
font-size:22px;
font-weight:bold;
border-radius:12px;
color:white;
}

.bt{
background-color:#3333fa;
border:none;
color:white;
padding:6px 20px;
text-align:center;
text-decoration:none;
display:inline-block;
font-size:16px;
font-weight:bold;
margin:4px 2px;
cursor:pointer;
border-radius:12px;
}

p {
font-family: verdana;
font-weight:bold;
font-size: 24px;
}
}
</style>
<script type="text/javascript">
 
function saveTextAsFile()
{
    var textToSave = document.getElementById("inputTextToSave").value;
    var textToSaveAsBlob = new Blob([textToSave], {type:"text/plain"});
    var textToSaveAsURL = window.URL.createObjectURL(textToSaveAsBlob);
    var fileNameToSaveAs = document.getElementById("inputFileNameToSaveAs").value;
 
    var downloadLink = document.createElement("a");
    downloadLink.download = fileNameToSaveAs;
    downloadLink.innerHTML = "Download File";
    downloadLink.href = textToSaveAsURL;
    downloadLink.onclick = destroyClickedElement;
    downloadLink.style.display = "none";
    document.body.appendChild(downloadLink);
 
    downloadLink.click();
}
 
function destroyClickedElement(event)
{
    document.body.removeChild(event.target);
}
 
function loadFileAsText()
{
    var fileToLoad = document.getElementById("fileToLoad").files[0];
 
    var fileReader = new FileReader();
    fileReader.onload = function(fileLoadedEvent) 
    {
        var textFromFileLoaded = fileLoadedEvent.target.result;
        document.getElementById("outputTextToSave").value = textFromFileLoaded;
    };
    fileReader.readAsText(fileToLoad, "UTF-8");
}
 
</script>
</head>
)=====";

const char download_config_eng[] PROGMEM = R"=====(
<br><b><font color="#0000FF" size="6">SAVE as FILE</font><br>
<br><b><font color="#0000FF">save the DSP-7 configuration as a file<br><br>
)=====";



const char download_config_ger[] PROGMEM = R"=====(
<br><b><font color="#0000FF" size="6">SPEICHERE als DATEI</font><br>
<br><b><font color="#0000FF">Speichere die Konfiguration von DSP-7 als Datei<br><br>
)=====";


const char download_config[] PROGMEM = R"=====(
<br><input type="hidden" id="inputFileNameToSaveAs">
<br><button class="bt" onclick="saveTextAsFile()">SAVE</button><br><br><hr><br>
)=====";

const char upload_config_eng[] PROGMEM = R"=====(
<br><b><font color="#0000FF" size="6">UPLOAD</font><br>
<br><b><font color="#0000FF">UPLOAD the configuration from a file into DSP-7<br>
<br>* Click "BROWSE",
<br>* choose the folder and configuration file
<br>* and click "Open"
<br>* enter your PASSCODE
<br>* Click "UPLOAD" to send it into DSP-7<br><br>
)=====";

const char upload_config_ger[] PROGMEM = R"=====(
<br><b><font color="#0000FF" size="6">UPLOAD</font><br>
<br><b><font color="#0000FF">Lade eine Konfigurationsdatei zum DSP-7 hoch<br>
<br>* Klicke "DURCHSUCHEN",
<br>* waehle ein Verzeichnis und die Konfigurationsdatei
<br>* und Klicke "Oeffnen"
<br>* gebe den korrekten PASSCODE ein
<br>* Klicke "UPLOAD" um sie zu DSP-7 zu senden<br><br>
)=====";


const char upload_config1[] PROGMEM = R"=====(
<input class="bt" type="file" id="fileToLoad" onchange="loadFileAsText()">  <!-- Select Configuration file for Upload -->
<form>
<br><br><b><font color="#0000FF">PASSCODE:
)=====";

const char upload_config2[] PROGMEM = R"=====(
Conf-Data: <input  name="CONFIG" id="outputTextToSave" readonly> <!-- Config for DSP7 -->
<br><br><button class="bt" type="submit" name="Upload">UPLOAD</button>
</form>
<br><hr><br><br><a href="/" target="_self" class="bt">RETURN</a><br>
</body></html>
)=====";


void makeConfigHTML(String s_secret)
{
char text[500+1];
char cfg[500+1];

  html_StartPage();

  html_send_progmem(config_tit1);
  html_send_ram("<body><p><a class=\"greentitle\"><img width=\"32\" height=\"25\" src=\"");
  html_send_progmem(wximage);
  if(!sprache)
    html_send_ram("\" alt=\" \"/>  DSP-7 KONFIGURATION</a></p>");
  else
    html_send_ram("\" alt=\" \"/>  DSP-7 CONFIG</a></p>");
  html_send_progmem(sprache?download_config_eng:download_config_ger);

  int len = getConfig(cfg);
  if(len == 0) strcpy(cfg,"no Cfg Data");
  if(len > 495) strcpy(cfg,"cfg too long");
  snprintf(text,500,"Conf-Data: <input  id=\"inputTextToSave\" value=\"%s\" readonly>",cfg);
  html_send_ram(text);

  html_send_progmem(download_config);

  html_send_progmem(sprache?upload_config_eng:upload_config_ger);
  html_send_progmem(upload_config1);
  snprintf(text,200,"<input type=\"text\" name=\"secret\" value=\"%s\"><br><br>",const_cast<char*>(s_secret.c_str()));
  html_send_ram(text);
  html_send_progmem(upload_config2);

  html_EndPage();
}

