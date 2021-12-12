/*
 * relay service
 * by Fabrizio Allevi
 * fabrizio . allevi @ tiscali . it
 */

#include <WiFiNINA.h>
#include "main.h"
#include "secret.h"

bool turn(bool b) {
  bool bRet=false;
  WiFiClient client;
  if(plant.nCh!=-1) {
    client.stop();
    String serverName=(plant.nCh==1?HEATER2:HEATER1);
    String operation=b?"GET /relay/0?turn=on HTTP/1.1":"GET /relay/0?turn=off HTTP/1.1";
    print(b?"turnONrelay":"turnOFFrelay"); print(" on "); println(serverName.c_str());
    if(client.connect(serverName.c_str(), 80)) {
      client.println(operation.c_str());
      client.print("Host: "); client.println(serverName.c_str());
      client.println("Connection: close");
      client.print("Authorization: Basic "); client.println(ENCODE64_SHELLY);
      client.println();
    }
    int i=0;
    int dly=0;
    char a[256];
    memset(a,0,256);
    while(i==0 && dly<500) {
      while(client.available()) {
        a[i++]=client.read();
      }
      if(i==0) {
        delay(1);
        dly++;
      }
    }
    if (i>0) {
      String shellyMsg=a;
      int ini = shellyMsg.indexOf("{\"ison\":");
      String ss=shellyMsg.substring(ini);
      println(ss.c_str());
      int fin = shellyMsg.indexOf(",");
      ss=shellyMsg.substring(ini+8,fin);
      println(ss);
      if((b && ss=="true") || (!b && ss=="false"))
        bRet=true;
      else
        bRet=false;
    }
    else {
      println("client not Answer!");
    }
  }
  return bRet;
}
