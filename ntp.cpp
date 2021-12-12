/*
 * ntp service
 * by Fabrizio Allevi
 * fabrizio . allevi @ tiscali . it
 */

#include <WiFi.h>
#include <WiFiUdp.h>

#include "main.h"
#include "secret.h"

/*struct tm {
  int	tm_sec;
  int	tm_min;
  int	tm_hour;
  int	tm_mday;
  int	tm_mon;
  int	tm_year;
  int	tm_wday;
  int	tm_yday;
  int	tm_isdst;
  ...
}*/
int portNTP=2390;
bool updateNtpTime() {
  const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
  byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
  WiFiUDP Udp;
  if(Udp.begin(portNTP++)) {
    delay(100);
    while (Udp.parsePacket() > 0) ; // discard any previously received packets
    // send NTP packet---------------------------------
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    packetBuffer[1] = 0;     // Stratum, or type of clock
    packetBuffer[2] = 6;     // Polling Interval
    packetBuffer[3] = 0xEC;  // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12]  = 49;
    packetBuffer[13]  = 0x4E;
    packetBuffer[14]  = 49;
    packetBuffer[15]  = 52;
    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    //IPAddress timeServer(129, 6, 15, 28); // time.nist.gov NTP server
    IPAddress timeServer(193,204,114,105);  // time.inrim.it
    Udp.beginPacket(timeServer, 123); //NTP requests are to port 123
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
    println("Transmit NTP Request");
      //-------------------------------------------------
    uint32_t beginWait = millis();
    while (millis() - beginWait < 1500) {
      int size = Udp.parsePacket();
      if (size >= NTP_PACKET_SIZE) {
        println("Receive NTP Response");
        Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
        unsigned long secsSince1900;
        // convert four bytes starting at location 40 to a long integer
        secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
        secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
        secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
        secsSince1900 |= (unsigned long)packetBuffer[43];
        time_t nptt=secsSince1900 - 2208988800UL + 3600; // +Central European Time+legal
        setTime(nptt);
        print("Time from NTP: ");print(hour());print(":");print(minute());print(":");println(second());
        return true;
      }
    }
    println("No NTP response :-(");
  }
  else {
    println("No UDP service :-(");
  }
  return false;
}


bool updateNTP() {
  bool bRet=false;
  int retryN=5;
  while(!bRet && retryN--) {
    bRet=updateNtpTime();
    if(!bRet) {
      print(".");
      delay(200);
    }
    blink();
  }
  stopBlink();
  return bRet;
}
