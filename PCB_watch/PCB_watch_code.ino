#include <DS3231M.h>
#include <avr/power.h>
#include <avr/io.h>

DS3231M_Class DS3231M;

const int buttonPin = 2;

// Press durations
const unsigned long ledToggleTime   = 500;   // 0.5s for LED toggle
const unsigned long editModeTime    = 2000;  // 2s to enter/cycle edit mode
const unsigned long dstToggleTime   = 8000;  // 8s for DST toggle

// LED matrix pins
const int anode[]   = {0,1,17,3,4,5,6,7,8};
const int cathode[] = {9,10,11,12,13,14,15,16};
const int rows = sizeof(anode)/sizeof(anode[0]);
const int cols = sizeof(cathode)/sizeof(cathode[0]);

enum EditMode { NONE, HOUR, MINUTE, SECOND };
EditMode currentMode = NONE;

unsigned long buttonDownTime = 0;
bool buttonPressed = false;

// Software debounce
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

// Blink for edit mode
bool blinkState = false;
unsigned long lastBlinkTime = 0;
const unsigned long blinkInterval = 500;

// DST toggle
bool dstActive = false; // false = winter, true = summer

// LED display state
bool ledsOn = true;

// Last displayed time for restore
int lastHour = 0;
int lastMinute = 0;
int lastSecond = 0;

// ---------- Setup ----------
void setup() {
    power_adc_disable();
    power_usart0_disable();
    power_timer1_disable();
    power_timer2_disable();
    power_spi_disable();

    for (int i=0; i<rows; i++) pinMode(anode[i], OUTPUT);
    for (int j=0; j<cols; j++) pinMode(cathode[j], OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);

    while(!DS3231M.begin()) {}

    // Safeguard: ensure currentMode starts as NONE
    if (currentMode != NONE && currentMode != HOUR && currentMode != MINUTE && currentMode != SECOND) {
        currentMode = NONE;
    }

    // Optionally set an initial time if RTC lost power
    DateTime now = DS3231M.now();
    if (now.year() < 2020) {
        DS3231M.adjust(DateTime(2025, 12, 10, 24, 13, 0));
        currentMode = HOUR; // enter edit mode automatically
    }
}

// ---------- Main Loop ----------
void loop() {
    handleButton();

    DateTime now = getAmsterdamTime();

    // Save current time for restoring LEDs
    lastHour   = now.hour();
    lastMinute = now.minute();
    lastSecond = now.second();

    // Handle edit mode blinking
    if (currentMode != NONE) {
        if (millis() - lastBlinkTime >= blinkInterval) {
            blinkState = !blinkState;
            lastBlinkTime = millis();
        }
    }

    // Skip normal display if LEDs are off and not in edit mode
    if (!ledsOn && currentMode == NONE) return;

    // Edit mode display
    if (currentMode != NONE) {
        turnOffAllLEDs();
        if (blinkState) {
            switch(currentMode){
                case HOUR:   lightHourLED(lastHour); break;
                case MINUTE: lightMinuteLED(lastMinute); break;
                case SECOND: lightSecondLED(lastSecond); break;
            }
        }
    } else {
        // Normal display cycle using last saved time
        turnOffAllLEDs();
        lightSecondLED(lastSecond);
        delayMicroseconds(100);

        turnOffAllLEDs();
        lightMinuteLED(lastMinute);
        delayMicroseconds(100);

        turnOffAllLEDs();
        lightHourLED(lastHour);
        delayMicroseconds(100);
    }
}

// ---------- Amsterdam Time with DST Toggle ----------
DateTime getAmsterdamTime() {
    DateTime now = DS3231M.now();
    int hour = now.hour();
    if (dstActive) hour = (hour + 1) % 24;
    return DateTime(now.year(), now.month(), now.day(), hour, now.minute(), now.second());
}

// ---------- Button Handling ----------
void handleButton() {
    bool reading = digitalRead(buttonPin);

    if (reading != lastButtonState) lastDebounceTime = millis();

    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading == LOW && !buttonPressed) {
            buttonPressed = true;
            buttonDownTime = millis(); // record press start
        } 
        else if (reading == HIGH && buttonPressed) {
            unsigned long pressDuration = millis() - buttonDownTime;
            buttonPressed = false;

            if (pressDuration >= dstToggleTime && currentMode == NONE) {
                // 8s → DST toggle only in normal mode
                dstActive = !dstActive;
            } 
            else if (pressDuration >= editModeTime) {
                // 2s → cycle edit mode
                switch (currentMode) {
                    case NONE:   currentMode = HOUR; break;
                    case HOUR:   currentMode = MINUTE; break;
                    case MINUTE: currentMode = SECOND; break;
                    case SECOND: currentMode = NONE; break;
                }
            } 
            else if (pressDuration >= ledToggleTime && currentMode == NONE) {
                // 0.5s → toggle LEDs only in normal mode
                ledsOn = !ledsOn;
                if (!ledsOn) turnOffAllLEDs();
            } 
            else if (pressDuration < ledToggleTime && currentMode != NONE) {
                // Short press increments value in edit mode
                incrementCurrentValue();
            }
        }
    }

    lastButtonState = reading;
}

// ---------- Increment Edit Mode ----------
void incrementCurrentValue() {
    DateTime now = DS3231M.now();
    int hour = now.hour();
    int minute = now.minute();
    int second = now.second();

    switch(currentMode) {
        case HOUR:   hour = (hour + 1) % 24; break;
        case MINUTE: minute = (minute + 1) % 60; break;
        case SECOND: second = (second + 1) % 60; break;
        case NONE:   return;
    }

    DS3231M.adjust(DateTime(now.year(), now.month(), now.day(), hour, minute, second));
}

// ---------- LED Control ----------
void lightSecondLED(int index) {
    int row = index / cols;
    int col = index % cols;
    digitalWrite(anode[row], HIGH);
    digitalWrite(cathode[col], LOW);
}

void lightMinuteLED(int index) {
    int row = index / cols;
    int col = index % cols;
    digitalWrite(anode[row], HIGH);
    digitalWrite(cathode[col], LOW);
}

void lightHourLED(int index) {
    index %= 12; // 12-hour format
    if (index < 4) {
        digitalWrite(7,HIGH); digitalWrite(8,LOW);
        digitalWrite(cathode[4+index], LOW);
    } else {
        digitalWrite(8,HIGH); digitalWrite(7,LOW);
        digitalWrite(cathode[index-4], LOW);
    }
}

void turnOffAllLEDs() {
    for (int i=0; i<rows; i++) digitalWrite(anode[i], LOW);
    for (int j=0; j<cols; j++) digitalWrite(cathode[j], HIGH);
}
