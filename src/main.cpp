// Full LED Control Example with State Machine, Error Handling, Watchdog Timer, Low Power Techniques, Advanced Interrupt Handling, Serial, and Virtual Function for Arduino Mega 2560

// Include the Arduino library
#include "Embedded.h"

// Define the pin for the built-in LED on the Arduino Mega 2560
const uint8_t LED_PIN = LED_BUILTIN;
// Define the pin for the push button
const uint8_t BUTTON_PIN = 2; // Change to the pin where your button is connected

// Define constants for debounce delay
const unsigned long DEBOUNCE_DELAY = 50; // Adjust as needed

// Error codes
enum ErrorCode {
  ERR_NONE,         // No error
  ERR_BUTTON_READ,  // Error reading button state
};

// Volatile variable to store the state of the push button
volatile bool buttonPressed = false;

// Enum for LED states
enum LEDState {
  IDLE,
  BLINKING
};

// Initial state
// Define variables
LEDState currentState = IDLE;

unsigned long lastDebounceTime = 0;

// Function prototypes
ErrorCode readButtonState();

// Base class for LED control
class LED {
  protected:
    const uint8_t pin;
    bool isBlinking;
    uint16_t blinkInterval;

  public:
    // Constructor
    LED(const uint8_t pinNumber) : pin(pinNumber), isBlinking(false), blinkInterval(1000) {
      pinMode(pin, OUTPUT);
    }

    // Virtual function to toggle the blinking state
    virtual void toggleBlinking() {
      isBlinking = !isBlinking; // Toggle the blinking state
    }

    // Virtual function to blink the LED
    virtual void blinkLED() = 0;

    // Function to check if the LED is blinking
    bool isBlinkingLED() const {
      return isBlinking;
    }
};

// Derived class for controlling the built-in LED
class BuiltInLED : public LED {
  public:
    // Constructor
    BuiltInLED() : LED(LED_PIN) {}

    // Implement the blinkLED function for the built-in LED
    void blinkLED() override {
      // Turn the LED on
      digitalWrite(pin, HIGH);
      delay(blinkInterval);
      // Turn the LED off
      digitalWrite(pin, LOW);
      delay(blinkInterval);
    }
};

// Create an instance of the BuiltInLED class
BuiltInLED builtInLED;

// Interrupt service routine for the push button
void buttonISR() {
  buttonPressed = true;
}

// Function to read the debounced button state
bool readDebouncedButtonState() {
  // Read the raw button state
  bool rawButtonState = digitalRead(BUTTON_PIN);

  // Check if the button state has changed and debounce delay has passed
  if (rawButtonState != buttonPressed && millis() - lastDebounceTime > DEBOUNCE_DELAY) {
    // Update last debounce time
    lastDebounceTime = millis();
    // Update button state
    buttonPressed = rawButtonState;
  }

  return buttonPressed;
}

void setup() {
  // Initialize serial communication at 9600 baud rate
  Serial.begin(9600);
  // Wait for serial port to connect
  while (!Serial);

  // Set up the push button pin for interrupt
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Use internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING); // Attach interrupt to the button pin

  // Enable the watchdog timer with a timeout period of 2 seconds
  wdt_enable(WDTO_2S);
}

// Define a variable to store the button state and its previous state
uint8_t buttonState = HIGH << 1;

void loop() {
  // Shift the current button state into the previous button state
  buttonState = (buttonState << 1) | digitalRead(BUTTON_PIN);

  // Check for a change in button state
  if ((buttonState & 0b11) == 0b10) {
    // Button state changed from HIGH to LOW
    Serial.println(F("Error reading button state: Button is pressed."));
  }

  // State machine logic based on current state
  switch (currentState) {
    case IDLE:
      // Check if the button is pressed to transition to BLINKING state
      if (buttonPressed) {
        currentState = BLINKING;
        Serial.println(F("LED blinking started."));
        buttonPressed = false; // Reset button state
      }
      break;
    case BLINKING:
      // Blink the LED
      builtInLED.blinkLED();
      // Check if the button is pressed to transition back to IDLE state
      if (buttonPressed) {
        currentState = IDLE;
        Serial.println(F("LED blinking stopped."));
        buttonPressed = false; // Reset button state
      }
      break;
    default:
      break;
  }

  // Reset the watchdog timer
  wdt_reset();
}


// Function to read the state of the push button
ErrorCode readButtonState() {
  // Read the button state
  int buttonState = digitalRead(BUTTON_PIN);
  // Check for errors
  if (buttonState == HIGH) {
    return ERR_BUTTON_READ; // Error reading button state
  }
  return ERR_NONE; // No error
}
