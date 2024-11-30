#include <GyverOLED.h>
#include "bitmaps.h"

// MACROS
// common
#define FPS 1000 / 4  // 4 FPS

// time
#define TIME_X_POS 20
#define TIME_Y_POS 3

// battery
#define BATTERY_X_POS 116
#define BATTERY_Y_POS 0

// connection
#define CONNECTION_X_POS 104
#define CONNECTION_Y_POS 0

// pulse sensor
#define HEART_X_POS 0
#define HEART_Y_POS 0

// bme
#define BME_X_POS 36
#define BME_Y_POS 0

// OBJECTS
// display
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

// TIME VARS
ulong last_display_tick = 0;

void display_init() {
  setCompileTime();
  oled.init();
  oled.clear();

  oled.drawBitmap(HEART_X_POS, HEART_Y_POS, HEART, 8, 8);
  oled.drawBitmap(BME_X_POS, BME_Y_POS, THERMOMETR, 8, 8);
  oled.drawBitmap(BME_X_POS + 30, BME_Y_POS, CELSIUS, 8, 8);
}

void display_time() {
  oled.setScale(3);
  oled.setCursor(TIME_X_POS, TIME_Y_POS);
  char buffer[2];
  sprintf(buffer, "%02d", hour());
  oled.print(buffer);

  if (second() % 2) {
    oled.print(':');
  } else {
    oled.print(' ');
  }
  sprintf(buffer, "%02d", minute());
  oled.print(buffer);
}

void display_info() {
  oled.setScale(1);
  display_bpm();
  display_bme();
  display_connection_status();
  display_battery();
}

void display_tick() {
  if (millis() - last_display_tick > FPS) {
    display_info();
    display_time();
    oled.update();

    last_display_tick = millis();
  }
}

void display_bpm() {
  oled.setCursor(HEART_X_POS + 12, HEART_Y_POS);
  oled.print("   ");
  oled.setCursor(HEART_X_POS + 12, HEART_Y_POS);

  int bpm = get_pulse();
  if (bpm > 200 || bpm < 10) {
    oled.print("--");
  } else {
    oled.print(bpm);
  }
}

void display_bme() {
  oled.setCursor(BME_X_POS + 12, BME_Y_POS);
  oled.print("   ");
  oled.setCursor(BME_X_POS + 12, BME_Y_POS);

  char buffer[3];
  int temp = (int) get_temperature();
  sprintf(buffer, "%+3d", temp);
  oled.print(buffer);
}

void display_connection_status() {
  oled.drawBitmap(CONNECTION_X_POS, CONNECTION_Y_POS, DISCONNECTED, 8, 8);
}

void display_battery() {
  oled.drawBitmap(BATTERY_X_POS, BATTERY_Y_POS, BATTERY_DEAD, 8, 8);
}
