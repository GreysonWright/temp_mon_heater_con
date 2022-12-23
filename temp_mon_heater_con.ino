/*
 * Created by Pi BOTS MakerHub
 *
 * Email: pibotsmakerhub@gmail.com
 * 
 * Github: https://github.com/pibotsmakerhub
 *
 * Join Us on Telegram : https://t.me/pibots 
 * Copyright (c) 2020 Pi BOTS MakerHub
*/

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.

DHT dht(DHTPIN, DHTTYPE, 6);
int relay = 5;
bool isHeating = false;
unsigned int measureCountHigh = 0;
unsigned int measureCountLow = 0;
double targetTemp = 42.00;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(relay, OUTPUT);
}

void loop() { 
  delay(2000);
  float f = dht.readTemperature(true);

  if (isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  Serial.print(F("Temperature: "));
  Serial.println(f);
  
  Serial.println(targetTemp - 3);
  Serial.println(targetTemp + 3);

  if (f <= targetTemp - 3 && !isHeating) {
    measureCountLow += 1;
    measureCountHigh = 0;
  } 
  if (f >= targetTemp + 3 && isHeating) {
    measureCountHigh += 1;
    measureCountLow = 0;
  }

  Serial.println(measureCountLow);
  Serial.println(measureCountHigh);
  Serial.println(isHeating);

  if (measureCountLow >= 3 && !isHeating) {
    Serial.println("turn on heat");
    measureCountHigh = 0;
    measureCountLow = 0;
    isHeating = true;
    digitalWrite(relay, HIGH);
    // delay(60000);
  }

  if (measureCountHigh >= 3 && isHeating) {
    Serial.println("turn off heat");
    measureCountHigh = 0;
    measureCountLow = 0;
    isHeating = false;
    digitalWrite(relay, LOW);
    delay(90000);
  }

}

