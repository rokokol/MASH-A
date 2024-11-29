#include <TimeLib.h>

#define ulong unsigned long

void setup() {
  display_init();
  init_buttons();

  Serial.begin(9600);
  Serial.println("start");
}

void loop() {
  display_tick();
  tick_buttons();
}

void setCompileTime() {
  int compileHour = atoi(__TIME__);
  int compileMinute = atoi(__TIME__ + 3);
  int compileSecond = atoi(__TIME__ + 6);

  setTime(compileHour, compileMinute, compileSecond, 0, 0, 0);
}
