/*
   temp: gestione lettura della temperatura da sensore DS18B20
   by Fabrizio Allevi
   fabrizio . allevi @ tiscali . it
*/

/*
   temps acquarium V2
   by Fabrizio Allevi
   fabrizio . allevi @ tiscali . it
*/

#include <OneWire.h>
#include <DallasTemperature.h>
#include "main.h"

// Data wire is plugged into port 13 on the ESP32
#define ONE_WIRE_BUS 2
// precision
#define TEMPERATURE_PRECISION 12

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
DeviceAddress h2oThermometer;

void initTemp() {
  // Start up the library
  sensors.begin();
  // locate devices on the bus
  print(sensors.getDeviceCount());print(" ");
  // report parasite power requirements
  if (sensors.isParasitePowerMode())
    print(" parasite is ON");
  else
    print(" parasite is OFF");
  // search sensor by index 0
  if(!sensors.getAddress(h2oThermometer, 0))
    println("Unable to find address for Device 0");
  // show the addresses we found on the bus
  print(" Address:");
  for (uint8_t i = 0; i < 8; i++) {
    // zero pad the address if necessary
    if (h2oThermometer[i] < 16)
      print("0");
    print(h2oThermometer[i]);
  }
  print(" resolution is:");
  // set the resolution to 9 bit per device
  sensors.setResolution(h2oThermometer, TEMPERATURE_PRECISION);
  print(sensors.getResolution(h2oThermometer));
  println();
}

void handleTemp(float &temperature) {
  float H2oTemp=0.0f;
  sensors.requestTemperatures(); // Send the command to get temperatures
  H2oTemp=sensors.getTempC(h2oThermometer);
  if(H2oTemp==DEVICE_DISCONNECTED_C) {
    println("Error: Could not read temperature data");
  }
  else {
    print("Temp C: ");
    temperature=H2oTemp+plant.tOffset;
    println(temperature);
  }
}
