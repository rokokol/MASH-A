#include <Arduino.h>
#include <WiFi.h>
#include <TimeLib.h>
#include <FastBot2.h>
#include <Wire.h>

#define ulong unsigned long
#define SDA_PIN 15 
#define SCL_PIN 14 


void setup() {
  Serial.begin(115200);
  Serial.println();
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.println("I2C Initialized!");

  controls_init();
  display_init();
  // camera_init();
  
  Serial.println("start");
}

void loop() {
  ulong t1 = millis();

  controls_tick();
  display_tick();
  // camera_tick();

  ulong diff = millis() - t1;
  if (diff > 100) {
    Serial.print("Time per tick: ");
    Serial.println(diff);
  }
}

void setCompileTime() {
  int compileHour = atoi(__TIME__);
  int compileMinute = atoi(__TIME__ + 3);
  int compileSecond = atoi(__TIME__ + 6);

  setTime(compileHour, compileMinute, compileSecond, 0, 0, 0);
}
