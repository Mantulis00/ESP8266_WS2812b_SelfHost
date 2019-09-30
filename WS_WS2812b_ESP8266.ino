//#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
//#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>




  

  
char webpage[] PROGMEM = R"=====(
<html>
<head>

  <script>
  var Socket;
  function init()
  {
    Socket = new WebSocket( 'ws://' + window.location.hostname + ':81/');
    Socket.onmessage = function(event)
    {
      document.getElementById("rxConsole").innerHTML = event.data;
    }
  }

  function butF0()
  {
    Socket.send("0");
  }
  function butF1()
  {
    Socket.send("1");
  }
  function butF2()
  {
    Socket.send("2");
  }
  function butF3()
  {
    Socket.send("3");
  }
  function butF4()
  {
    Socket.send("4");
  }
  function butF5()
  {
    Socket.send("5");
  }


  function butF7()
  {
    Socket.send("7");
  }
  function butF8()
  {
    Socket.send("8");
  }
  function butF9()
  {
    Socket.send("9");
  }
  
  </script>
</head>


<body onload = "javascript:init()">
<div>
  <p id ="rxConsole"> _________ </p>
  </div>
  <hr/>
  <div>
 
  <br>
  <button type = "button" id = "but1" onclick="butF1()"> FIRST </button>
  <button type = "button" id = "but2" onclick="butF2()"> SECCOND </button>
  <br>
  <button type = "button" id = "but3" onclick="butF3()"> THIRD </button>
  <button type = "button" id = "but4" onclick="butF4()"> FOURTH </button>
  <br>
  <button type = "button" id = "but5" onclick="butF5()"> FIFTH </button>
  <br>
  <button type = "button" id = "but7" onclick="butF7()"> SEVENTH </button>
  <button type = "button" id = "but8" onclick="butF8()"> EIGHTH </button>
  <br>
  <button type = "button" id = "but9" onclick="butF9()"> ALLON </button>
  <button type = "button" id = "but0" onclick="butF0()"> OFF </button>
  </div>

</body> 
</html>
)=====";


 /* function sendText()
  {
    Socket.send(document.getElementById("txBar").value);
    document.getElementById("txBar").value = "";
  }*/
 //<input type = "text" id = "txBar" onkeydown = "if(event.keyCode == 13) sendText();" />



//////////////////////////////////////////////////////////////////////////////////////////////

  ESP8266WebServer server;
  WebSocketsServer webSocket = WebSocketsServer(81);

  const char *ssid = "88";
  const char *password = "19990808";
  IPAddress local_IP(192,168,4,43);
  IPAddress gateway(192,168,4,9);
  IPAddress subnet(255,255,255,0);
  
  
  String serverMessage;
  
void setupServer()
{
  WiFi.persistent(false);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.mode(WIFI_AP);
  WiFi.enableAP(true);
  WiFi.enableSTA(false);
  WiFi.setSleepMode (WIFI_NONE_SLEEP, 10);
  setupServerActions();
  WiFi.softAP(ssid, password, 8); //WiFi.softAP(ssid, password, channel, hidden, max_connection)
  server.begin();
}


