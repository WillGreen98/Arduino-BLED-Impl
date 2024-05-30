#pragma once

// Define the pin for the built-in LED on the Arduino Mega 2560
// constexpr __UINT8_TYPE__ LED_PIN = LED_BUILTIN;
constexpr __UINT8_TYPE__ LED_PIN = 13;
// Define the pin for the push button
constexpr __UINT8_TYPE__ BUTTON_PIN = 2; // Change to the pin where your button is connected

// Define constants for debounce delay
constexpr unsigned long DEBOUNCE_DELAY = 50; // Adjust as needed

// Error codes
enum class ErrorCode {
    None,         // No error
    ButtonRead,   // Error reading button state
};

// Enum for LED states
enum class LEDState {
    Idle,
    Blinking
};

extern volatile bool buttonPressed;
extern unsigned long lastDebounceTime;
bool readDebouncedButtonState() noexcept;
ErrorCode readButtonState() noexcept;