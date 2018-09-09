// #include <Arduino.h>
#include <HBridge.hpp>

uint8_t exampleController = 0;

//AnalogHBridge(forward_pin, backward_pin, enable_pin, delay_time)
AnalogHBridge leftWheel  = AnalogHBridge(4, 5, 6, 10);
AnalogHBridge rightWheel = AnalogHBridge(7, 8, 9, 10);
bool led                 = true;


void setup() {
  Serial.begin(9600);
}

void loop()  {
  if (Serial.available() > 0) {
    exampleController = Serial.read();
  }

  switch (exampleController) {
  case 0:

    leftWheel.stopMove();
    rightWheel.startMoveForward(150, 5000);
    break;

  case 1:

    leftWheel.startMoveForward(150, 5000);
    rightWheel.stopMove();
    break;

  case 2:
    rightWheel.startMoveForward(150, 5000);
    leftWheel.startMoveForward(150, 5000);
    break;

  case 3:
    leftWheel.startMoveBackward(150, 5000);
    rightWheel.startMoveBackward(150, 5000);
    break;

  default:
    leftWheel.startMoveBackward(150, 5000);
    rightWheel.startMoveBackward(150, 5000);
    break;
  }

  rightWheel.move();
  leftWheel.move();

  analogWrite(13, led);
  led = !led;
}