void setupServerActions()
{
  webSocket.onEvent(webSocketEvent);
  webSocket.begin();
 
 // MDNS.begin("1488");

  //////////////
  
   server.on("/", homePage); 

  ////////////////
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
  #define NUM_LEDS 30
  #define DATA_PIN 13

Adafruit_NeoPixel strip(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);

//Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);





  unsigned long times =0 ;
  const int count = 1, prstN = 10;
  boolean state = false;

  String names;

  bool ledDirection = true;
  bool allOFF = true;
  bool prst[prstN];
  int ledNum = 0;
  int lenghtLED = 3;

void setupLED()
{
  strip.setBrightness(255);
  strip.clear();
  strip.show();
  strip.begin();
}



void setupArduino()
{
  Serial.begin(9600);




  setupLED();
}



void setup()
{
 setupArduino();
 setupServer();

}
/////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////// Responses
char c[5];

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t lenght)  /// if websocket port is called
{
  //delay(1);
  //serverMessage = "";
  
 if (type == WStype_TEXT)
 {
  names = "";
  for (int x=0; x<lenght; x++ )
  {
    names+=(char) payload[x];
   // Serial.print((char) payload[x]); 
  }

  allOFF = false;
  clearPresets();
  if (names == "1")
  {
    serverMessage = "1";
    prst[0] = true;

  }
  else if (names == "2")
  {
    serverMessage = "2";
    prst[1] = true;
  }
  else if (names == "3")
  {
    serverMessage = "3";
    prst[2] = true;
    lenghtLED = 5;
  }
  else if (names == "4")
  {
    serverMessage = "4";
    prst[3] = true;
  }
  else if (names == "5")
  {
    serverMessage = "5";
    prst[4] = true;
  }
  else if (names == "6")
  {
    serverMessage = "6";
    prst[5] = true;
  }
  else if (names == "7")
  {
    serverMessage = "7";
    ledNum = 0;
    lenghtLED = 1;
    prst[6] = true;
  }
  else if (names == "8")
  {
    serverMessage = "8";
    prst[7] = true;
  }
  else if (names == "9")
  {
    serverMessage = "9";
    prst[9] = true;
  }
  else
  {
    serverMessage = "OFF";
    ledNum = 14;
    lenghtLED = 3;
    allOFF = true;
  }

 }


 strcpy( c, serverMessage.c_str()); 
 delay(1);
  for (int x=0; x<serverMessage.length(); x++)
  {
    webSocket.broadcastTXT(c, sizeof(c));
  }
 delay(2);
}


