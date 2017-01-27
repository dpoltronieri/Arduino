/*
   HBridge.cpp - Library for controlling L293D IC and boards based on it.
   Created by Daniel P. Poltronieri, December, 1, 2017.
   Released into the public domain.
 */

 #include <Arduino.h>
 #include "HBridge.hpp"

// //////////////////////////////////////////////////////////////////////////
// /////////////////////// SimpleHBridge ///////////////////////////////////
// ////////////////////////////////////////////////////////////////////////

// since this is a simple constructor, it can simply inline the parent
// contructor
SimpleHBridge::SimpleHBridge(const uint8_t fpin,
                             const uint8_t bpin,
                             const uint8_t epin) :
  HBridge(fpin, bpin, epin)
{}

/*
   The functions between here and SimpleHBridge::stopMove(void) only set
      variables
   to be used there, with ttime being the target time.
 */
void SimpleHBridge::startMove(const size_t ttime,
                              const bool   direction) {
  _target_time      = ttime;
  _target_direction = direction;
  _previous_time    = millis();
}

void SimpleHBridge::startMoveForward(const size_t ttime) {
  _target_time      = ttime;
  _target_direction = MOVE_FORWARD;
  _previous_time    = millis();
}

void SimpleHBridge::startMoveBackward(const size_t ttime) {
  _target_time      = ttime;
  _target_direction = MOVE_BACKWARD;
  _previous_time    = millis();
}

// The HBridge locks the motor in place
void SimpleHBridge::stopMove(void) {
  _target_direction = NOT_MOVE;

  _previous_time = millis();
  _target_time   = 200;
        digitalWrite(_enable_pin,   LOW);
        digitalWrite(_forward_pin,  LOW);
        digitalWrite(_backward_pin, LOW);
}

/*
   This function changes the direction or stops the motor depending of the
      variables
 */
void SimpleHBridge::move(void) {
  _current_time = millis();

  if (_current_time - _previous_time <= _target_time) { // Unoccupied wait
    // Checks to avoid multiple executions
    if ((_target_direction != _current_direction)) {
      _current_direction = _target_direction;

      if (_current_direction == MOVE_FORWARD) {
        digitalWrite(_forward_pin,  HIGH);
        digitalWrite(_backward_pin, LOW);
        digitalWrite(_enable_pin,   HIGH);
      } else if (_current_direction == MOVE_BACKWARD) {
        digitalWrite(_forward_pin,  LOW);
        digitalWrite(_backward_pin, HIGH);
        digitalWrite(_enable_pin,   HIGH);
      }
    }

    // When the timer runs out.
  } else {
    SimpleHBridge::stopMove();
  }
}

// //////////////////////////////////////////////////////////////////////////
// /////////////////////// AnalogHBridge ///////////////////////////////////
// ////////////////////////////////////////////////////////////////////////

// The parent contructor and set the _delay_time
AnalogHBridge::AnalogHBridge(const uint8_t fpin,
                             const uint8_t bpin,
                             const uint8_t epin,
                             const size_t  delay_time) :
  HBridge(fpin, bpin, epin),
  _delay_time {
  delay_time
}

{}

/*
   The functions between here and AnslogHBridge::stopMove(void) only set
      variables
   to be used there, with ttime being the target time.
 */
void AnalogHBridge::startMove(const uint8_t speed,
                              const size_t  ttime,
                              const bool    direction) {
  _target_speed     = speed;
  _target_time      = ttime;
  _target_direction = direction;
  _previous_time    = millis();
}

void AnalogHBridge::startMoveForward(const uint8_t speed,
                                     const size_t  ttime) {
  _target_speed     = speed;
  _target_time      = ttime;
  _target_direction = MOVE_FORWARD;
  _previous_time    = millis();
}

void AnalogHBridge::startMoveBackward(const uint8_t speed,
                                      const size_t  ttime) {
  _target_speed     = speed;
  _target_time      = ttime;
  _target_direction = MOVE_BACKWARD;
  _previous_time    = millis();
}

// This function releases control of the motor, allowing it to stop on its own.
void AnalogHBridge::stopMove(void) {
  _target_direction = NOT_MOVE;
  _current_speed    = 0;

  _previous_time = millis();
  _target_time   = 200;
        digitalWrite(_enable_pin,   0);
        digitalWrite(_forward_pin,  0);
        digitalWrite(_backward_pin, 0);
}

// This function locks the motor in place.
void AnalogHBridge::stopMoveNow(void) {
  _target_direction = NOT_MOVE;
  _current_speed    = 0;
  _previous_time    = millis();
  _target_time      = 200;
        digitalWrite(_forward_pin,  0);
        digitalWrite(_backward_pin, 0);

  // This delay is used to guarantee the motor has stopped before releasing
  // control.
  delay(_delay_time * 10 + 50);
        digitalWrite(_enable_pin, 0);
}

// This function changes the direction or stops the motor depending on the
// variables.
void AnalogHBridge::move(void) {
  _current_time = millis();

  if (_current_time - _previous_time <= _target_time) { // Unoccupied wait
    // Checks to avoid multiple executions
    if ((_target_direction != _current_direction)) {
      _current_direction = _target_direction;

      if (_current_direction == MOVE_FORWARD) {
        digitalWrite(_forward_pin,  HIGH);
        digitalWrite(_backward_pin, LOW);
      } else if (_current_direction == MOVE_BACKWARD) {
        digitalWrite(_forward_pin,  LOW);
        digitalWrite(_backward_pin, HIGH);
      }
    }

    // Speed smoothing, analogWrite is called inside to call it the minimum
    // ammount possible.
    if (_current_speed < _target_speed) {
      _current_speed++;
      analogWrite(_enable_pin, _current_speed);
    } else if (_current_speed > _target_speed) {
      _current_speed--;
      analogWrite(_enable_pin, _current_speed);
    }

    delay(_delay_time); // Small ocupied time
    // When the timmer runs out.
  } else {
    AnalogHBridge::stopMove();
  }
}

void AnalogHBridge::moveNow(void) {
  _current_time = millis();

  if (_current_time - _previous_time <= _target_time) { // Unoccupied wait
    // Checks to avoid multiple executions
    if ((_target_direction != _current_direction)) {
      _current_direction = _target_direction;

      if (_current_direction == MOVE_FORWARD) {
        digitalWrite(_forward_pin,  HIGH);
        digitalWrite(_backward_pin, LOW);
      } else if (_current_direction == MOVE_BACKWARD) {
        digitalWrite(_forward_pin,  LOW);
        digitalWrite(_backward_pin, HIGH);
      }
    }

    // Checks to avoid multiple executions
    if (_current_speed != _target_speed) {
      _current_speed = _target_speed;
      analogWrite(_enable_pin, _current_speed);
    }

    // When the timmer runs out.
  } else {
    AnalogHBridge::stopMove();
  }
}
