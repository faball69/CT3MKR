/*
 * wifiService service
 * by Fabrizio Allevi
 * fabrizio . allevi @ tiscali . it
 */

#include <WiFi.h>
#include <ArduinoOTA.h>
#include "main.h"
#include "secret.h"

void initWifi() {
  WiFi.disconnect();
  delay(1000);
  print("Connecting to network...");
  WiFi.begin(SECRET_SSID, SECRET_PASS);
  int rty=5;
  while(WiFi.status()!=WL_CONNECTED) {
    print(".");
    delay(1000);
    blink();
    if(--rty==0) {
      WiFi.disconnect();
      delay(10);
      WiFi.begin(SECRET_SSID, SECRET_PASS);
      rty=5;
    }
  }
  stopBlink();
  println("");
  print(WiFi.SSID());
  print(": IPaddr=");
  IPAddress ip=WiFi.localIP();
  print(ip[0]);print(".");print(ip[1]);print(".");print(ip[2]);print(".");println(ip[3]);
  // OTA service
  ArduinoOTA.begin(WiFi.localIP(), OTA_USER, OTA_PSWD, InternalStorage);
}

long tWiFi=0;
bool handleWifi() {
  // if WiFi is down, try reconnecting
  if(WiFi.status()!=WL_CONNECTED) {
    long tNow=millis();
    if(tNow-tWiFi>=30000) {
      WiFi.disconnect();
      WiFi.begin(SECRET_SSID, SECRET_PASS);
      tWiFi=tNow;
    }
    return false;
  }
  else
    ArduinoOTA.poll();
  return true;
}
