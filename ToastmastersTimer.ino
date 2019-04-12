/*
 * Toastmasters Timer LED Matrix Webserver
 *Many thanks to Brian Lough for the setup of the MAtrix:
 *https://www.hackster.io/brian-lough/rgb-led-matrix-with-an-esp8266-a16fa9
 *
 *
 *
 *Connect to ADHOC Network "TM Timer" to input your wifi password and select network ssid.
 *
 * Connect to "http://TMTimer.local" or the IP Address shown on board
 * to bring up an HTML form to control the connected LED Matrix. 
 * 
 *
 * 
*/

#include <StopWatch.h>



#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <ESP8266mDNS.h>
#include <PxMatrix.h>
#include <Fonts/FreeSansBoldOblique9pt7b.h>
#include <Fonts/FreeSansBoldOblique12pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
// Macros to retrieve the fractional seconds and minute parts of a time
// supplied in ms
#define numberOfSeconds(_time_) ((_time_ / 1000) % 60)  
#define numberOfMinutes(_time_) (((_time_ / 1000) / 60) % 60) 

/*Code for Matrix Display
 * This controls the display
 */

#ifdef ESP32

#define P_LAT 22
#define P_A 19
#define P_B 23
#define P_C 18
#define P_D 5
#define P_E 15
#define P_OE 2
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#endif

#ifdef ESP8266

#include <Ticker.h>
Ticker display_ticker;
#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_D 12
#define P_E 0
#define P_OE 2

#endif

// Pins for LED MATRIX
PxMATRIX display(64,32,P_LAT, P_OE,P_A,P_B,P_C,P_D,P_E);
//variable for interrupting fade

#ifdef ESP8266
// ISR for display refresh
void display_updater()
{
  //display.displayTestPattern(70);
  display.display(70);
}
#endif

#ifdef ESP32
void IRAM_ATTR display_updater(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  //isplay.display(70);
  display.displayTestPattern(70);
  portEXIT_CRITICAL_ISR(&timerMux);
}
#endif


void countUp(String green, String yellow, String red) {
StopWatch stopwatch;
  stopwatch.start();
  
  /*
display.fillScreen(display.color565(0, 0,0));
  
  display.setTextSize(1);
  
  //display.setFont(&FreeSansBoldOblique9pt7b);
  display.setFont(&FreeSansBoldOblique12pt7b);
  //display.setFont(&FreeMonoBold12pt7b);
  //display.setCursor(14,31-18); display.print(MESSAGEvalue);

  display.setBrightness(200);
  
  display.setTextColor(display.color565(255, 255, 255));
  display.setCursor(64,-9);
  display.print(String(stopwatch.ms()));
 */
  display.fillScreen(display.color565(0, 0,0));
  
  display.setTextSize(1);
  
  //display.setFont(&FreeSansBoldOblique9pt7b);
  display.setFont(&FreeSansBoldOblique12pt7b);
  //display.setFont(&FreeMonoBold12pt7b);
  // Calculate the time remaining 
  int greenS = green.toInt();
  int yellowS = yellow.toInt();
  int redS = red.toInt();
  
  unsigned long greenMillis = (unsigned long) (greenS *1000);
  unsigned long yellowMillis = (unsigned long) (yellowS *1000);
  unsigned long redMillis = (unsigned long) (yellowS *1000);
  
  display.fillScreen(display.color565(0, 0,0));
   while(stopwatch.ms() < greenMillis) {
   
     Serial.print("LoopWhite");
    // To display the countdown in mm:ss format, separate the parts
   int seconds = numberOfSeconds((unsigned long) stopwatch.ms());
    int minutes = numberOfMinutes((unsigned long) stopwatch.ms()); 
   
Serial.print(String(minutes) + ":" + String(seconds) + "white");
    //display.fillScreen(display.color565(0, 0,0));
  
    delay(150);
  display.setBrightness(200);
  display.clearDisplay();
  
  display.setTextColor(display.color565(255, 255, 255));
  display.setCursor(75,-9);
  if (seconds < 10){
  display.print(String(minutes) + ":0" + String(seconds));}
  else{display.print(String(minutes) + ":" + String(seconds));}
  delay(150);


  
  }

  while((stopwatch.ms() >= greenS*1000) && (yellowS * 1000 > stopwatch.ms()))  {
    
Serial.print("LoopGreen");
    // To display the countdown in mm:ss format, separate the parts
  int seconds = numberOfSeconds((unsigned long)stopwatch.ms());
    int minutes = numberOfMinutes((unsigned long) stopwatch.ms()); 
      delay(150);
  display.setBrightness(200);
  display.clearDisplay();
  
  display.setTextColor(display.color565(0, 255, 0));
  display.setCursor(75,-9);
  if (seconds < 10){
  display.print(String(minutes) + ":0" + String(seconds));}
  else{display.print(String(minutes) + ":" + String(seconds));}
  delay(150);
    
  
  }
  

   while((stopwatch.ms() >= yellowS *1000)  && (redS * 1000 > stopwatch.ms()))  {
   
    Serial.print("LoopYellow");
    // To display the countdown in mm:ss format, separate the parts
    int seconds = numberOfSeconds((unsigned long)stopwatch.ms());
    int minutes = numberOfMinutes((unsigned long)stopwatch.ms());  
      delay(150);
  display.setBrightness(200);
  display.clearDisplay();
  
  display.setTextColor(display.color565(255, 255, 0));
  display.setCursor(75,-9);
  if (seconds < 10){
  display.print(String(minutes) + ":0" + String(seconds));}
  else{display.print(String(minutes) + ":" + String(seconds));}
  delay(150);
 
  
  
  }
 
   while(stopwatch.ms() >= redS *1000)  {
   
  Serial.print("LoopRed");
  
   
    // To display the countdown in mm:ss format, separate the parts
    int seconds = numberOfSeconds((unsigned long)stopwatch.ms());
    int minutes = numberOfMinutes((unsigned long)stopwatch.ms()); 
      delay(150);
  display.setBrightness(200);
  display.clearDisplay();
  display.setTextColor(display.color565(255, 0, 0));
  display.setCursor(75,-9);
  if (seconds < 10){
  display.print(String(minutes) + ":0" + String(seconds));}
  else{display.print(String(minutes) + ":" + String(seconds));}
  delay(150);
  
  
}
  
}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  openWifi();
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster

}


