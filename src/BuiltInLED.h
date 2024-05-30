#pragma once

#include "LED.h"

class BuiltInLED : public LED {
    public:
        BuiltInLED() noexcept;
        void blinkLED() noexcept override;
        void blinkLEDTemp() noexcept;
};