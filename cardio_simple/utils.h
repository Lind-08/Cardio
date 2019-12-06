#include <Arduino.h>

void blink(uint8_t pin, uint8_t init_state)
{
  digitalWrite(pin, !init_state);
  delay(500);
  digitalWrite(pin, init_state);
}