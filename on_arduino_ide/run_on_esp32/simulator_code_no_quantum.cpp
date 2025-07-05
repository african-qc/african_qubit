#include <LiquidCrystal.h> // Include the standard LiquidCrystal library for parallel LCDs

// --- Configuration ---
// Define the GPIO pins for your LEDs
const int ledPin1 = 2;  // Example: GPIO 2
const int ledPin2 = 4;  // Example: GPIO 4

// Define the GPIO pin for the pushbutton
const int buttonPin = 22; // Pushbutton connected to GPIO 22

// Define the probability for LED1 to light up (0-100)
// The remaining probability will be for LED2
const int probabilityLED1 = 70; // 70% chance for LED1, 30% chance for LED2

// --- LCD Setup ---
// Initialize the LiquidCrystal library with the numbers of the interface pins:
// LiquidCrystal(rs, enable, d4, d5, d6, d7)
const int LCD_RS_PIN = 19;
const int LCD_EN_PIN = 23;
const int LCD_D4_PIN = 18;
const int LCD_D5_PIN = 17;
const int LCD_D6_PIN = 16;
const int LCD_D7_PIN = 15;

LiquidCrystal lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

const int LCD_COLS = 16;
const int LCD_ROWS = 2;

// --- Button Debouncing Variables ---
long lastDebounceTime = 0;   // The last time the output pin was toggled
long debounceDelay = 50;    // The debounce time; increase if button is jumpy

int lastButtonState = HIGH;  // The previous reading from the input pin (HIGH because of INPUT_PULLUP)
int currentButtonState;      // The current reading from the input pin

// --- Global Variable to store current LED state (for LCD display) ---
String currentLEDStateMessage = "Press Button"; // Default message

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  // --- LED Setup ---
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin1, LOW); // Ensure LEDs are off initially
  digitalWrite(ledPin2, LOW);

  // --- Button Setup ---
  // Set the button pin as an input with an internal pull-up resistor
  // This means the pin will be HIGH by default, and go LOW when the button is pressed (connected to GND)
  pinMode(buttonPin, INPUT_PULLUP);

  // --- LCD Setup ---
  lcd.begin(LCD_COLS, LCD_ROWS); // Set up the LCD's number of columns and rows

  // Print initial message to LCD
  lcd.clear();
  lcd.setCursor(0, 0); // Column 0, Row 0
  lcd.print("LED Prob. Sim");
  lcd.setCursor(0, 1); // Column 0, Row 1
  lcd.print(currentLEDStateMessage);

  // --- Random Seed ---
  // Seed the random number generator using an unconnected analog pin (e.g., GPIO 34, 35, 36, 39 are typically ADC inputs)
  randomSeed(analogRead(34)); // If you don't have a floating analog pin, consider using a fixed seed for testing
  Serial.println("Random number generator seeded.");
}

void loop() {
  // Read the state of the pushbutton
  int reading = digitalRead(buttonPin);

  // Check if the button state has changed (from last reading)
  // and if enough time has passed for debouncing
  if (reading != lastButtonState) {
    lastDebounceTime = millis(); // Reset the debouncing timer
  }

  // After the debounce delay, consider the button press valid
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // If the button state has actually changed and is different from currentButtonState
    if (reading != currentButtonState) {
      currentButtonState = reading;

      // Check if the button is pressed (LOW because of INPUT_PULLUP)
      if (currentButtonState == LOW) {
        Serial.println("Button Pressed!");
        // --- Button was pressed, generate new result ---
        generateAndDisplayResult();
      }
    }
  }

  // Save the current reading as the lastButtonState for the next loop iteration
  lastButtonState = reading;

  // The original delay(10) from your template.
  // This small delay is usually fine, but you can remove it if you notice no difference.
  delay(10);
}

// Function to generate the random result, control LEDs, and update LCD
void generateAndDisplayResult() {
  long randomNumber = random(100); // Generates a number from 0 to 99

  Serial.print("Generated random number: ");
  Serial.println(randomNumber);

  // Check the probability and control LEDs
  if (randomNumber < probabilityLED1) {
    digitalWrite(ledPin1, HIGH); // Turn LED1 on
    digitalWrite(ledPin2, LOW);  // Ensure LED2 is off
    currentLEDStateMessage = "LED1 is ON!";
    Serial.println("LED1 is ON (Probability Met)");
  } else {
    digitalWrite(ledPin1, LOW);  // Ensure LED1 is off
    digitalWrite(ledPin2, HIGH); // Turn LED2 on
    currentLEDStateMessage = "LED2 is ON!";
    Serial.println("LED2 is ON (Remaining Probability)");
  }

  // Update the LCD display
  lcd.setCursor(0, 1); // Move cursor to the start of the second row
  lcd.print("                "); // Clear the second line by printing spaces
  lcd.setCursor(0, 1); // Move cursor back to the start of the second row
  lcd.print(currentLEDStateMessage); // Print the new message
}