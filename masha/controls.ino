#include <OneButton.h>
#include <EncButton.h>
#include <GRGB.h>
#include <PulseSensorPlayground.h>
#include <GyverBME280.h>
#include "modes.h"

// MACROS
// common
#define left_HOLD_START 300
#define right_CLICK_TIMEOUT 100

// buttons
#define BUTTON_R 8
#define BUTTON_C 5
#define BUTTON_L 2

// laser
#define LASER_PIN 3

// pulse sensor
#define PULSE_SENSOR_PIN A0
#define PULSE_THRESHOLD 550
#define PUSLSE_SENSOR_TICK 20

// led
#define LED_R 9
#define LED_G 6
#define LED_B 7
#define WHEEL_START 160

// bme
#define BME_TICK 5000


// OBJECTS
// led
GRGB led(COMMON_CATHODE, LED_R, LED_G, LED_B);

// bme
GyverBME280 bme;

// buttons
Button right_button(BUTTON_R);
Button center_button(BUTTON_C);
Button left_button(BUTTON_L);

// pulse sensor
PulseSensorPlayground pulseSensor;

// VARS
// laser
bool laser_flag = false;

// led
bool led_flag = false;
static byte led_wheel = WHEEL_START;

// bme
static int temperature = 0;
static int humidity = 0;
static int pressure = 0;

// pulse sensor
static int current_pulse = 0;

// TIME VARS
ulong bme_tick = BME_TICK;

void init_controls() {
  // buttons
  left_button.setHoldTimeout(left_HOLD_START);
  right_button.setClickTimeout(right_CLICK_TIMEOUT);

  // pulse sensor
  pulseSensor.analogInput(PULSE_SENSOR_PIN);
  pulseSensor.setThreshold(PULSE_THRESHOLD);
  if (!pulseSensor.begin()) Serial.println("Pulse Sensor Error!");

  // bme
  if (!bme.begin(0x76)) Serial.println("BME Error!");

  pinMode(LASER_PIN, OUTPUT);
}

void tick_controls() {
  right_button.tick();
  center_button.tick();
  left_button.tick();

  // left button
  if (left_button.hold()) {
    Serial.println("left hold");

    if (!led_flag) {
      left_hold();
    }
  } else if (left_button.step(1)) {
    Serial.println("left step 1");

    if (led_flag) {
      led_wheel += 8;
      led.setWheel8(led_wheel);
    }
  } else if (left_button.click(1)) {
    Serial.println("left click");

    if (is_display_on()) {
      switch_bar(-1);
    }
  } else if (left_button.release()) {
    Serial.println("left release");

    left_hold_release();
  }

  // right button
  if (right_button.click(2)) {
    Serial.println("right double click");
    toggle_led();
  } else if (right_button.step(1)) {
    Serial.println("right step 1");

    if (led_flag) {
      led_wheel -= 8;
      led.setWheel8(led_wheel);
    }
  } else if (right_button.click(1)) {
    Serial.println("right click");

    if (is_display_on()) {
      switch_bar(1);
    }
  }

  // bme 
  check_bme();
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
  if (led_flag) {
    led_flag = false;
    led.disable();
    led_wheel = WHEEL_START;
    Serial.println("led off");
  } else {
    led_flag = true;
    led.enable();
    led.setRGB(255, 255, 255);
  }
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

void check_bme() {
  if (millis() - bme_tick > BME_TICK) {
    temperature = bme.readTemperature();
    humidity = bme.readHumidity();
    pressure = bme.readPressure();

    bme_tick = millis();
  }
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
