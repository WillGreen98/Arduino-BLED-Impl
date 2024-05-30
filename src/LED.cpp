#include "LED.h"

LED::LED(uint8_t pinNumber) noexcept
    : pin(pinNumber), isBlinking(false), blinkInterval(1000), previousMillis(0) {
    pinMode(pin, OUTPUT);
}

void LED::toggleBlinking() noexcept {
    isBlinking = !isBlinking;
}

bool LED::isBlinkingLED() const noexcept {
    return isBlinking;
}