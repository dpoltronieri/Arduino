/*
 * LineController.h - Library for using digital line tracking sensors.
 * Created by Daniel P. Poltronieri, December, 1, 2017.
 * Released into the public domain.
 */

#include <Arduino.h>
#include "LineController.hpp"

/*
 * Template function to get the most frequent value in an array,
 * used here on the running chacker, LineRunner, to smooth out reading errors.
 */
template <typename T, typename U>
T mostFrequent(T * valores, U tamanho){
    T max   = 0;
    U count = 0;
    T maxValue;

    for (int i; i <= tamanho; i++) {
        count = 0;

        // Counts the number of uccurances in the array
        for (int j = 0; j < tamanho; j++) {
            if (valores[i] == valores[j]) count++;
        }

        // If it is the new maximum, store its value
        if (count > max) {
            max      = count;
            maxValue = valores[i];
        }
    }

    return maxValue;
}

// /////////////////////////////////////////////////////////////////////////////
// ///////////////////// Line Checker ////////////////////////////////////////
// /////////////////////////////////////////////////////////////////////////

// A simple constructor, that calls the parent constructor and defines the
// relevant pins as inputs.
LineChecker::LineChecker(const uint8_t lpin,
  const uint8_t                        rpin)
    : LineController(lpin, rpin){
    pinMode(_left_pin, INPUT);
    pinMode(_right_pin, INPUT);
}

// Reads the pins and returns the combined state.
uint8_t LineChecker::check(void){
    _left_trigger  = digitalRead(_left_pin);
    _right_trigger = digitalRead(_right_pin);

    if ((_left_trigger == HIGH) && (_right_trigger == HIGH)) {
        return NO_SENSOR_TRIGGERED;
    } else if ((_left_trigger == HIGH) && (_right_trigger == LOW)) {
        return LEFT_SENSOR_TRIGGERED;
    } else if ((_right_trigger == HIGH) && (_left_trigger == LOW)) {
        return RIGHT_SENSOR_TRIGGERED;
    } else if ((_right_trigger == LOW) && (_left_trigger == LOW)) {
        return BOTH_SENSOR_TRIGGERED;
    }
}

// /////////////////////////////////////////////////////////////////////////////
// ///////////////////// Line Runner /////////////////////////////////////////
// /////////////////////////////////////////////////////////////////////////

/*
 * The constructor for the running reader creates a dinamically allocated vector
 *    for each sensor to store the last values read.
 */
LineRunner::LineRunner(const uint8_t lpin,
  const uint8_t                      rpin,
  const uint8_t                      sample_size)
    : LineController(lpin, rpin){
    pinMode(_left_pin, INPUT);
    pinMode(_right_pin, INPUT);

    // malloc(sample_size * 8) can be used here because uint8_t guarantees the
    // size.
    _left_sampler  = (uint8_t *) malloc(sample_size * 8);
    _right_sampler = (uint8_t *) malloc(sample_size * 8);
    _sample_size   = sample_size;
}

uint8_t LineRunner::check(void){
    _left_sampler[_current_sample]  = digitalRead(_left_pin);
    _right_sampler[_current_sample] = digitalRead(_right_pin);

    // This is used for circular vector math.
    _current_sample++;
    _current_sample = _current_sample % _sample_size;

    // Uses the most common value the same as the LineChecker::check();
    _max_left_sample  = mostFrequent(_left_sampler, _sample_size);
    _max_right_sample = mostFrequent(_right_sampler, _sample_size);

    if ((_max_left_sample == HIGH) && (_max_right_sample == HIGH)) {
        return NO_SENSOR_TRIGGERED;
    } else if ((_max_left_sample == HIGH) && (_max_right_sample == LOW)) {
        return LEFT_SENSOR_TRIGGERED;
    } else if ((_max_right_sample == HIGH) && (_max_left_sample == LOW)) {
        return RIGHT_SENSOR_TRIGGERED;
    } else if ((_max_right_sample == LOW) && (_max_left_sample == LOW)) {
        return BOTH_SENSOR_TRIGGERED;
    }
}
