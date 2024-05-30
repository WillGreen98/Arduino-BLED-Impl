#pragma once

#include <Arduino.h>

class LED {
    protected:
        const uint8_t pin;
        bool isBlinking;
        uint16_t blinkInterval;
        unsigned long previousMillis;

    public:
        explicit LED(uint8_t pinNumber) noexcept;
        virtual ~LED() = default;
        virtual void toggleBlinking() noexcept;
        virtual void blinkLED() noexcept = 0;
        bool isBlinkingLED() const noexcept;
};