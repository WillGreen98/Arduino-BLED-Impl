#include "Embedded.h"

volatile bool buttonPressed = false;
unsigned long lastDebounceTime = 0;
LEDState currentState = LEDState::Idle;
BuiltInLED builtInLED;

void buttonISR() {
    buttonPressed = true;
}

bool readDebouncedButtonState() noexcept {
    bool rawButtonState = digitalRead(BUTTON_PIN);
    if (rawButtonState != buttonPressed && millis() - lastDebounceTime > DEBOUNCE_DELAY) {
        lastDebounceTime = millis();
        buttonPressed = rawButtonState;
    }
    return buttonPressed;
}

ErrorCode readButtonState() noexcept {
    int buttonState = digitalRead(BUTTON_PIN);
    if (buttonState == HIGH) {
        return ErrorCode::ButtonRead;
    }
    return ErrorCode::None;
}

void setup() {
    Serial.begin(9600);
    while (!Serial);

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);

    wdt_enable(WDTO_2S);
}

void loop() {
    auto buttonError = readButtonState();
    if (buttonError == ErrorCode::ButtonRead) {
        Serial.print(F("Error reading button state: "));
        Serial.println(static_cast<int>(buttonError));
    }

    switch (currentState) {
        case LEDState::Idle:
            if (buttonPressed) {
                currentState = LEDState::Blinking;
                Serial.println(F("LED blinking started."));
                buttonPressed = false;
            }
            break;
        case LEDState::Blinking:
            builtInLED.blinkLEDTemp();
            if (buttonPressed) {
                currentState = LEDState::Idle;
                Serial.println(F("LED blinking stopped."));
                buttonPressed = false;
                digitalWrite(LED_PIN, LOW); // Ensure the LED is turned off when stopping
            }
            break;
        default:
            break;
    }

    wdt_reset();
}