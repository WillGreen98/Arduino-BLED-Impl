#include "BuiltInLED.h"
#include "Utils.h"

BuiltInLED::BuiltInLED() noexcept : LED(LED_PIN) {}

// TODO: fix this logic - Maybe use bit flipping instead.
void BuiltInLED::blinkLED() noexcept {
    unsigned long currentMillis = millis();
    if (isBlinking && currentMillis - previousMillis >= blinkInterval) {
        previousMillis = currentMillis;
        digitalWrite(pin, !digitalRead(pin)); // Toggle the LED state
    }
}

void BuiltInLED::blinkLEDTemp() noexcept {
    // Turn the LED on
    digitalWrite(pin, HIGH);
    delay(blinkInterval);
    // Turn the LED off
    digitalWrite(pin, LOW);
    delay(blinkInterval);
}