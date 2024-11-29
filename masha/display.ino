#include <GyverOLED.h>
#include "bitmaps.h"

#define TIME_X_POS 20
#define TIME_Y_POS 3
#define BATTERY_X_POS 116
#define BATTERY_Y_POS 0
#define DISPLAY_TIME_TICK 1000 / 12  // 12 FPS

GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
ulong last_display_tick = 0;

void display_init() {
  setCompileTime();
  oled.init();
  oled.clear();
  oled.setScale(3);
  oled.home();
}

void display_time() {
  oled.setCursor(TIME_X_POS, TIME_Y_POS);
  char buffer[2];
  sprintf(buffer, "%02d", hour());
  oled.print(buffer);
  oled.print(':');
  sprintf(buffer, "%02d", minute());
  oled.print(buffer);
}

void display_battery() {
  oled.drawBitmap(BATTERY_X_POS, BATTERY_Y_POS, BATTERY_DEAD, 8, 8);
}

void display_tick() {
  if (millis() - last_display_tick >= DISPLAY_TIME_TICK) {
    display_battery();
    display_time();
    oled.update();

    last_display_tick = millis();
  }
}
