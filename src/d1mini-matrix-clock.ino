/*
  D1 mini + matrix shield = internet clock
*/
#include <ESP8266WiFi.h>
#include <RemoteDebug.h>
#include <ArduinoOTA.h>
#include <WiFiManager.h>

#include <NTPClient.h>
#include <Time.h>
#include <Timezone.h>
#include <WEMOS_Matrix_LED.h>

#include <ESP8266mDNS.h>       // RemoteDebug dependency
#include <WiFiUdp.h>           // ArduinoOTA and NTPClient dependency
#include <DNSServer.h>         // WiFiManager dependency
#include <ESP8266WebServer.h>  // WiFiManager dependency

#define HOST_NAME "d1mini-clock"

// --- LIBRARIES INIT ---

WiFiManager   wifiManager;
RemoteDebug   debug;
WiFiUDP       ntpUDP;
NTPClient     timeClient(ntpUDP);
MLED          mled(5);

// --- MATRIX LED ---

// 3x3 font
int font[][3] = {
  {B111, B101, B111}, // 0
  {B010, B010, B010}, // 1
  {B110, B010, B011}, // 2
  {B111, B011, B111}, // 3
  {B101, B111, B001}, // 4
  {B011, B010, B110}, // 5
  {B100, B111, B111}, // 6
  {B111, B001, B001}, // 7
  {B011, B111, B111}, // 8
  {B111, B111, B001}  // 9
};

// write 4 digits to the led matrix
void writeMatrix(int a1, int a0, int b1, int b0) {
  for (int i=0; i<3; i++) {
    mled.disBuffer[i+1] = font[a1][i] * 0x10 + font[a0][i];
  }
  for (int i=0; i<3; i++) {
    mled.disBuffer[i+5] = font[b1][i] * 0x10 + font[b0][i];
  }
}

// "logo" : light half of the matrix
void logoMatrix() {
  for (int i=0; i<4; i++) {
    mled.disBuffer[i*2+1] = 0xAA;
  }
}

// --- TIMEZONE DEFINITION ---

TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120}; // Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};   // Central European Time
Timezone CE_TZ(CEST, CET);

// --- SETUP ---

void setup() {

  // print a "logo" to the matrix led
  logoMatrix();
  mled.display();

  // create a hotspot to get wifi credentials
  wifiManager.autoConnect(HOST_NAME);

  // remote debug
  Serial.begin(9600);
  debug.begin(HOST_NAME);
  debug.setResetCmdEnabled(true);
  debug.setSerialEnabled(true);

  // time synchronization
  timeClient.begin();
  timeClient.setUpdateInterval(60*60*1000);

  // over the air update
  ArduinoOTA.setHostname(HOST_NAME);
  ArduinoOTA.begin();
}

// --- MAIN LOOP ---

bool blink = false;

void loop() {

  // check for updates and remote debug
  ArduinoOTA.handle();
  debug.handle();

  // update time via ntp and apply timezone adjustment
  timeClient.update();
  setTime(timeClient.getEpochTime());
  time_t localTime = CE_TZ.toLocal(now());
  int curMin = minute(localTime);
  int curHour = hour(localTime);

  // adjust brightness
  if ( (curHour > 20 ) || (curHour < 6) ) {
    mled.intensity = 1;
  } else {
    mled.intensity = 7;
  }

  // write the time
  writeMatrix(curHour/10, curHour%10, curMin/10, curMin%10);

  // blinking dot
  mled.dot(7, 0, blink);
  blink = !blink;

  // render screen
  mled.display();

  // wait 1s
  delay(1000);
}
