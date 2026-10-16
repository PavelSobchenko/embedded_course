#include <Arduino.h>

#define LED_PIN 5
#define LIGHT_RES 7

unsigned long last_time = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LIGHT_RES, INPUT);
}

void loop() {
    unsigned long currentTime = millis();
    // int lightVal = analogRead(LIGHT_RES);
    // Serial.printf("Light %d\n", analogRead(LIGHT_RES));
    // digitalWrite(LED_PIN, lightVal > 800 ? HIGH : LOW);
    // delay(1000);
    if (currentTime - last_time > 100) {
        int rawVal = analogRead(LIGHT_RES);
        float vComputed = (rawVal / 4095.0f) * 3100.0f;
        int vFromAnalogRead = analogReadMilliVolts(LIGHT_RES);

        float absError = fabs(vComputed - (float)vFromAnalogRead);
        float relError = vFromAnalogRead > 0 ? absError / vFromAnalogRead * 100.0f : 0.0f;

        Serial.printf("rawVal: %d; vComputed: %.0f; vFromAnalogRead: %lu; abs: %.0f; rel: %.2f\n", rawVal, vComputed, vFromAnalogRead, absError, relError);

        last_time = currentTime;
    }
}