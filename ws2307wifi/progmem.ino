 /* HTML Codes welche im Progmem gespeichert werden.
 Dazu wird das Textstück hiermit umrandet:
 
 const char textname[] PROGMEM = R"=====(
/// Your entire webpage here. (Can be multi-line)
)=====";
*/

// ============ AJAX XML Texte ===========================

// Beginn des AJAX Scripts

const char XML_ScriptBegin[] PROGMEM = R"=====(
<SCRIPT>
var xmlHttp=createXmlHttpObject();

function createXmlHttpObject(){
 if(window.XMLHttpRequest){
 xmlHttp=new XMLHttpRequest();
 }else{
 xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');
 }
 return xmlHttp;
}

function process(){
 if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
 xmlHttp.open('PUT','xml',true);
 xmlHttp.onreadystatechange=handleServerResponse; // no brackets?????
 xmlHttp.send(null);
 }
 setTimeout('process()',500);
}

function handleServerResponse(){
 if(xmlHttp.readyState==4 && xmlHttp.status==200){
 xmlResponse=xmlHttp.responseXML;
)=====";

// und hiermit wird das Script abgeschlossen

const char XML_ScriptEnd[] PROGMEM = R"=====(
}
}
</SCRIPT></head>
)=====";


// ============ allgemeine Texte für die Webseiten ===========================

const char htmlpages_begin[] PROGMEM = R"=====(
<!doctype html><head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>DSP-7 Web-Interface</title>
)=====";


