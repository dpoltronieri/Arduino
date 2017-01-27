// #include <Arduino.h>
#include <HBridge.hpp>

uint8_t exampleController = 0;

// SimpleHBridge(forward_pin, backward_pin, enable_pin)
SimpleHBridge leftWheel  = SimpleHBridge(4, 5, 6);
SimpleHBridge rightWheel = SimpleHBridge(7, 8, 9);
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
    rightWheel.startMoveForward(5000);
    break;

  case 1:

    leftWheel.startMoveForward(5000);
    rightWheel.stopMove();
    break;

  case 2:
    rightWheel.startMoveForward(5000);
    leftWheel.startMoveForward(5000);
    break;

  case 3:
    leftWheel.startMoveBackward(5000);
    rightWheel.startMoveBackward(5000);
    break;

  default:
    leftWheel.startMoveBackward(5000);
    rightWheel.startMoveBackward(5000);
    break;
  }

  rightWheel.move();
  leftWheel.move();

  analogWrite(13, led);
  led = !led;
}
