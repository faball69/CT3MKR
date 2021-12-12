/*
 * data service
 * by Fabrizio Allevi
 * fabrizio . allevi @ tiscali . it
 */

#include "main.h"
#include <WiFiNINA.h>

stPlant plant = {true, 0.0, 19.0, 20.8, 0.2, 7*60, 22*60, -1};

bool saveData() {
  WiFiStorageFile s = WiFiStorage.open("/fs/plant");
  if(s) {
    s.erase();
  }
  s.write(&plant, sizeof(stPlant));
  println("flash update storage!");
  return true;
}

bool loadData() {
  // init storage
  WiFiStorageFile s = WiFiStorage.open("/fs/plant");
  if (s) {
    while (s.available()) {
      int ret = s.read(&plant, sizeof(stPlant));
      if(ret<sizeof(stPlant)) {
        println("file storage is not equal!");
        saveData();
      }
      else {
        println("file storage read WELL!");
      }
    }
  }
  else {
    println("storage IS NOT VALID!");
    saveData();
  }
}
