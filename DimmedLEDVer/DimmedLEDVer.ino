#include <DS3231M.h>
#include <avr/power.h>
#include <avr/io.h>

DS3231M_Class DS3231M;

const int buttonPin = 2;

// Press durations
const unsigned long ledToggleTime = 500;
const unsigned long editModeTime  = 2000;
const unsigned long dstToggleTime = 8000;

// LED matrix pins
const int anode[]   = {0,1,17,3,4,5,6,7,8};
const int cathode[] = {9,10,11,12,13,14,15,16};
const int rows = sizeof(anode)/sizeof(anode[0]);
const int cols = sizeof(cathode)/sizeof(cathode[0]);

// ---------- PWM ----------
uint8_t pwmFrame = 0;
const uint8_t maxFrame = 3;

// Dim levels
const uint8_t blueDim        = 1;
const uint8_t redDimMinutes  = 1;
const uint8_t redDimSeconds  = 2;


// ---------- Edit mode ----------
enum EditMode { NONE, HOUR, MINUTE, SECOND };
EditMode currentMode = NONE;
EditMode lastMode    = NONE;

// Button
unsigned long buttonDownTime = 0;
bool buttonPressed = false;

// Debounce
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

// Blink edit
bool blinkState = false;
unsigned long lastBlinkTime = 0;
const unsigned long blinkInterval = 500;

// DST
bool dstActive = false;

// LEDs
bool ledsOn = true;

// Time cache
int lastHour, lastMinute, lastSecond;

// ---------- Setup ----------
void setup() {
    power_adc_disable();
    power_usart0_disable();
    power_timer1_disable();
    power_timer2_disable();
    power_spi_disable();

    for (int i=0;i<rows;i++) pinMode(anode[i], OUTPUT);
    for (int j=0;j<cols;j++) pinMode(cathode[j], OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);

    while (!DS3231M.begin()) {}
}

// ---------- Main Loop ----------
void loop() {
    handleButton();

    // 🔑 Leaving edit mode → HARD resync to RTC second
    if (lastMode != NONE && currentMode == NONE) {
        int s = DS3231M.now().second();
        while (DS3231M.now().second() == s) {
            // wait for next RTC second tick
        }

        pwmFrame = 0;
    }
    lastMode = currentMode;

    DateTime now = getAmsterdamTime();
    lastHour   = now.hour();
    lastMinute = now.minute();
    lastSecond = now.second();

    if (currentMode != NONE && millis() - lastBlinkTime > blinkInterval) {
        blinkState = !blinkState;
        lastBlinkTime = millis();
    }

    if (!ledsOn && currentMode == NONE) return;

    pwmFrame = (pwmFrame + 1) % maxFrame;

    // ---------- DISPLAY ----------
    if (currentMode != NONE) {
        turnOffAllLEDs();
        if (blinkState) {
            if (currentMode == HOUR)   lightHourLED(lastHour);
            if (currentMode == MINUTE) lightMinuteLED(lastMinute);
            if (currentMode == SECOND) lightSecondLED(lastSecond);
        }
        return;
    }

    // HOURS
    turnOffAllLEDs();
    if (pwmFrame < blueDim) lightHourLED(lastHour);
    delayMicroseconds(50);

    // MINUTES
    turnOffAllLEDs();
    if (pwmFrame < redDimMinutes) lightMinuteLED(lastMinute);
    delayMicroseconds(50);

    // SECONDS (unchanged behavior)
    turnOffAllLEDs();
    if (pwmFrame < redDimSeconds) {
        lightSecondLED(lastSecond);
        delayMicroseconds(20);
    }

    turnOffAllLEDs();
    delayMicroseconds(30);
}

// ---------- Time ----------
DateTime getAmsterdamTime() {
    DateTime now = DS3231M.now();
    int h = now.hour();
    if (dstActive) h = (h + 1) % 24;
    return DateTime(now.year(), now.month(), now.day(),
                    h, now.minute(), now.second());
}

// ---------- Button ----------
void handleButton() {
    bool reading = digitalRead(buttonPin);
    if (reading != lastButtonState) lastDebounceTime = millis();

    if (millis() - lastDebounceTime > debounceDelay) {
        if (reading == LOW && !buttonPressed) {
            buttonPressed = true;
            buttonDownTime = millis();
        } else if (reading == HIGH && buttonPressed) {
            buttonPressed = false;
            unsigned long d = millis() - buttonDownTime;

            if (d >= dstToggleTime && currentMode == NONE)
                dstActive = !dstActive;
            else if (d >= editModeTime) {
                if (currentMode == NONE) currentMode = HOUR;
                else if (currentMode == HOUR) currentMode = MINUTE;
                else if (currentMode == MINUTE) currentMode = SECOND;
                else currentMode = NONE;
            } else if (d >= ledToggleTime && currentMode == NONE)
                ledsOn = !ledsOn;
            else if (currentMode != NONE)
                incrementCurrentValue();
        }
    }
    lastButtonState = reading;
}

// ---------- Increment ----------
void incrementCurrentValue() {
    DateTime now = DS3231M.now();
    int h = now.hour(), m = now.minute(), s = now.second();
    if (currentMode == HOUR)   h = (h + 1) % 24;
    if (currentMode == MINUTE) m = (m + 1) % 60;
    if (currentMode == SECOND) s = (s + 1) % 60;
    DS3231M.adjust(DateTime(now.year(), now.month(), now.day(), h, m, s));
}

// ---------- LEDs ----------
void lightSecondLED(int i) {
    digitalWrite(anode[i / cols], HIGH);
    digitalWrite(cathode[i % cols], LOW);
}
void lightMinuteLED(int i) {
    digitalWrite(anode[i / cols], HIGH);
    digitalWrite(cathode[i % cols], LOW);
}
void lightHourLED(int i) {
    i %= 12;
    if (i < 4) {
        digitalWrite(7, HIGH); digitalWrite(8, LOW);
        digitalWrite(cathode[4 + i], LOW);
    } else {
        digitalWrite(8, HIGH); digitalWrite(7, LOW);
        digitalWrite(cathode[i - 4], LOW);
    }
}
void turnOffAllLEDs() {
    for (int i=0;i<rows;i++) digitalWrite(anode[i], LOW);
    for (int j=0;j<cols;j++) digitalWrite(cathode[j], HIGH);
}
