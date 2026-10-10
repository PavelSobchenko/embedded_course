#include <Arduino.h>

#define RED_LED 47
#define BLUE_LED 48
#define YELLOW_LED 45
#define BASIC_TASK false
#define COMPLEX_PATTERN false
#define THREE_LEDS false
#define ONE_PIN true

void complexPattern(int pin);

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
}

void loop() {
  #if BASIC_TASK
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BLUE_LED, LOW);
    delay(500);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, HIGH);
    delay(500);
  #endif

  #if COMPLEX_PATTERN
    complexPattern(RED_LED);
    complexPattern(BLUE_LED);
  #endif

  #if THREE_LEDS
    complexPattern(RED_LED);
    complexPattern(BLUE_LED);
    complexPattern(YELLOW_LED);
  #endif

  #if ONE_PIN
    digitalWrite(RED_LED, HIGH);
    delay(500);
    digitalWrite(RED_LED, LOW);
    delay(500);
  #endif
}

void complexPattern(int pin) {
  for (int i = 0; i < 5; i++) {
    digitalWrite(pin, HIGH);
    delay(i * 10);
    digitalWrite(pin, LOW);
    delay(i * 10);
  }
}