#include <Arduino.h>
#include "Button.h"

Button::Button(int p) : pin(p), pressed(false) {
  pinMode(pin, INPUT_PULLUP);
  attachInterruptArg(digitalPinToInterrupt(pin), isrRouter, this, FALLING);
}

void IRAM_ATTR Button::isrRouter(void* arg) {
  Button* btn = static_cast<Button*>(arg);
  btn->pressed = true;
}

bool Button::isPressed() {
  
  if (pressed) {
    Serial.println("Button pressed");
    pressed = false;
    return true;
  }else {
    Serial.println("Button unpressed");
    return false;
  }
}
