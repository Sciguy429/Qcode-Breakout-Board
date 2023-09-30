#include "Debounce.h"

Debounce::Debounce(uint8_t pin) {
    //Save our pin
    this->pin = pin;

    //Call reset
    reset();
}

void Debounce::reset() {
    //Reset our vars
    currentButtonState = BS_RELEASED;
    lastState = false;
    lastToggleMillis = millis();
}

ButtonState Debounce::read() {
    //Switch off of our last state
    switch (currentButtonState) {
    case BS_RELEASED: 
        //Button was released on our last call
        if (waitForButton(BUTTON_HIGH, DEBOUNCE_TIME)) {
            currentButtonState = BS_HELD;
        }
        break;

    case BS_HELD:
        //Button was being held down on our last call
        // - Debounce time as elapsed, but the button has yet to be released by the user
        if (waitForButton(BUTTON_HIGH, LONG_PRESS_TIME)) {
            currentButtonState = BS_LONG_HELD;
        }
        else if (waitForButton(BUTTON_LOW, DEBOUNCE_TIME)) {
            //Set the output state
            currentButtonState = BS_PRESSED;
        }
        break;
    
    case BS_LONG_HELD:
        //Button was being held down on our last call
        // - Both debounce time and long press time have elapsed, but the button has not be released
        if (waitForButton(BUTTON_LOW, DEBOUNCE_TIME)) {
            //Set the output state
            currentButtonState = BS_LONG_PRESSED;
        }
        break;
    
    default:
        //Clear out the invalid state
        currentButtonState = BS_RELEASED;
        break;
    }

    //Return our state
    return currentButtonState;
}

bool Debounce::waitForButton(bool state, unsigned long time) {
    //Get current button state
    bool currentState = digitalRead(pin);
    
    //Get the current time
    unsigned long currentMillis = millis();

    //Update last toggle time if needed
    if (currentState != lastState) {
        lastState = currentState;
        lastToggleMillis = currentMillis;
    }

    //Return weather the button has been in state for long enough
    return (currentState == state) && (currentMillis - lastToggleMillis >= time);
}