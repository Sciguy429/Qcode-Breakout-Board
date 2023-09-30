//## Button Debounce Handler ##
// - Handles debouncing and long press support for the 4 face push buttons
// - pinMode must still be called on the desired pin

#pragma once

#include "Arduino.h"

//## Delay Macros ##

//Debounce time in milliseconds
#define DEBOUNCE_TIME 40

//Jitter time in milliseconds
#define JITTER_TIME 20

//Long press time in milliseconds
#define LONG_PRESS_TIME 2000

//Button trigger voltage
// - We are using pullups, so buttons are active LOW not HIGH
#define BUTTON_HIGH LOW
#define BUTTON_LOW !BUTTON_HIGH

//Button state enum
enum ButtonState: uint8_t {
    //Button released
    BS_RELEASED = 0,

    //Button pressed
    BS_PRESSED = 1,
    
    //Button held
    BS_HELD = 2,

    //Button long pressed
    BS_LONG_PRESSED = 3,

    //Button long press held
    BS_LONG_HELD = 4,
};

//Button debounce class
class Debounce {
public:
    //Constructor
    Debounce(uint8_t pin);

    //Reset the debounce state
    void reset();

    //Read the button state
    ButtonState read();

//Private functions
private:
    //Wait for the button to be HIGH or LOW for a specified length of time
    bool waitForButton(bool state, unsigned long time);

//Local vars
private:
    //Button pin
    uint8_t pin;

    //Current button state
    ButtonState currentButtonState;

    //Last button state
    bool lastState;

    //Last toggle millis
    // - Updated any time the button toggles state
    unsigned long lastToggleMillis;
};