#include <OneButton.h>
#include <EncButton.h>
#include <GRGB.h>
#include <PulseSensorPlayground.h>
#include <GyverBME280.h>

// MACROS
// common
#define RIGHT_HOLD_START 300
#define LEFT_CLICK_TIMEOUT 100

// buttons
#define BUTTON_L 8
#define BUTTON_C 5
#define BUTTON_R 2

// laser
#define LASER_PIN 3

// pulse sensor
#define PULSE_SENSOR_PIN A0
#define PULSE_THRESHOLD 540
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
Button left_button(BUTTON_L);
Button center_button(BUTTON_C);
Button right_button(BUTTON_R);

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
ulong bme_tick = 0;

void init_controls() {
  // buttons
  right_button.setHoldTimeout(RIGHT_HOLD_START);
  left_button.setClickTimeout(LEFT_CLICK_TIMEOUT);

  // pulse sensor
  pulseSensor.analogInput(PULSE_SENSOR_PIN);
  pulseSensor.setThreshold(PULSE_THRESHOLD);
  if (!pulseSensor.begin()) Serial.println("Pulse Sensor Error!");

  // bme
  if (!bme.begin(0x76)) Serial.println("BME Error!");

  pinMode(LASER_PIN, OUTPUT);
}

void tick_controls() {
  left_button.tick();
  center_button.tick();
  right_button.tick();

  // right button
  if (right_button.hold()) {
    if (!led_flag) {
      right_hold();
    }
  } else if (right_button.release()) {
    right_hold_release();
  } else if (right_button.step(1)) {
    if (led_flag) {
      led_wheel += 8;
      led.setWheel8(led_wheel);
    }
  }

  // left button
  if (left_button.click(2)) {
    toggle_led();
  } else if (left_button.step(1)) {
    if (led_flag) {
      led_wheel -= 8;
      led.setWheel8(led_wheel);
    }
  }

  // bme 
  check_bme();
}

void right_hold() {
  if (!laser_flag) {
    laser_flag = true;
    digitalWrite(LASER_PIN, laser_flag);
  }
}

void right_hold_release() {
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