MDNSResponder mdns;

ESP8266WebServer server(80);

const char INDEX_HTML[] =
"<!DOCTYPE HTML>"
"<html>"
"<head>"
"<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">"
"<title>TM Timer!</title>"
"<style>"
//"\"body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }\""
"input[type=text], select {width: 70%;padding: 12px 20px;margin: 8px 0;display: inline-block;border: 1px solid #ccc;border-radius: 4px;box-sizing: border-box;}"
"input[type=text], select {width: 70%;padding: 12px 20px;margin: 8px 0;display: inline-block;border: 1px solid #ccc;border-radius: 4px;box-sizing: border-box;}"

"input[type=submit] {width: 70%;background-color: #4CAF50;color: white;padding: 14px 20px;margin: 8px 0;border: none;border-radius: 4px;cursor: pointer;}"

"input[type=submit]:hover {background-color: #45a049;}"
"div {width: 70%;border-radius: 5px;background-color: #f2f2f2;padding: 20px;}"
"</style>"
"</head>"
"<body>"
"<h1>Toastmasters Timer!</h1>"
"<div>"
"<FORM action=\"/\" method=\"post\">"
"<P>"
"Green Time in seconds:<br>"
"<INPUT type=\"text\" name=\"greenTime\"><BR>"
"Yellow Time in seconds:<br>"
"<INPUT type=\"text\" name=\"yellowTime\"><BR>"
"Red Time in seconds:<br>"
"<INPUT type=\"text\" name=\"redTime\"><BR>"

"<INPUT type=\"submit\" value=\"Send\"><br/>"

"</P>"
"</FORM>"
"</div>"
"</body>"
"</html>";

// GPIO#0 is for Adafruit ESP8266 HUZZAH board. Your board LED might be on 13.
//const int LEDPIN = 0;

void handleRoot()
{
  if (server.hasArg("greenTime") && server.hasArg("yellowTime") && server.hasArg("redTime")) {
    handleSubmit();
  }
  else {
    server.send(200, "text/html", INDEX_HTML);
  }
}

void returnFail(String msg)
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(500, "text/plain", msg + "\r\n");
}

