/*
   LineController.h - Library for using digital line tracking sensors.
   Created by Daniel P. Poltronieri, December, 1, 2017.
   Released into the public domain.
 */

#ifndef LineController_h
#define LineController_h

#define NO_SENSOR_TRIGGERED 0
#define LEFT_SENSOR_TRIGGERED 1
#define RIGHT_SENSOR_TRIGGERED 2
#define BOTH_SENSOR_TRIGGERED 3

#include <Arduino.h>

/*
   This class was made to be inherited and contains the basic parameters to
   controll future builds.
 */
class LineController {
public:

  // A simple constructor, it is only called inside other constructors.
  // Constructor in the header to help with the inline.
  LineController(const uint8_t lpin,
                 const uint8_t rpin) :
    _left_pin {
    lpin
  },
  _right_pin { rpin } {}

  // This prevents instanciating a LineController and helps with inheritance
  virtual inline uint8_t check(void) = 0;

protected:

  uint8_t _left_pin, _right_pin; // Pin variables.
};

// /////////////////////////////////////////////////////////////////////////////
// ///////////////////// Line Checker ////////////////////////////////////////
// /////////////////////////////////////////////////////////////////////////
class LineChecker : public LineController {
public:

  LineChecker(const uint8_t lpin,
              const uint8_t rpin);

  uint8_t check(void);

protected:

  uint8_t _left_trigger, _right_trigger;
};

// /////////////////////////////////////////////////////////////////////////////
// ///////////////////// Line Runner /////////////////////////////////////////
// /////////////////////////////////////////////////////////////////////////
class LineRunner : public LineController {
public:

  LineRunner(const uint8_t lpin,
             const uint8_t rpin,
             const uint8_t sample_size);

  uint8_t check(void);

protected:

  uint8_t *_left_sampler, *_right_sampler; // Pinters to be used with malloc.
  uint8_t  _sample_size, _current_sample, _max_left_sample, _max_right_sample;
};

#endif // ifndef Morse_h
