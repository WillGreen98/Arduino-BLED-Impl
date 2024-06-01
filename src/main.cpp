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
    if(buttonState == HIGH) {
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
    static bool errorPrinted = false; // Flag to track if error has been printed
    static bool blinkingStarted = false; // Flag to track if blinking started message has been printed

    auto buttonError = readButtonState();

    // Print error message only if there's an error and it hasn't been printed before
    if (buttonError != ErrorCode::None && !errorPrinted) {
        Serial.print(F("Error reading button state: "));
        Serial.println(static_cast<int>(buttonError));
        errorPrinted = true; // Set flag to true after printing the error
    }

    // TODO: fix toggle logic
    switch (currentState) {
        case LEDState::Idle:
            if (buttonPressed) {
                currentState = LEDState::Blinking;
                builtInLED.toggleBlinking(); // Start blinking
                if (!blinkingStarted) {
                    Serial.println(F("LED blinking started."));
                    blinkingStarted = true;
                }

                buttonPressed = false;
                errorPrinted = false; // Reset error flag when button is pressed again
            }
            break;
        case LEDState::Blinking:
            builtInLED.update(); // Update LED state
            if (buttonPressed) {
                currentState = LEDState::Idle;
                builtInLED.toggleBlinking(); // Stop blinking
                Serial.println(F("LED blinking stopped."));
                buttonPressed = false;
                blinkingStarted = false; // Reset blinking started flag when button is pressed again
                errorPrinted = false; // Reset error flag when button is pressed again
            }

            break;
        default:
            break;
    }

    wdt_reset();
}