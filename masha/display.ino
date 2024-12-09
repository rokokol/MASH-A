#include <GyverOLED.h>
#include "bitmaps.h"

// MACROS
// common
#define FPS 1000 / 4  // 4 FPS

// time
#define TIME_X_POS 20
#define TIME_Y_POS 3
#define TIME_X_POS_MINI 48
#define TIME_Y_POS_MINI 2

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
#define METEO_X 48
#define METEO_Y 2
#define METEO_ICON_X 32
#define METEO_ICON_Y 16
#define UPDATTE_TIME_TICK 5000

// OBJECTS
// display
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

// VARS
static unsigned int mode = 0;
static unsigned int display_mode = 0;
static bool is_on = true;
static bool big_timer = true;
static bool update_time = true;
static bool is_stopwatch_started = false;

// TIME VARS
static ulong last_display_tick = 0;
static ulong temp_draw_tick = 0;
static ulong mode_draw_tick = 0;
static ulong stopwatch_started_millis = 0;
static ulong last_stopwatch_diff = 0;

void display_init() {
  setCompileTime();
  oled.init();
  oled.clear();

  display_temp(true);
  draw_bars();
}

void display_time(bool mini = false) {
  set_cursor_for_time(mini);

  char time[6];
  oled.print(parse_digits_to_time(time, hour(), minute()));
}

void display_info() {
  oled.setScale(1);
  display_bpm();
  display_temp(false);
  display_connection_status();
  display_battery();
}

