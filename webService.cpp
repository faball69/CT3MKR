/*
 * web service
 * by Fabrizio Allevi
 * fabrizio . allevi @ tiscali . it
 */

#include "main.h"
#include <WiFiNINA.h>

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void page0(WiFiClient &client) {
  char temp[5000];
  memset(temp, 0, sizeof(temp));
  snprintf(temp, 5000,
    "<html><body>\
    <script>if(typeof window.history.pushState == 'function') {window.history.pushState({}, \"Hide\", \"/\");}</script>\
    <h1>Plant%d</h1>\
    <strong>Temperature: </strong> %.1f <strong> Time: </strong> %.02d:%.02d <br>\
    <strong>Heater: </strong> %s <strong>Force: </strong> %s <br><br>\
    <table style=\"border-collapse: collapse; width: 100%%;\" border=\"1\">\
    <tbody>\
    <tr>\
    <td style=\"width: 12.5%%;\">\
    <form action=\"/get\">\
      offset[deg]:<input min=\"-7.0\" max=\"7.0\" name=\"PF1\" step=\"0.1\" type=\"number\" value=\"%.1f\" /><br><br>\
      tempLow[deg]:<input min=\"15.0\" max=\"20.0\" name=\"PF2\" step=\"0.1\" type=\"number\" value=\"%.1f\" /><br><br>\
      TempHigh[deg]:<input min=\"15.0\" max=\"23.0\" name=\"PF3\" step=\"0.1\" type=\"number\" value=\"%.1f\" /><br><br>\
      Hysteresis[deg]:<input min=\"0.0\" max=\"0.5\" name=\"PF4\" step=\"0.1\" type=\"number\" value=\"%.1f\" /><br><br>\
      timeStart[hh:mm]:<input type=\"time\" name=\"TS\" value=\"%.02d:%.02d\" /><br><br>\
      timeEnd[hh:mm]:<input type=\"time\" name=\"TE\" value=\"%.02d:%.02d\" /><br><br>\
      nCh:<input min=\"-1\" max=\"1\" type=\"number\" name=\"CH\" value=\"%d\" /> -1=APmode 0=Heater1 1=Heater2<br><br>\
      serial:<input min=\"0\" max=\"2\" type=\"number\" name=\"SE\" value=\"%d\" /> 0=disable 1=usb 2=buffer<br><br><br>\
    <input type=submit value=Save>Parameters</form><br>\
    </td>\
    <td style=\"width: 12.5%%;\">\
    <form action=\"/force\"><input type=submit value=Force>heater ON for 30min from now!</form><br><br><br>\
    <form action=\"/reset\"><input type=submit value=Reset>ESP32 Board to restart! Pay Attention!</form><br><br><br>\
    <form action=\"/plant\">now Plant is %s <input type=submit value=Turn%s></form>\
    </td>\
    </tr>\
    </tbody>\
    </table>\
    <br><strong> debugMsg: </strong> %s \
    </body></html>\n", plant.nCh+1, ambT, hour(), minute(), (state?"ON":"OFF"), (force?"ON":"OFF"),
                       plant.tOffset, plant.tLow, plant.tHigh, plant.tHyst, plant.minIni/60, plant.minIni%60, plant.minFin/60, plant.minFin%60, plant.nCh, serial,
                       (plant.bEnable?"ON":"OFF"), (plant.bEnable?"OFF":"ON"), debugMsg.c_str());
  client.println(temp);
}

void initWeb() {
  // start the web server on port 80
  server.begin();
}

char buffer[5000];
void handleWeb() {
  WiFiClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    while (client.connected()) {
      println("\nvvvvvv HTTP vvvvvv");
      if (client.available()) {
        memset(buffer, 0, sizeof(buffer));
        client.read((uint8_t*)buffer, sizeof(buffer));
        String text=buffer;
        print(text);
        if(text.indexOf("GET /get")!=-1) {
          int ini=text.indexOf("?");
          int fin=text.indexOf(" HTTP");
          if(ini!=-1 && fin!=-1) {  // ricezione dati cambiati
            bool bChangeFH=false;
            buffer[fin]='\0'; // termino stringa fin dove mi serve
            char *ptr=strtok(buffer+ini+1, "&");
            while(ptr!=NULL) {
              String s=ptr;
              ini = s.indexOf("=");
              fin = s.length();
              String var=s.substring(0,ini);
              String val=s.substring(ini+1,fin);
              println("var="+var);
              println("val="+val);
              if(var=="PF1") {
                plant.tOffset=val.toFloat();
              }
              else if(var=="PF2") {
                plant.tLow=val.toFloat();
              }
              else if(var=="PF3") {
                plant.tHigh=val.toFloat();
              }
              else if(var=="PF4") {
                plant.tHyst=val.toFloat();
              }
              else if(var=="TS") {
                plant.minIni=val.substring(0,2).toInt()*60+val.substring(5).toInt();
                println(plant.minIni);
              }
              else if(var=="TE") {
                plant.minFin=val.substring(0,2).toInt()*60+val.substring(5).toInt();
                println(plant.minFin);
              }
              else if(var=="CH") {
                int newInt=val.toInt();
                if(newInt!=plant.nCh) {
                  plant.nCh=newInt;
                  reset=true;
                }
              }
              else if(var=="SE") {
                int newInt=val.toInt();
                if(newInt!=serial) {
                  serial=newInt;
                }
              }
              ptr = strtok(NULL, "&");  // takes a list of delimiters
            }
          }
          saveData();
        }
        else if(text.indexOf("GET /force")!=-1) {
          if(plant.bEnable) {
            force=true;
            println("Force is enabled!");
          }
        }
        else if(text.indexOf("GET /reset")!=-1) {
          reset=true;
        }
        else if(text.indexOf("GET /plant")!=-1) {
          plant.bEnable=!plant.bEnable;
          if(plant.bEnable)
            println("Now plant is enabled!");
          else {
            println("Now plant is disabled!");
            force=false;
          }
          saveData();
        }
        // create page
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");  // the connection will be closed after completion of the response
        client.println();
        page0(client);
        println("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
        break;
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}
