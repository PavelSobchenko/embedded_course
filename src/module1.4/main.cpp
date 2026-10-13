#include <Arduino.h>

#define LED_RED 16
#define LED_BLUE 15
#define EXTERNAL_BTN 21
#define INTERNAL_BOOT 0

bool mode1 = true;
unsigned long pressedBtnTime = 0;
unsigned long mode_time = 0;
int lastPressedBtn = 0; // 0 - no pressed any btn, 1 - external, 2 - internal

void setMode1Light(int current);
void startRailwayCrossMode();

void setup() {
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(EXTERNAL_BTN, INPUT_PULLUP);
    pinMode(INTERNAL_BOOT, INPUT_PULLUP);

    // initial turn on led 
    setMode1Light(HIGH);
}

void loop() {
    // read all data first
    unsigned long currTime = millis();
    bool external_btn_pressed = !digitalRead(EXTERNAL_BTN);
    bool internal_btn_pressed = !digitalRead(INTERNAL_BOOT);

    // check for pressed conditions and set mode 
    if (external_btn_pressed) {
        if (lastPressedBtn != 1) {
            pressedBtnTime = currTime;
            lastPressedBtn = 1;
        } else if (currTime - pressedBtnTime > 20 && !mode1) {
            mode1 = true;
            mode_time = currTime;
            setMode1Light(HIGH);
        }
    } else if (internal_btn_pressed) {
        if (lastPressedBtn != 2) {
            pressedBtnTime = currTime;
            lastPressedBtn = 2;
        } else if (currTime - pressedBtnTime > 20 && mode1) {
            mode1 = false;
            mode_time = currTime;
            startRailwayCrossMode();
        }
    } else {
        lastPressedBtn = 0;
    }

    if (mode1) {
        if (currTime - mode_time >= 200) {
            int newVal = !digitalRead(LED_RED);
            setMode1Light(newVal);
            mode_time = currTime;
        }
    } else {
        if (currTime - mode_time >= 1000) {
            digitalWrite(LED_RED, digitalRead(LED_RED) ? LOW : HIGH);
            digitalWrite(LED_BLUE, digitalRead(LED_BLUE) ? LOW : HIGH);
            mode_time = currTime;
        }
    }
    
}

void setMode1Light(int current) {
    digitalWrite(LED_RED, current);
    digitalWrite(LED_BLUE, current);
}

void startRailwayCrossMode() {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_BLUE, LOW);
}