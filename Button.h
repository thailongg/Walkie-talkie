#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
  int pin;
  volatile bool pressed;

  static void IRAM_ATTR isrRouter(void* arg);

public:
  Button(int p);
  bool isPressed();
};

#endif