void homePage() 
{
  server.send_P(200, "text/html", webpage);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

boolean checkEnd()
{
  if (ledNum >= NUM_LEDS && ledDirection)
  {
    ledDirection = false;
    return  true;
  }
  else if (ledNum<0 && !ledDirection)
  {
    ledDirection = true;
    return  true;
  }
}

void clearPresets()
{
  for (int x=0; x<prstN; x++)
  {
    prst[x] = false;
  }
}

void preset0()
{ 
  checkEnd();
  if (ledDirection)
  {
     strip.setPixelColor(ledNum, 255, 0, 0);
     ledNum++;
  }
  else
  {
    strip.setPixelColor(ledNum, 0, 0, 255);
     ledNum--;
  }
     strip.show();
}

void preset1()
{
   checkEnd();
  if (ledDirection)
  {
     strip.setPixelColor(ledNum, 255, 0, 0);
     
     if (ledNum > 2)
     strip.setPixelColor(ledNum-3, 0, 0, 0);
     
     ledNum++;
  }
  else
  {
    strip.setPixelColor(ledNum, 0, 0, 255);
     
     if (ledNum < NUM_LEDS - 3)
     strip.setPixelColor(ledNum+3, 0, 0, 0);
     
     ledNum--;
  }
     strip.show();
     delay(3);
}

void preset2()
{

  if (ledNum == -1)
  {
    for (int x=1; x<=lenghtLED; x++)
      {
          strip.setPixelColor(ledNum+x, 0, 0, 0);
      }
  }
  
  if (ledNum <=NUM_LEDS/2-1 && ledNum >=0)
  {
    
     strip.setPixelColor(ledNum, 255, 0, 0);

     if (ledNum <= 14-lenghtLED)
     strip.setPixelColor(ledNum+lenghtLED, 0, 0, 0);
     
     ledNum--;
  }
  
  else if (ledNum < 0 && ledNum >= -NUM_LEDS/2)
  {
    strip.setPixelColor(NUM_LEDS/2 -1 -ledNum, 0, 0, 255);
     
     if (-1*ledNum > lenghtLED)
     strip.setPixelColor(NUM_LEDS/2 -1 -ledNum-lenghtLED, 0, 0, 0);
     
     ledNum--;
  }
  else
  {
   for (int x=0; x<=lenghtLED; x++)
      {
          strip.setPixelColor(NUM_LEDS/2 -1 -ledNum-x, 0, 0, 0);
      }

    
  ledNum = 14;
  }
     strip.show();

delay(3);
}

void preset3()
{
  checkEnd();
  if (ledDirection)
  {
    if (NUM_LEDS / 2 > ledNum)
      strip.setPixelColor(ledNum, 255, 0, 0);
     else
     strip.setPixelColor(ledNum, 0, 0, 255);

      ledNum++;
  }
  
  else
  {
    if (NUM_LEDS / 2 > ledNum)
      strip.setPixelColor(ledNum, 0, 0, 255);
     else
     strip.setPixelColor(ledNum, 255, 0, 0);
     
     ledNum--;
  }
     strip.show();
}
void preset9()
{
  for (int x=0; x<NUM_LEDS; x++)
   strip.setPixelColor(x, 255, 255, 255);

   strip.show();
}

void preset4()
{
  if (ledNum >= 30)
  ledNum = 0;

  if (ledNum == 0)
  {
    for (int x=0; x<NUM_LEDS/2; x++)
    {
      strip.setPixelColor(x, 255, 0, 0);
    }
  }
  else if (ledNum == 8)
  {
    for (int x=NUM_LEDS/2; x<NUM_LEDS; x++)
    {
      strip.setPixelColor(x, 0, 0, 255);
    }
  }
  else if (ledNum == 16)
  {
    for (int x=0; x<NUM_LEDS/2; x++)
    {
      strip.setPixelColor(x, 0, 0, 0);
    }
  }
  else if (ledNum == 24)
  {
   for (int x=NUM_LEDS/2; x<NUM_LEDS; x++)
    {
      strip.setPixelColor(x, 0, 0, 0);
    }
  }
  strip.show();
  delay(2);
  ledNum++;
}

void preset6()
{

  if ( NUM_LEDS/2 == ledNum) 
  {
    ledNum = 0;
    lenghtLED++;
  }
  if (lenghtLED == 8)
  {
    ledNum = 0;
    lenghtLED = 0;
    allOFF = true;
  }

  if (lenghtLED % 2 == 1)
  {
     strip.setPixelColor(NUM_LEDS/2 + ledNum, 255, 100, 0);
     strip.setPixelColor(NUM_LEDS/2-1 - ledNum, 255, 100, 0);

     if (ledNum > lenghtLED)
     {
       strip.setPixelColor(NUM_LEDS/2 + ledNum - (lenghtLED + 1), 0, 0, 0);
       strip.setPixelColor(NUM_LEDS/2-1 - ledNum + (lenghtLED + 1), 0, 0, 0);
     }
     delay(15);
  }
  else if (lenghtLED %2 == 0)
  {
      strip.setPixelColor( ledNum, 255, 150, 20);
      strip.setPixelColor(NUM_LEDS -1 - ledNum, 255, 150, 20);

     if (ledNum > lenghtLED)
     {
      strip.setPixelColor( ledNum - (lenghtLED + 1), 0, 0, 0);
      strip.setPixelColor(NUM_LEDS -1 - ledNum + (lenghtLED + 1), 0, 0, 0);
     }
  }


  delay(10);
  strip.show();
  ledNum++;
}



void loop()
{
  if (!WiFi.softAP(ssid, password))
  {
    ESP.restart();
  }
  delay(1);

//WiFi.forceSleepWake(); 
//Serial.print(WiFi.getMode());
//Serial.print(WiFi.getPhyMode());
//Serial.print(WiFi.getSleepMode ());
//Serial.print(WiFi.getListenInterval());   
//Serial.print(WiFi.status());     
//Serial.println(WiFi.softAPgetStationNum()); 

  if (allOFF || WiFi.softAPgetStationNum() <1)
  {
    clearPresets();
    strip.clear();
    strip.show();
  }
   if (prst[0])
    preset0();
   else if (prst[1])
    preset1();
   else if (prst[2])
    preset2();
   else if (prst[3])
    preset3();
   else if (prst[4])
    preset4();

    
    else if (prst[6])
    preset6();
 //   else if (prst[7])
//    preset7();
    
   else if (prst[9])
    preset9();
    
  delay(5);
  webSocket.loop();
  server.handleClient();

  delay(2);
  
 // MDNS.update();

}
