// #include <Arduino.h>
#include <LineController.hpp>

const uint8_t left_led = 4, right_led = 5;
LineChecker   lines    = LineChecker(2, 3);
bool led               = true;
uint8_t sensor_status;


void setup() {
  // Serial.begin(9600);
}

void loop()  {
  sensor_status = lines.check();

  switch (sensor_status) {
  case LEFT_SENSOR_TRIGGERED:

    digitalWrite(left_led,  HIGH);
    digitalWrite(right_led, LOW);
    break;

  case RIGHT_SENSOR_TRIGGERED:

    digitalWrite(left_led,  LOW);
    digitalWrite(right_led, HIGH);
    break;

  case BOTH_SENSOR_TRIGGERED:

    digitalWrite(left_led,  HIGH);
    digitalWrite(right_led, HIGH);
    break;

  case NO_SENSOR_TRIGGERED:
    digitalWrite(left_led,  LOW);
    digitalWrite(right_led, LOW);
    break;

  default:
    break;
  }

  analogWrite(13, led);
  led = !led;
}
