#include <OneButton.h>
#include <EncButton.h>
#include <GRGB.h>

#define BUTTON_L 8
#define BUTTON_C 5
#define BUTTON_R 2

#define LASER_PIN 3

#define LED_R 9
#define LED_G 6
#define LED_B 7

#define RIGHT_HOLD_START 300
#define LEFT_CLICK_TIMEOUT 100
#define WHEEL_START 160

GRGB led(COMMON_CATHODE, LED_R, LED_G, LED_B);

Button left_button(BUTTON_L);
Button center_button(BUTTON_C);
Button right_button(BUTTON_R); 

bool laser_flag = false;
bool led_flag = false;
static byte led_wheel = WHEEL_START;

void init_buttons() {
  right_button.setHoldTimeout(RIGHT_HOLD_START);

  left_button.setClickTimeout(LEFT_CLICK_TIMEOUT);

  pinMode(LASER_PIN, OUTPUT);
}

void tick_buttons() {
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
      Serial.println(led_wheel);
    }
  }
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
