#include <TimeLib.h>

#define ulong unsigned long

void setup() {
  Serial.begin(9600);

  controls_init();
  display_init();
  
  Serial.println("start");
}

void loop() {
  ulong t1 = millis();

  controls_tick();
  display_tick();

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
