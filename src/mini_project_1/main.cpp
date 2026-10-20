#include <Arduino.h>

#define LIGHT_SENSOR 4
#define RELE_PIN 5
#define BUTTON 6
#define NIGHT_LIGHT_PIN 12
#define PWM_CH 0

constexpr uint16_t LED_ON_BELOW = 900;
constexpr uint16_t LED_OFF_ABOVE = 1600;
constexpr uint16_t NIGHT_ON_DURATION = 10000;
constexpr uint16_t NIGHT_ON_KEEP = 5000;
constexpr uint8_t PWM_ARR[] = {10, 20, 30, 40, 50, 40, 30, 20};
constexpr uint8_t PWM_MAX_INDEX = 8;

enum Mode {
    DAY,
    NIGHT_IDLE,
    NIGHT_ON
};

struct State
{
    Mode mode = Mode::DAY;
    unsigned long nightOnTimer = 0;
    unsigned long measureTime = 0;
    unsigned long idleLightTimer = 0;
    uint16_t lightSensorVal = 4095;
    bool isButtonPressed = false;
    uint8_t pwmIndex = 0;
};

State state = State();

void setBrightness(int percent);
void turnLightOff();
void setNightIdle(unsigned long time);

void setup() {
    Serial.begin(115200);

    // outputs
    pinMode(RELE_PIN, OUTPUT);
    ledcSetup(PWM_CH, 5000, 8);
    ledcAttachPin(NIGHT_LIGHT_PIN, PWM_CH);

    // inputs
    pinMode(BUTTON, INPUT_PULLUP);
    pinMode(LIGHT_SENSOR, INPUT);

    // set initial state
    turnLightOff();
}

void loop() {
    // track current time
    unsigned long currentTime = millis();

    // do all measurements
    if (currentTime - state.measureTime > 100) {
        state.isButtonPressed = digitalRead(BUTTON);
        state.lightSensorVal = analogRead(LIGHT_SENSOR);
        state.measureTime = currentTime;
        // Serial.printf("Light: %d\n, ButtonPressed: %d\n", state.lightSensorVal, state.isButtonPressed);
    }

    // set current mode
    if (state.mode == Mode::DAY)
    {
        // from day to night idle
        if (state.lightSensorVal <= LED_ON_BELOW) {
            setNightIdle(currentTime);
        }
    }
    // from night(any) to day
    else if (state.lightSensorVal >= LED_OFF_ABOVE)
    {
        state.mode = Mode::DAY;
    }
    // handle all night modes
    else
    {
        // from night idle to night on
        if (state.mode == Mode::NIGHT_IDLE)
        {   
            if (state.isButtonPressed) {
                state.mode = Mode::NIGHT_ON;
                state.nightOnTimer = currentTime;
            }
        }

        if (state.mode == Mode::NIGHT_ON) {
            // button is pressed again after 5sec on - keep current mode
            if (state.isButtonPressed && currentTime - state.nightOnTimer > NIGHT_ON_KEEP)
            {
                Serial.print("Keep current mode for 10sec more\n");
                state.nightOnTimer = currentTime;
            } else if (currentTime - state.nightOnTimer > NIGHT_ON_DURATION) {
                setNightIdle(currentTime);
            }
        }
    }

    switch(state.mode) {
        case Mode::DAY:
            turnLightOff();
            break;
        case Mode::NIGHT_IDLE:
            digitalWrite(RELE_PIN, LOW);
            if (currentTime - state.idleLightTimer > 100) {
                state.pwmIndex = (state.pwmIndex + 1) % PWM_MAX_INDEX;
                Serial.printf("PWM index: %d\n", state.pwmIndex);
                setBrightness(PWM_ARR[state.pwmIndex]);
                state.idleLightTimer = currentTime;
            }
            break;
        case Mode::NIGHT_ON:
            digitalWrite(RELE_PIN, HIGH);
            setBrightness(0);
            break;
    }
}

void setNightIdle(unsigned long time) {
    state.mode = Mode::NIGHT_IDLE;
    state.pwmIndex = 0;
    setBrightness(PWM_ARR[state.pwmIndex]);
    state.idleLightTimer = time;
}

void setBrightness(int percent) {
    ledcWrite(PWM_CH, 255 * percent / 100);
}

void turnLightOff() {
    digitalWrite(RELE_PIN, LOW);
    setBrightness(0);
}