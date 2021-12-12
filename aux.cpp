/*
 * aux file
 * by Fabrizio Allevi
 * fabrizio . allevi @ tiscali . it
 */

#include "main.h"

bool toggle=false;
void blink() {
  if(serial!=1) {
    if(toggle)
      digitalWrite(LED_BUILTIN, 1);
    else
      digitalWrite(LED_BUILTIN, 0);
    toggle=!toggle;
  }
}
void stopBlink() {
  if(serial!=1)
    digitalWrite(LED_BUILTIN, 0);
}

String debugMsg="";
int serial=2;  // 0=disable 1=serial 2=buffer
// wrap for debug text
void print(String m) {
  if(serial==1)
    Serial.print(m);
  else if(serial==2)
    debugMsg+=m;
}
void print(const char *m) {
  if(serial==1)
    Serial.print(m);
  else if(serial==2)
    debugMsg+=String(m);
}
void print(char *m) {
  if(serial==1)
    Serial.print(m);
  else if(serial==2)
    debugMsg+=String(m);
}
void print(int m) {
  if(serial==1)
    Serial.print(m);
  else if(serial==2)
    debugMsg+=String(m);
}
void print(uint8_t m) {
  if(serial==1)
    Serial.print(m);
  else if(serial==2)
    debugMsg+=String(m);
}
void print(uint16_t m) {
  if(serial==1)
    Serial.print(m);
  else if(serial==2)
    debugMsg+=String(m);
}
void print(uint32_t m) {
  if(serial==1)
    Serial.print(m);
  else if(serial==2)
    debugMsg+=String(m);
}
void print(double m) {
  if(serial==1)
    Serial.print(m);
  else if(serial==2)
    debugMsg+=String(m);
}
void print(float m) {
  if(serial==1)
    Serial.print(m);
  else if(serial==2)
    debugMsg+=String(m);
}
// Print with New Line
void println(String m) {
  if(serial==1)
    Serial.println(m);
  else if(serial==2)
    debugMsg+=m+"\n";
}
void println(const char *m) {
  if(serial==1)
    Serial.println(m);
  else if(serial==2)
    debugMsg+=String(m)+"\n";
}
void println(char *m) {
  if(serial==1)
    Serial.println(m);
  else if(serial==2)
    debugMsg+=String(m)+"\n";
}
void println(int m) {
  if(serial==1)
    Serial.println(m);
  else if(serial==2)
    debugMsg+=String(m)+"\n";
}
void println(uint8_t m) {
  if(serial==1)
    Serial.println(m);
  else if(serial==2)
    debugMsg+=String(m)+"\n";
}
void println(uint16_t m) {
  if(serial==1)
    Serial.println(m);
  else if(serial==2)
    debugMsg+=String(m)+"\n";
}
void println(uint32_t m) {
  if(serial==1)
    Serial.println(m);
  else if(serial==2)
    debugMsg+=String(m)+"\n";
}
void println(float m) {
  if(serial==1)
    Serial.println(m);
  else if(serial==2)
    debugMsg+=String(m)+"\n";
}
void println(double m) {
  if(serial==1)
    Serial.println(m);
  else if(serial==2)
    debugMsg+=String(m)+"\n";
}