void display_tick() {
  if (millis() - last_display_tick > FPS) {
    display_info();

    if (update_time) {
      display_time(!big_timer);
    }

    switch (get_display_mode()) {
      case TIME:
        if (is_stopwatch_correct() && last_stopwatch_diff >= 1000) {
          display_stopwatch(true);
        }
        break;

      case METEO:
        update_meteo();
        break;

      case STOPWATCH:
        display_stopwatch(false);
        break;
    }

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

void display_temp(bool force) {
  if (force || millis() - temp_draw_tick > BME_TICK) {
    if (force) {
      check_bme(true);
    }

    oled.setScale(1);
    oled.setCursor(BME_X_POS + 12, BME_Y_POS);
    oled.print("   ");
    oled.setCursor(BME_X_POS + 12, BME_Y_POS);

    char buffer[3];
    int temp = (int)get_temperature();
    sprintf(buffer, "%+3d", temp);
    oled.print(buffer);
    temp_draw_tick = millis();
  }
}

void draw_bars() {
  const char* mode_name = MODES[mode % MODES_COUNT];

  oled.setCursor(0, 7);
  oled.setScale(1);
  oled.print(mode_name);

  oled.drawBitmap(HEART_X_POS, HEART_Y_POS, HEART, 8, 8);
  oled.drawBitmap(BME_X_POS, BME_Y_POS, THERMOMETR, 8, 8);
  oled.drawBitmap(BME_X_POS + 30, BME_Y_POS, CELSIUS, 8, 8);
}

void switch_bar(int dir) {
  if (dir > 0) {
    mode = (mode + 1) % MODES_COUNT;
  } else {
    mode = (MODES_COUNT + mode - 1) % MODES_COUNT;
  }
  Serial.print("mode: ");
  Serial.println(mode);

  draw_bars();
}

void display_connection_status() {
  oled.drawBitmap(CONNECTION_X_POS, CONNECTION_Y_POS, DISCONNECTED, 8, 8);
}

void display_battery() {
  oled.drawBitmap(BATTERY_X_POS, BATTERY_Y_POS, BATTERY_DEAD, 8, 8);
}

unsigned int get_mode() {
  return mode;
}

unsigned int get_display_mode() {
  return display_mode;
}

bool is_display_on() {
  return is_on;
}

char* parse_digits_to_time(char* buffer, int first_digits, int second_digits) {
  buffer[5] = '\0';
  sprintf(buffer, "%02d", first_digits);

  if (second() % 2) {
    buffer[2] = ':';
  } else {
    buffer[2] = ' ';
  }
  char temp[2];

  sprintf(temp, "%02d", second_digits);
  buffer[3] = temp[0];
  buffer[4] = temp[1];

  return buffer;
}

void display_bme(bool another) {
  oled.setScale(1);

  oled.drawBitmap(METEO_ICON_X, METEO_ICON_Y, THERMOMETR, 8, 8);
  oled.drawBitmap(METEO_ICON_X, METEO_ICON_Y + 8, HUMIDITY, 8, 8);
  oled.drawBitmap(METEO_ICON_X, METEO_ICON_Y + 16, PRESSURE, 8, 8);

  float hum = Bme.readHumidity();
  float press = Bme.readPressure();

  oled.setCursor(METEO_X, METEO_Y);
  if (another) {
    oled.print(get_temperature() + 273.15);
    oled.print(' ');
    oled.drawBitmap(METEO_ICON_X + 52, METEO_ICON_Y, KELVIN, 8, 8);
  } else {
    oled.print(get_temperature());
    oled.drawBitmap(METEO_ICON_X + 48, METEO_ICON_Y, CELSIUS, 8, 8);
  }

  oled.setCursor(METEO_X, METEO_Y + 1);
  oled.print(get_humidity());
  oled.print(" %");

  oled.setCursor(METEO_X, METEO_Y + 2);
  if (another) {
    oled.print(get_pressure() / 133.32, 2);
    oled.print(" mmHg");
  } else {
    oled.print(get_pressure() / 1000, 2);
    oled.print(" kPa");
  }
}

void update_meteo() {
  if (millis() - mode_draw_tick >= BME_TICK) {
    display_bme(false);
    mode_draw_tick = millis();
  }
}

void display_stopwatch(bool mini) {
  if (!is_stopwatch_correct()) {
    return;
  }

  set_cursor_for_time(mini);
  ulong diff;
  if (is_stopwatch_started) {
    diff = (millis() - stopwatch_started_millis) / 1000;
  } else {
    diff = last_stopwatch_diff / 1000;
  }

  char value[6];
  oled.print(parse_digits_to_time(value, diff / 60, diff % 60));
}

void toggle_stopwatch() {
  if (is_stopwatch_started) {
    last_stopwatch_diff = millis() - stopwatch_started_millis;
  } else {
    stopwatch_started_millis = millis() - last_stopwatch_diff;
  }

  is_stopwatch_started = !is_stopwatch_started;
}

bool is_stopwatch_correct() {
  return millis() - stopwatch_started_millis < 3600000;
}

void set_cursor_for_time(bool mini) {
  if (mini) {
    oled.setScale(1);
    oled.setCursor(TIME_X_POS_MINI, TIME_Y_POS_MINI);
  } else {
    oled.setScale(3);
    oled.setCursor(TIME_X_POS, TIME_Y_POS);
  }
}

void reset_stopwatch() {
  stopwatch_started_millis = millis();
  last_stopwatch_diff = 0;
  is_stopwatch_started = false;
}

void switch_mode(int mode) {
  // erase workspase
  oled.setCursor(0, 1);
  oled.setScale(4);
  oled.print("         ");

  oled.setCursor(0, 3);
  oled.setScale(4);
  oled.print("         ");

  if (is_stopwatch_started) {
    toggle_stopwatch();
  }

  switch (get_mode()) {
    case TIME:
      display_time();
      display_mode = TIME;
      big_timer = true;
      update_time = true;
      break;

    case METEO:
      display_bme(false);
      display_mode = METEO;
      update_time = false;
      big_timer = false;
      break;

    case STOPWATCH:
      display_time(true);
      display_stopwatch(false);
      display_mode = STOPWATCH;
      update_time = true;
      big_timer = false;
      break;
  }
}