const char largepage_css_style[] PROGMEM = R"=====(
<style> 
body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; } 
table { background-color: #f4f4f4; } 
table, td, th { border: 1px solid black; -moz-border-radius: 6px; -webkit-border-radius: 6px; border-radius: 6px; } 
th, td { padding: 3px; } 

td:nth-child(3) { white-space: nowrap; } 
tr:nth-child(1) { font-weight: bold; } 
tr:nth-child(2) { background-color: #fff0f0; } 
tr:nth-child(3) { background-color: #fff0f0; } 
tr:nth-child(4) { background-color: #fff0f0; } 
tr:nth-child(5) { background-color: #fff0f0; } 
tr:nth-child(6) { background-color: #fffff0; } 
tr:nth-child(7) { background-color: #fffff0; } 
tr:nth-child(8) { background-color: #fffff0; } 
tr:nth-child(9) { background-color: #f0fff0; } 
tr:nth-child(10) { background-color: #f0fff0; } 
tr:nth-child(11) { background-color: #f0fff0; } 
tr:nth-child(12) { background-color: #f0fff0; } 
tr:nth-child(13) { background-color: #fff0f0; } 
tr:nth-child(14) { background-color: #fff0f0; } 
tr:nth-child(15) { background-color: #fffff0; } 
tr:nth-child(16) { background-color: #fffff0; } 
tr:nth-child(17) { background-color: #f0fff0; } 
tr:nth-child(18) { background-color: #f0fff0; } 
tr:nth-child(19) { background-color: #808080; } 

a.bt { background-color: #3333fa; border: none; color: white; padding: 6px 20px; 
text-align: center; text-decoration: none; display: inline-block; font-size: 14px; width: 150px;
font-weight: bold; margin: 4px 2px; cursor: pointer; border-radius: 12px; } 

a.sb { background-color: #3333fa; color: white; font-size: 10px; font-weight: bold; 
text-decoration: none; border-radius: 8px; padding: 2px 20px; margin: auto; display: block; } 

.greentitle { padding: 8px; margin-bottom: 8px; background-color: #3333fa; 
font-size: 22px; font-weight: bold; border-radius: 12px; color: white; } 
</style>
)=====";


const char html_picture[] PROGMEM = R"=====(
<p><a class="greentitle"><img width="32" height="25" src="data:image/png;base64,iVBORw0KG
goAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAABUSAAAVEgHzhBcVAAAByElEQ
VRYw8WXvUtcQRTFf2dZP1BT2FlFDGqxEvIvpLExNkGCdazS2GsVsBKxTmstBHv/jCSVFqtIwE4tFGPc3WPhW3mZPPfjvbfjg
SneHe7MfefOnHsHIsL2se1a2lbp0bHQfCdUuiy8YNuS0rY5B0jPlxoAYKBpezyw
lYYKcdEKDdWSDtdJjz8zCZyVHoCkmT6C1UumAEkuGkB7gWYyCiFPCurARFmMVHNSeBPmNaR2YNfQ9nyGEDmmDrR6kWXbNdvL
gzgDnTYfBaYSXXjKWhQltH0A3AInAzuEtt8DHzLs7uBznmJhGHgt6brXDWu2G+liZHva/+Od7V/OxqTtN8k4tb1QNAVDGVf
zh6S3wDSwFcxdSqpLqgN/w2paqhRLOgO+6rFBWAO+Ry/H7eZE0p6kT4O4hhfANtBIvsdiS/EFsBmKT962rJrjvk8B34D7xP
RK0lLMajgBfOwz6DZDLaCRZixKQyIJ2+vAPLCRTldZPeEOMNJFO74kivjZ9r6kw7xKOBtKXbsQdRgjyToOZbtaIs1/uvzM
OHAErPQjRFdZe+UM8C6R4tt/GLC9C6w+49cEfgZOv4HFVGMyFONV+7Kv4yKPz254AH43SVQ0DMrvAAAAAElFTkSuQmCC" 
alt=" "/>DSP-7 Controller WiFi</a></p>
)=====";
  
  
const char html_BodyBegin[] PROGMEM = R"=====(
<body onload='process()'>
)=====";


// ============ Texte der großen Hauptseite ===========================

const char html_ltail[] PROGMEM = R"=====(
</table><br>
<p>
<a href="/control.php" target="_self" class="bt">Remote Control</a><br>
<a href="/coup1.php" target="_self" class="bt">Coupler-1</a><br>
<a href="/coup2.php" target="_self" class="bt">Coupler-2</a><br>
<a href="/coup3.php" target="_self" class="bt">Coupler-3</a><br>
<a href="/setup.php" target="_self" class="bt">SETUP</a><br>
<a href="/config.php" target="_self" class="bt">CONFIG</a><br>
</p>
<p>
<font face="Arial" color="#0000FF" size="1">&#169; Version 1.7, DJ0ABR (DARC OV-U02 Bayerwald)
<br><br>Radio Club: <a href="http://www.darc.de/der-club/distrikte/u/ortsverbaende/02/">DARC OV-U02</a>
<br>Author's Homepage: <a href="http://www.dj0abr.de">dj0abr.de</a>
<br>Manufacturing & Sales: <a href="http://www.helitron.de\">helitron.de</a></font>
</p>
</body></html>
)=====";


const char html_ltail_german[] PROGMEM = R"=====(
</table><br>
<p>
<a href="/control.php" target="_self" class="bt">Fernbedienung</a><br>
<a href="/coup1.php" target="_self" class="bt">Koppler-1</a><br>
<a href="/coup2.php" target="_self" class="bt">Koppler-2</a><br>
<a href="/coup3.php" target="_self" class="bt">Koppler-3</a><br>
<a href="/setup.php" target="_self" class="bt">SETUP</a><br>
<a href="/config.php" target="_self" class="bt">Konfiguration</a><br>
</p>
<p>
<font face="Arial" color="#0000FF" size="1">&#169; Version 1.7, DJ0ABR (DARC OV-U02 Bayerwald)
<br><br>Radio Club: <a href="http://www.darc.de/der-club/distrikte/u/ortsverbaende/02/">DARC OV-U02</a>
<br>Homepage des Autors: <a href="http://www.dj0abr.de">dj0abr.de</a>
<br>Herstellung & Vertrieb: <a href="http://www.helitron.de\">helitron.de</a></font>
</p>
</body></html>
)=====";


// Datentabelle: Separator
const char html_lsep[] PROGMEM = "<tr><td colspan=\"4\"> </td></tr>";

// ========================== für Setup =============================

const char wximage[] PROGMEM = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAABUSAAAVEgHzhBcVAAAByElEQVRYw8WXvUtcQRTFf2dZP1BT2FlFDGqxEvIvpLExNkGCdazS2GsVsBKxTmstBHv/jCSVFqtIwE4tFGPc3WPhW3mZPPfjvbfjgSneHe7MfefOnHsHIsL2se1a2lbp0bHQfCdUuiy8YNuS0rY5B0jPlxoAYKBpezywlYYKcdEKDdWSDtdJjz8zCZyVHoCkmT6C1UumAEkuGkB7gWYyCiFPCurARFmMVHNSeBPmNaR2YNfQ9nyGEDmmDrR6kWXbNdvLgzgDnTYfBaYSXXjKWhQltH0A3AInAzuEtt8DHzLs7uBznmJhGHgt6brXDWu2G+liZHva/+Od7V/OxqTtN8k4tb1QNAVDGVfzh6S3wDSwFcxdSqpLqgN/w2paqhRLOgO+6rFBWAO+Ry/H7eZE0p6kT4O4hhfANtBIvsdiS/EFsBmKT962rJrjvk8B34D7xPRK0lLMajgBfOwz6DZDLaCRZixKQyIJ2+vAPLCRTldZPeEOMNJFO74kivjZ9r6kw7xKOBtKXbsQdRgjyToOZbtaIs1/uvzMOHAErPQjRFdZe+UM8C6R4tt/GLC9C6w+49cEfgZOv4HFVGMyFONV+7Kv4yKPz254AH43SVQ0DMrvAAAAAElFTkSuQmCC";

const char setup_tit1[] PROGMEM = "<style>\
body{background-color:#cccccc;font-family:Arial,Helvetica,Sans-Serif;Color:#000088;}\
.greentitle{\
padding:8px;\
margin-bottom:8px;\
background-color:#3333fa;\
font-size:22px;font-weight:bold;\
border-radius:12px;\
color:white;\
}\
.bt{\
background-color:#3333fa;\
border:none;\
color:white;\
padding:6px 20px;\
text-align:center;\
text-decoration:none;\
display:inline-block;\
font-size:14px;font-weight:bold;\
margin:4px 2px;\
cursor:pointer;\
border-radius:12px;\
}\
a.bt{\
background-color:#3333fa;\
border:none;\
color:white;\
padding:6px 20px;\
text-align:center;\
text-decoration:none;\
display:inline-block;\
font-size:14px;font-weight:bold;\
margin:4px 2px;\
cursor:pointer;\
border-radius:12px;\
}\
</style></head><body><p><a class=\"greentitle\"><img width=\"32\" height=\"25\" src=\"";

const char setup_tit2[] PROGMEM = 
"\" alt=\" \"/>\
  DSP-7 SETUP WiFi Interface</a></p>\
<form>\
<b><font color=\"#0000FF\">Passcode:   </font></b>\
<input type=\"text\" name=\"secret\"><br><br>\
<b><font color=\"#0000FF\">WLan Settings</font></b></p>\
<b><font color=\"#FF0000\">Empty fields stay unchanged</font></b></p>\
SSID:<br>\
<input type=\"text\" name=\"ssid\" value=\"";


const char setup_tit2_ger[] PROGMEM = 
"\" alt=\" \"/>\
  DSP-7 SETUP WiFi Interface</a></p>\
<form>\
<b><font color=\"#0000FF\">Zugangskennung:   </font></b>\
<input type=\"text\" name=\"secret\"><br><br>\
<b><font color=\"#0000FF\">WLan Einstellungen</font></b></p>\
<b><font color=\"#FF0000\">Leere Felder bleiben unver&#228;ndert</font></b></p>\
SSID:<br>\
<input type=\"text\" name=\"ssid\" value=\"";


const char setup_tit3[] PROGMEM = "\"><br>\
Passwort:<br>\
<input type=\"text\" name=\"password\" value=\"";

const char setup_tit4[] PROGMEM = "\"><br><br>\
<b><font color=\"#FF0000\">cycle on/off to activate new SSID/Password !</font></b></p><br>";

const char setup_tit4_ger[] PROGMEM = "\"><br><br>\
<b><font color=\"#FF0000\">SSID/Passwort werden erst nach einem RESET &#252;bernommen !</font></b></p><br>";