void handleSubmit()
{
  String greenT;
  String yellowT;
  String redT;
  
  if (!(server.hasArg("greenTime") && server.hasArg("yellowTime") && server.hasArg("redTime"))) return returnFail("BAD ARGS");
  greenT = server.arg("greenTime");
  yellowT = server.arg("yellowTime");
  redT = server.arg("redTime");
 countUp(greenT, yellowT, redT);
  server.send(200, "text/html", INDEX_HTML);

  
  
}

void returnOK()
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "OK\r\n");
}


void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void instructions(){
  
  display.setFont(NULL);
  display.fillScreen(display.color565(0, 0,0));
  display.clearDisplay();
  display.setBrightness(200);
  display.setTextColor(display.color565(255, 0, 255));
  display.setCursor(2,0);
  display.print("TM");
  display.setTextColor(display.color565(0, 255, 255));
  display.setCursor(2,8);
  display.print("Timekeeper");
  display.setTextColor(display.color565(255, 255, 0));
  display.setCursor(2,16);
  display.print(WiFi.localIP());
  display.setTextColor(display.color565(255, 0, 255));
  //edit 31.3. (also changed brightness above from 200 to 0)
  //display_update_enable(true);
  /* for loop for brightness
  for (uint8_t brightness=0; brightness<256; brightness++)
  {
    display.setBrightness(brightness);
    delay(10);
  }
*/
  //yield();
  //delay(3000);

  //display_update_enable(false);
  //end edit 31.3.
}
void failedConn(){
  display.setFont(NULL);
  display.fillScreen(display.color565(0, 0,0));
  display.clearDisplay();
  display.setBrightness(200);
  display.setTextColor(display.color565(255, 0, 255));
  display.setCursor(2,0);
  display.print("Connection");
  display.setTextColor(display.color565(0, 255, 255));
  display.setCursor(2,8);
  display.print("failed.");
  display.setTextColor(display.color565(255, 255, 0));
  display.setCursor(2,24);
  display.print("Restarting");
  display.setTextColor(display.color565(255, 0, 255));
}

void openWifi(){
  display.setFont(NULL);
  display.fillScreen(display.color565(0, 0,0));
  display.clearDisplay();
  display.setBrightness(200);
  display.setTextColor(display.color565(255, 0, 255));
  display.setCursor(2,0);
  display.print("Open Wifi,");
  display.setTextColor(display.color565(0, 255, 255));
  display.setCursor(2,8);
  display.print("enter");
  display.setTextColor(display.color565(255, 255, 0));
  display.setCursor(2,16);
  display.print("password");
  display.setTextColor(display.color565(255, 0, 255));
}

void successConn(){
  display.setFont(NULL);
  display.fillScreen(display.color565(0, 0,0));
  display.clearDisplay();
  display.setBrightness(200);
  display.setTextColor(display.color565(255, 0, 255));
  display.setCursor(2,0);
  display.print("Connection");
  display.setTextColor(display.color565(0, 255, 255));
  display.setCursor(2,8);
  display.print("success!");
  display.setTextColor(display.color565(255, 255, 0));
  display.setCursor(2,24);
  display.print("-------");
  display.setTextColor(display.color565(255, 0, 255));
}

//try removing the word void from the function parameters!
void setup(void)
{ // Initialize RGB matrix
  Serial.begin(9600);
  display.begin(16);
  //pinMode(interruptPin, INPUT_PULLUP);

  
  #ifdef ESP8266
    display_ticker.attach(0.002, display_updater);
  #endif

  #ifdef ESP32
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &display_updater, true);
    timerAlarmWrite(timer, 2000, true);
    timerAlarmEnable(timer);
  #endif

  delay(1000);
 
  
  
  
//WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
  //wifiManager.resetSettings();

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("TM Timer")) {
    Serial.println("failed to connect and hit timeout");
    failedConn();
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  successConn();
  
/*setup for server-led script
   * Here the Web server is set up
   *
   */
  //Serial.begin(115200);
  //WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ?");
  //Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("TMTimer", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  //server.on("/ledon", handleLEDon);
  //server.on("/ledoff", handleLEDoff);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.print("Connect to http://tmtimer.local or IP shown on display");
  Serial.println(WiFi.localIP());
  instructions();
}

void loop(void)
{
  
  server.handleClient();
}
