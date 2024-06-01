#pragma once

#include "LED.h"

class BuiltInLED : public LED {
    public:
        BuiltInLED() noexcept;
        virtual ~BuiltInLED() = default;
        void blinkLED() noexcept;
        void blinkLEDTemp() noexcept;
        void update() noexcept;
    private:
        void toggleLEDState() noexcept;
};