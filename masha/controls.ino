#include <OneButton.h>
#include <EncButton.h>
#include <PulseSensorPlayground.h>
#include <GyverBME280.h>
#include "modes.h"

// MACROS
// buttons
#define LEFT_HOLD_START 300
#define LEFT_CLICK_TIMEOUT 200
#define CENTER_CLICK_TIMEOUT 200
#define RIGHT_CLICK_TIMEOUT 200
#define CENTER_HOLD_START 300

// buttons
#define BUTTON_R 0
#define BUTTON_C 16
#define BUTTON_L 2

// laser
#define LASER_PIN 12

// pulse sensor
#define PULSE_SENSOR_PIN 13
#define PULSE_THRESHOLD 520
#define PUSLSE_SENSOR_TICK 20

// led
// #define LED_R 6
// #define LED_G 5
// #define LED_B 3
// #define WHEEL_START 160
#define LED_PIN 4

// bme
#define BME_TICK 5000

// modes
#define TIME 0
#define METEO 1
#define STOPWATCH 2
// #define WIFI 3


// OBJECTS
// led
// GRGB Led(COMMON_CATHODE, LED_R, LED_G, LED_B);

// bme
GyverBME280 Bme;

// buttons
Button RightButton(BUTTON_R);
Button CenterButton(BUTTON_C);
Button LeftButton(BUTTON_L);

// pulse sensor
PulseSensorPlayground pulseSensor;

// VARS
// laser
bool laser_flag = false;

// led
bool led_flag = false;
// static byte led_wheel = WHEEL_START;

// bme
static float temperature = 0;
static float humidity = 0;
static float pressure = 0;

// pulse sensor
static int current_pulse = 0;

// TIME VARS
ulong bme_tick = BME_TICK;

void controls_init() {
  // buttons
  LeftButton.setHoldTimeout(LEFT_HOLD_START);
  LeftButton.setClickTimeout(LEFT_CLICK_TIMEOUT);
  CenterButton.setClickTimeout(CENTER_CLICK_TIMEOUT);
  RightButton.setClickTimeout(RIGHT_CLICK_TIMEOUT);
  CenterButton.setHoldTimeout(CENTER_HOLD_START);

  // pulse sensor
  pulseSensor.analogInput(PULSE_SENSOR_PIN);
  pulseSensor.setThreshold(PULSE_THRESHOLD);
  if (!pulseSensor.begin()) Serial.println("Pulse Sensor Error!");

  // bme
  if (!Bme.begin(0x76)) Serial.println("BME Error!");
  check_bme(true);

  // laser
  pinMode(LASER_PIN, OUTPUT);

  // led
  pinMode(LED_PIN, OUTPUT);
}

void controls_tick() {
  RightButton.tick();
  CenterButton.tick();
  LeftButton.tick();

  // buttons
  handle_left_button();
  handle_center_button();
  handle_right_button();

  // bme
  check_bme(false);
}

void handle_left_button() {
  if (LeftButton.hold()) {
    Serial.println("left hold");

    if (!led_flag) {
      left_hold();
    }
  } else if (LeftButton.step(1)) {
    Serial.println("left step 1");

    // if (led_flag) {
    //   led_wheel += 8;
    //   Led.setWheel8(led_wheel);
    // }
  } else if (LeftButton.click(1)) {
    Serial.println("left click");

    if (is_display_on()) {
      switch_bar(-1);
    }
  } else if (LeftButton.click(2)) {
    Serial.println("left double click");

    make_photo();
  } else if (LeftButton.releaseStep()) {
    Serial.println("left step release");

    left_hold_release();
  }
}

void handle_center_button() {
  if (CenterButton.click(1)) {
    Serial.println("Center click 1");
    if (is_display_on()) {
      switch (get_display_mode()) {
        case METEO:
          display_bme(true);
          break;

        case STOPWATCH:
          toggle_stopwatch();
          break;
      }
    }
  } else if (CenterButton.click(2)) {
    Serial.println("Center click 2");

    if (is_display_on()) {
      switch_mode(get_mode());
    }
  } else if (CenterButton.hold()) {
    Serial.println("Center hold");

    switch (get_display_mode()) {
      case STOPWATCH:
        reset_stopwatch();
        break;
    }
  }
}

void handle_right_button() {
  if (RightButton.click(2)) {
    Serial.println("right double click");
    toggle_led();
  } else if (RightButton.step(1)) {
    Serial.println("right step 1");

    // if (led_flag) {
    //   led_wheel -= 8;
    //   Led.setWheel8(led_wheel);
    // }
  } else if (RightButton.click(1)) {
    Serial.println("right click");

    if (is_display_on()) {
      switch_bar(1);
    }
  }
}

void left_hold() {
  if (!laser_flag) {
    laser_flag = true;
    digitalWrite(LASER_PIN, laser_flag);
  }
}

void left_hold_release() {
  if (laser_flag) {
    laser_flag = false;
    digitalWrite(LASER_PIN, laser_flag);
  }
}

void toggle_led() {
  led_flag = !led_flag;
  digitalWrite(LED_PIN, led_flag);
  Serial.print("led mode: ");
  Serial.println(laser_flag);
  // if (led_flag) {
  //   led_flag = false;
  //   Led.disable();
  //   led_wheel = WHEEL_START;
  //   Serial.println("led off");
  // } else {
  //   led_flag = true;
  //   Led.enable();
  //   Led.setRGB(255, 255, 255);
  // }
}

int get_pulse() {
  if (pulseSensor.sawStartOfBeat()) {
    int bpm = pulseSensor.getBeatsPerMinute();
    if (bpm < 200 && bpm > 10) {
      current_pulse = bpm;
    }
  }

  return current_pulse;
}

void check_bme(bool force) {
  if (force || millis() - bme_tick > BME_TICK) {
    temperature = Bme.readTemperature();
    humidity = Bme.readHumidity();
    pressure = Bme.readPressure();

    bme_tick = millis();
  }
}

void make_photo() {
}

double get_temperature() {
  return temperature;
}

double get_humidity() {
  return humidity;
}

double get_pressure() {
  return pressure;
}
