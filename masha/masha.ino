#include <TimeLib.h>

#define ulong unsigned long

void setup() {
  Serial.begin(9600);

  display_init();
  init_controls();
  
  Serial.println("start");
}

void loop() {
  ulong t1 = millis();

  display_tick();
  tick_controls();

  ulong diff = millis() - t1;
  if (diff > 100) {
    Serial.println(diff);
  }
}

void setCompileTime() {
  int compileHour = atoi(__TIME__);
  int compileMinute = atoi(__TIME__ + 3);
  int compileSecond = atoi(__TIME__ + 6);

  setTime(compileHour, compileMinute, compileSecond, 0, 0, 0);
}
