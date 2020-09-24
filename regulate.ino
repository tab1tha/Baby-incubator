
// DHT Temperature & Humidity Sensing, monitoring and control
//for a baby incubator.
// Written by Tambe Tabitha Achere

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define DHTPIN 2   // Digital pin connected to the DHT sensor 
#define DHTTYPE    DHT11     // DHT 11

// guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

LiquidCrystal_I2C lcd(0x27,16,2);
DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

const int coolingFan = 7;
const int exhaustFan = 8;
const int humidifierFan = 9;
const int heaterRelay = 10;
const int heatingFan = 11;
const int bulbRelay = 12;


const float maxHumidity = 98.0; //initiallly 75
const float minHumidity = 50.0; //initially 70
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
  Serial.println(F("I promise to keep your baby safe !"));
  
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
  
  lcd.begin(16,2);
  //lcd.begin();
  //lcd.print("Baby is safe");
  
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
 
}

void loop() {
  // Delay of one minute between measurements.
  delay(60000);
  
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  
  Serial.print(F("Temperature: "));
  Serial.print(event.temperature);
  Serial.println(F("°C"));
  
  lcd.setCursor(0,0); 
  lcd.print("Temperature:");
  lcd.print(event.temperature);
  lcd.print((char)223);
  lcd.print("C");
    
  //Regulate temperature
  if (!isnan(event.temperature) && event.temperature>=minTemperature && event.temperature<=maxTemperature) {
      digitalWrite(coolingFan, HIGH);
      digitalWrite(heatingFan, HIGH);
      digitalWrite(bulbRelay, HIGH);
      Serial.println(F("Temperature is normal"));
    
  }
  else if(!isnan(event.temperature) && event.temperature<minTemperature){
      digitalWrite(heatingFan, LOW);
      digitalWrite(bulbRelay, LOW);
      digitalWrite(coolingFan, HIGH);
      Serial.println(F("Heating up"));
  }
  else if(!isnan(event.temperature) && event.temperature>maxTemperature){
      digitalWrite(coolingFan, LOW);
      digitalWrite(heatingFan, HIGH);
      digitalWrite(bulbRelay, HIGH);
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
  
  lcd.setCursor(0,1);
  lcd.print("Humidity:");
  lcd.print(event.relative_humidity);
  lcd.print("%");
    
  //Regulate humidity
  if (!isnan(event.relative_humidity) && event.relative_humidity>=minHumidity && event.relative_humidity<=maxHumidity ) {
      digitalWrite(exhaustFan, HIGH);
      digitalWrite(humidifierFan, HIGH);
      digitalWrite(heaterRelay, HIGH);
      Serial.println(F("Humidity is normal"));
    
  }
  else if(!isnan(event.relative_humidity) && event.relative_humidity>maxHumidity){
      digitalWrite(exhaustFan, LOW);
      digitalWrite(humidifierFan, HIGH);
      digitalWrite(heaterRelay, HIGH);
      Serial.println(F("Reducing humidity"));
  }
  else if(!isnan(event.relative_humidity) && event.relative_humidity<minHumidity){
      digitalWrite(humidifierFan, LOW);
      digitalWrite(heaterRelay, LOW);
      digitalWrite(exhaustFan, HIGH);
      Serial.println(F("Increasing humidity")); 
  }
  else{
    Serial.println(F("Error reading humidity!"));  
  }


}
