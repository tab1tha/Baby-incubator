// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2   // Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

#define DHTTYPE    DHT11     // DHT 11

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

const int coolingFan = 7;
const int exhaustFan = 8;
const int humidifierFan = 9;
const int heaterRelay = 10;
const int heatingFan = 11;
const int bulbRelay = 12;


const float maxHumidity = 75.0;
const float minHumidity = 70.0;
const float maxTemperature = 37.5;
const float minTemperature = 36.5;

void setup() {
  
  pinMode(DHTPIN, INPUT);
  pinMode(exhaustFan, OUTPUT);
  pinMode(humidifierFan, OUTPUT);
  pinMode(heaterRelay, OUTPUT);
  pinMode(heatingFan, OUTPUT);
  pinMode(bulbRelay, OUTPUT);
  pinMode(coolingFan, OUTPUT);

  digitalWrite(exhaustFan,HIGH);
  digitalWrite(humidifierFan,HIGH);
  digitalWrite(heaterRelay,HIGH);
  digitalWrite(heatingFan,HIGH);
  digitalWrite(bulbRelay,HIGH);
  digitalWrite(coolingFan,HIGH);
  
  Serial.begin(9600);
  // Initialize device.
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

void loop() {
  // Delay between measurements.
  //delay(delayMS);
  delay(5000);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);

  Serial.print(F("Temperature: "));
  Serial.print(event.temperature);
  Serial.println(F("°C"));
    
  if (!isnan(event.temperature) && event.temperature>=minTemperature && event.temperature<=maxTemperature) {
      digitalWrite(coolingFan, HIGH);
      digitalWrite(heatingFan, HIGH);
      digitalWrite(bulbRelay, HIGH);
      Serial.println(F("Temperature is normal"));
    
  }
  else if(!isnan(event.temperature) && event.temperature<minTemperature){
      digitalWrite(heatingFan, LOW);
      digitalWrite(bulbRelay, LOW);
      Serial.println(F("Heating up"));
  }
  else if(!isnan(event.temperature) && event.temperature>maxTemperature){
      digitalWrite(coolingFan, LOW);
      Serial.println(F("Cooling down"));
  }
  else{
   Serial.println(F("Error reading temperature!"));
  }

  
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);

  Serial.print(F("Humidity: "));
  Serial.print(event.relative_humidity);
  Serial.println(F("%"));
    
  if (!isnan(event.relative_humidity) && event.relative_humidity>=minHumidity && event.relative_humidity<=maxHumidity ) {
      digitalWrite(exhaustFan, HIGH);
      digitalWrite(humidifierFan, HIGH);
      digitalWrite(heaterRelay, HIGH);
      Serial.println(F("Humidity is normal"));
    
  }
  else if(!isnan(event.relative_humidity) && event.relative_humidity>maxHumidity){
      digitalWrite(exhaustFan, LOW);
      Serial.println(F("Reducing humidity"));
  }
  else if(!isnan(event.relative_humidity) && event.relative_humidity<minHumidity){
      digitalWrite(humidifierFan, LOW);
      digitalWrite(heaterRelay, LOW);
      Serial.println(F("Increasing humidity")); 
  }
  else{
    Serial.println(F("Error reading humidity!"));  
  }
}
