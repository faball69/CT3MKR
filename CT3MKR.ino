/*
 * ambience app
 * by Fabrizio Allevi
 * fabrizio . allevi @ tiscali . it
 */

#include "main.h"
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>
#include <Arduino_PMIC.h>

float ambT=0.0f;
bool state=false, reset=false, firstLoop=true;
long tLast=0;


void setup() {
  // RGB led
  WiFiDrv::pinMode(26, OUTPUT); //define red pin
  WiFiDrv::pinMode(25, OUTPUT); //define green pin
  WiFiDrv::pinMode(27, OUTPUT); //define blue pin
  if(serial==1) {
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial) {
      delay(1000); // wait for serial port to connect. Needed for native USB port only
    }
  }
  else
    pinMode(LED_BUILTIN, OUTPUT);
  // disable CHRG led on board (thanks to Michał Blinkiewicz)
  if (PMIC.begin()) {
    println("OK initialize PMIC!");
    PMIC.disableWatchdog();
    PMIC.end();
  }
  else
    println("Failed to initialize PMIC!");

  delay(1000);
  println("setup start!");
  loadData();
  initTemp();
  initWifi();
  WiFiDrv::analogWrite(26, 1);WiFiDrv::analogWrite(25, 0);WiFiDrv::analogWrite(27, 0);
  while(!updateNTP()) {
    delay(500);
  }
  initWeb();
  WiFiDrv::analogWrite(26, 0);WiFiDrv::analogWrite(25, 0);WiFiDrv::analogWrite(27, 1);
}

int commFail=0; // comunication fail count > 100sec reset board

void loop() {
  long tNow=millis();
  if(tNow>tLast+5000 && reset)
    NVIC_SystemReset(); // system reset
  if(!handleWifi()) {
    WiFiDrv::analogWrite(26, 1);WiFiDrv::analogWrite(25, 0);WiFiDrv::analogWrite(27, 0);
    return;
  }
  else if(!firstLoop)
    WiFiDrv::analogWrite(26, 0);WiFiDrv::analogWrite(25, 1);WiFiDrv::analogWrite(27, 0);
  if(tNow>tLast+10000) {
    handleTemp(ambT);
    bool newState=scheduler();
    if(newState!=state || firstLoop) {
      bool bRet=turn(newState);
      if(!bRet) {
        commFail++;
        if(commFail>10)
          reset=true;
      }
      else
        commFail=0;
      if(plant.nCh!=-1 && bRet) {
        state=newState;
        println(state?"Heater is ON":"Heater is OFF");
        if(serial!=1) {
          if(state)
            digitalWrite(LED_BUILTIN, 1);
          else
            digitalWrite(LED_BUILTIN, 0);
        }
        if(firstLoop) {
          if(serial==2)
            serial=0;
          firstLoop=false;
          WiFiDrv::analogWrite(26, 0);WiFiDrv::analogWrite(25, 1);WiFiDrv::analogWrite(27, 0);
        }
      }
    }
    tLast=tNow;
  }
  handleWeb();
}
