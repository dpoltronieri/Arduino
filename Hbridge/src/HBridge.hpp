/*
   HBridge.hpp - Library for controlling L293D IC and boards based on it.
   Created by Daniel P. Poltronieri, December, 1, 2017.
   Released into the public domain.
 */

#ifndef HBridge_h
#define HBridge_h

#define MOVE_FORWARD 1
#define MOVE_BACKWARD 0
#define NOT_MOVE 2

#include <Arduino.h>

/*
   This class was made to be inherited and contains the basic parameters to
   controll future builds.
 */

class HBridge {
public:

  // A simple constructor, it is only called inside other constructors.
  // Constructor in the header to help with the inline.
  HBridge(const uint8_t fpin,
          const uint8_t bpin,
          const uint8_t epin) :
    _forward_pin {
    fpin
  },
  _backward_pin { bpin },
  _enable_pin { epin }
  {
    pinMode(_enable_pin,   OUTPUT);
    pinMode(_forward_pin,  OUTPUT);
    pinMode(_backward_pin, OUTPUT);
  }

  // This prevents instanciating a HBridge and helps with inheritance
  virtual void move()     = 0;
  virtual void stopMove() = 0;

protected:

  uint8_t _forward_pin, _backward_pin, _enable_pin;    // Pin variables
  size_t  _previous_time, _current_time, _target_time; // Unocupied wait
                                                       // variables
  uint8_t _current_direction, _target_direction;       // Control variables
};

/*
   This is the most simple control algorythm, and also the less
   resource intensive. Start here and change to AnalogHBridge if needed.
 */

// //////////////////////////////////////////////////////////////////////////
// /////////////////////// SimpleHBridge ///////////////////////////////////
// ////////////////////////////////////////////////////////////////////////

class SimpleHBridge : public HBridge {
public:

  SimpleHBridge(const uint8_t fpin,
                const uint8_t bpin,
                const uint8_t epin);

  /*
     A function that accepts bool, avoid its usage, but its made to be used
        instead
     of if-else logic.
   */
  void startMove(const size_t time,
                 const bool   direction);

  // The motor will move forward until the defined time, in miliseconds runs out
  // or the direction is changed
  void startMoveForward(const size_t ttime);

  // The motor will move backward until the defined time, in miliseconds runs
  // out
  // or the direction is changed
  void startMoveBackward(const size_t ttime);

  // The motor will stop moving and be locked in place
  void stopMove(void);

  // This is the control function, it shoud be caled inside the main loop
  void move(void);
};

// //////////////////////////////////////////////////////////////////////////
// /////////////////////// AnalogHBridge ///////////////////////////////////
// ////////////////////////////////////////////////////////////////////////
class AnalogHBridge : public HBridge {
public:

  /*
     The constructor for this function now accepts delay_time,
     witch is the time increment, in miliseconds, between any change in speed
   */
  AnalogHBridge(const uint8_t fpin,
                const uint8_t bpin,
                const uint8_t epin,
                const size_t  delay_time);

  /*
     The motor will move acording to the direction until the defined time, in
     miliseconds runs out, or the direction is changed, increaseing or
        decreasing
     the speed until it reaches the target speed.
     The speed value uses an AnalogWrite, so it is set between 0 - 255.
   */

  void startMove(const uint8_t speed,
                 const size_t  time,
                 const bool    direction);

  /*
     The motor will move forward until the defined time, in miliseconds runs out
     or the direction is changed, increaseing or decreasing the speed until it
     reaches the target speed.
     The speed value uses an AnalogWrite, so it is set between 0 - 255.
   */
  void startMoveForward(const uint8_t speed,
                        const size_t  ttime);

  /*
     The motor will move forward until the defined time, in miliseconds runs out
     or the direction is changed, increaseing or decreasing the speed until it
     reaches the target speed.
     The speed value uses an AnalogWrite, so it is set between 0 - 255.
   */
  void startMoveBackward(const uint8_t speed,
                         const size_t  ttime);

  // This is the control function, it shoud be caled inside the main loop
  void move(void);

  // Differently from the move(void), this one doesn't smooth the speed,
  // changing instantaneously the AnalogWrite value.
  void moveNow(void);

  // This function releases the power of the motor driver,
  // allowing the motor to stop on its own.
  void stopMove(void);

  // This function locks the motor rotation, causing an abrupt stop.
  void stopMoveNow(void);

protected:

  uint8_t _target_speed, _current_speed; // Speed smoothing variables.
  size_t  _delay_time;                   // Time between speed increments or
                                         // decrements.
};

#endif // ifndef Morse_h
