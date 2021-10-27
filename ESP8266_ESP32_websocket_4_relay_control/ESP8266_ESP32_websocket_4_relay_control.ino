#if defined(ESP8266)
#include <ESP8266WiFi.h>          
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
#else
#include <WiFi.h>          
#include <WebServer.h>
WebServer server(80); 
#endif
#include <WebSocketsServer.h>
#include <EEPROM.h> 
     String LEDswitch1 ;
     String LEDswitch2 ;
     String LEDswitch3 ;
     String LEDswitch4 ;
const char webSiteCont[] PROGMEM =
R"=====(
<!DOCTYPE html>
<html>
<style>
.green {
  background:linear-gradient(to bottom, #44c767 5%, #5cbf2a 100%);
  background-color:#44c767;
  border-radius:6px;
  border:2px solid #18ab29;
  display:inline-block;
  cursor:pointer;
  color:#ffffff;
  font-family:Arial;
  font-size:15px;
  font-weight:bold;
  padding:6px 24px;
  text-decoration:none;
}
.green:hover {
  background:linear-gradient(to bottom, #5cbf2a 5%, #44c767 100%);
  background-color:#5cbf2a;
}
.green:active {
  position:relative;
  top:1px;
}
.red {
  background:linear-gradient(to bottom, #fe1a00 5%, #ce0100 100%);
  background-color:#fe1a00;
  border-radius:6px;
  border:2px solid #d83526;
  display:inline-block;
  cursor:pointer;
  color:#ffffff;
  font-family:Arial;
  font-size:15px;
  font-weight:bold;
  padding:6px 24px;
  text-decoration:none;
}
.red:hover {
  background:linear-gradient(to bottom, #ce0100 5%, #fe1a00 100%);
  background-color:#ce0100;
}
.red:active {
  position:relative;
  top:1px;
}    
  </style>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0">
  <title>Switcheroo</title>
  <script>
   InitWebSocket()
   function InitWebSocket()
      {
         websock = new WebSocket('ws://'+window.location.hostname+':88/'); 
         websock.onmessage=function(evt)
      {
       JSONobj = JSON.parse(evt.data); 
       document.getElementById('btn1').innerHTML = JSONobj.LEDonoff1;
       document.getElementById('btn2').innerHTML = JSONobj.LEDonoff2;
       document.getElementById('btn3').innerHTML = JSONobj.LEDonoff3;
       document.getElementById('btn4').innerHTML = JSONobj.LEDonoff4;
       if(JSONobj.LEDonoff1 == 'ON1')
          {document.getElementById('btn1').className = "green";}
      else 
          {document.getElementById('btn1').className = "red";}
      if(JSONobj.LEDonoff2 == 'ON2')
          {document.getElementById('btn2').className = "green";}
      else 
          {document.getElementById('btn2').className = "red";}
      if(JSONobj.LEDonoff3 == 'ON3')
          {document.getElementById('btn3').className = "green";}
      else 
          {document.getElementById('btn3').className = "red";}
      if(JSONobj.LEDonoff4 == 'ON4')
          {document.getElementById('btn4').className = "green";}
      else 
          {document.getElementById('btn4').className = "red";}
      }
      
      }
      function button1(){
        btn = 'LEDonoff1=ON1';
        if(document.getElementById('btn1').innerHTML ==='ON1')
        {btn = 'LEDonoff1=OFF1';}
        websock.send(btn);        
      }
       function button2(){
        btn2 = 'LEDonoff2=ON2';
        if(document.getElementById('btn2').innerHTML ==='ON2')
        {btn2 = 'LEDonoff2=OFF2';}
        websock.send(btn2);        
      }
       function button3(){
        btn3 = 'LEDonoff3=ON3';
        if(document.getElementById('btn3').innerHTML ==='ON3')
        {btn3 = 'LEDonoff3=OFF3';}
        websock.send(btn3);        
      }
      
       function button4(){
        btn4 = 'LEDonoff4=ON4';
        if(document.getElementById('btn4').innerHTML ==='ON4')
        {btn4 = 'LEDonoff4=OFF4';}
        websock.send(btn4);        
      }
           
</script>
</head>
<body>
<a  href="#" id="btn1" ONCLICK='button1()' class="green"></a>
</br>
<a  href="#" id="btn2" ONCLICK='button2()' class="green"></a>
</br>
<a  href="#" id="btn3" ONCLICK='button3()' class="green"></a>
</br>
<a  href="#" id="btn4" ONCLICK='button4()' class="green"></a>
</body>
</html>
)=====";

  
#define RELAY 5  //D1
#define BUTTON 4 //D2

int LED2=2;
int LED=13;
int websockMillis=50;
//String webSite;
String JSONtxt;
unsigned long websocketCount=0;
boolean LEDonoff1;
boolean LEDonoff2;
boolean LEDonoff3;
boolean LEDonoff4;
WebSocketsServer webSocket = WebSocketsServer(88);

const char* ssid = "wifi_ssid";
const char* password = "wifi_pass";

// Commands sent through Web Socket

//----------------------------------------------------------------------
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t welength)
{
      String payloadString = (const char *)payload; 
Serial.print("payloadString= "); 
Serial.println(payloadString); 
if(type == WStype_TEXT) // receive text from cliet 
{
 byte separator=payloadString.indexOf('='); 
 String var = payloadString.substring(0,separator); 
 Serial.print("var= "); 
 Serial.println(var); 
 String val = payloadString.substring(separator+1); 
 Serial.print("val ="); Serial.println(val); 
 Serial.println(" "); 
 if(var == "LEDonoff1" && val=="OFF1")  //LEDonoff1 = false;
  {EEPROM.write(1,HIGH);
   EEPROM.commit();}
 if(var == "LEDonoff1" && val=="ON1") //LEDonoff1 = true;
  {EEPROM.write(1,LOW);
   EEPROM.commit();}
 if(var == "LEDonoff2" && val=="OFF2") //LEDonoff2 = false;
 {EEPROM.write(2,HIGH);
  EEPROM.commit();}
 if(var == "LEDonoff2" && val=="ON2")  //LEDonoff2 = true;
  {EEPROM.write(2,LOW);
  EEPROM.commit();}
 if(var == "LEDonoff3" && val=="OFF3") //LEDonoff3 = false;
  {EEPROM.write(3,HIGH);
  EEPROM.commit();}
 if(var == "LEDonoff3" && val=="ON3") //LEDonoff3 = true;
  {EEPROM.write(3,LOW);
  EEPROM.commit();}
 if(var == "LEDonoff4" && val=="OFF4") //LEDonoff4 = false;
  {EEPROM.write(4,HIGH);
  EEPROM.commit();}
 if(var == "LEDonoff4" && val=="ON4") //LEDonoff4 = true;
  {EEPROM.write(4,LOW);
  EEPROM.commit();}

 }
}

void webSite(){
  server.send ( 200, "text/html", webSiteCont );}

//----------------------------------------------------------------------
void setup() {
 EEPROM.begin(18);
 pinMode(LED2, OUTPUT);
 pinMode(LED, OUTPUT);
//  digitalWrite(pin_led,LOW);
 
  Serial.begin(115200);
  Serial.println();
  Serial.println("Serial started at 115200");
  Serial.println();
 
  // Connect to a WiFi network
  Serial.print(F("Connecting to "));  Serial.println(ssid);
WiFi.begin("WIFI_ssid", "wifi_password");
 
  // connection with timeout
 
 
  if (WiFi.status() != WL_CONNECTED)
  { 
     Serial.println("");  Serial.print("Failed to connect to ");  Serial.println(ssid);
     delay(500);
  }
 
  Serial.println("");
  Serial.println(F("[CONNECTED]"));   Serial.print("[IP ");  Serial.println(WiFi.localIP()); 
  
  server.on("/",webSite);
  // start a server
  server.begin();
  Serial.println("Server started");
 
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
  server.handleClient();
   LEDswitch1 = "OFF1";
   LEDswitch2 = "OFF2";
   LEDswitch3 = "OFF3";
   LEDswitch4 = "OFF4";
     if(EEPROM.read(1)==0){
    LEDswitch1 = "ON1";}
  else LEDswitch1 = "OFF1";
  if(EEPROM.read(2)==0){
     LEDswitch2 = "ON2";}
  else LEDswitch2 = "OFF2";
  if(EEPROM.read(3)==0){
      LEDswitch3 = "ON3";}
  else LEDswitch3 = "OFF3";
  if(EEPROM.read(4)==0){
      LEDswitch4 = "ON4";}
  else LEDswitch4 = "OFF4";
   JSONtxt="{\"LEDonoff1\":\""+LEDswitch1+"\",\"LEDonoff2\":\""+LEDswitch2+"\",\"LEDonoff3\":\""+LEDswitch3+"\",\"LEDonoff4\":\""+LEDswitch4+"\"}";
   webSocket.broadcastTXT(JSONtxt);

}
