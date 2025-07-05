#include <LiquidCrystal.h> // Include the standard LiquidCrystal library for parallel LCDs

// --- Configuration ---
// Define the GPIO pins for your LEDs
const int ledPin1 = 2;  // Example: GPIO 2
const int ledPin2 = 4;  // Example: GPIO 4

// Define the GPIO pin for the pushbutton
const int buttonPin = 22; // Pushbutton connected to GPIO 22

// Define the number of iterations
const int TOTAL_ITERATIONS = 10; // Change this to your desired number

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

// --- Global Variables ---
String currentLEDStateMessage = "Press Button"; // Default message
bool waitingForQuantumResult = false; // Flag to prevent multiple requests
unsigned long quantumRequestTime = 0; // Time when quantum request was sent
const unsigned long QUANTUM_TIMEOUT = 5000; // 5 second timeout for quantum results

// --- Quantum State Variables ---
int currentIteration = 0;
bool inSuperposition = false; // Whether LEDs are in superposition state
unsigned long lastSuperpositionToggle = 0; // For LED blinking timing
const unsigned long SUPERPOSITION_INTERVAL = 200; // 200ms for LED blinking
bool led1State = false; // Current state of LED1 in superposition

// --- Statistics ---
int count0 = 0;
int count1 = 0;

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
  lcd.print("Quantum LED Sim");
  lcd.setCursor(0, 1); // Column 0, Row 1
  lcd.print("Press to start");

  Serial.println("Quantum LED Simulator Ready!");
  Serial.println("Press button to start quantum measurement sequence");
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
        
        if (currentIteration >= TOTAL_ITERATIONS) {
          // Show final results
          showFinalResults();
        } else if (!waitingForQuantumResult && !inSuperposition) {
          // Start new measurement cycle
          startNewMeasurement();
        } else if (inSuperposition && !waitingForQuantumResult) {
          // Button pressed during superposition - trigger measurement
          triggerMeasurement();
        }
      }
    }
  }

  // Handle superposition LED blinking
  if (inSuperposition && !waitingForQuantumResult) {
    handleSuperpositionBlinking();
  }

  // Check for quantum results from Python
  if (Serial.available() > 0) {
    String quantumResult = Serial.readStringUntil('\n');
    quantumResult.trim();
    
    if (quantumResult == "0" || quantumResult == "1") {
      handleQuantumResult(quantumResult);
    }
  }

  // Check for timeout on quantum requests
  if (waitingForQuantumResult && (millis() - quantumRequestTime > QUANTUM_TIMEOUT)) {
    Serial.println("Timeout waiting for quantum result");
    waitingForQuantumResult = false;
    inSuperposition = false;
    updateLCDMessage("Timeout Error");
  }

  // Save the current reading as the lastButtonState for the next loop iteration
  lastButtonState = reading;

  // Small delay to prevent overwhelming the system
  delay(10);
}

// Function to start a new measurement cycle
void startNewMeasurement() {
  currentIteration++;
  inSuperposition = true;
  led1State = false;
  lastSuperpositionToggle = millis();
  
  updateLCDMessage("Superposition");
  Serial.print("Starting measurement ");
  Serial.print(currentIteration);
  Serial.print(" of ");
  Serial.println(TOTAL_ITERATIONS);
  
  // Start LED blinking to show superposition
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, LOW);
}

// Function to handle LED blinking during superposition
void handleSuperpositionBlinking() {
  if (millis() - lastSuperpositionToggle >= SUPERPOSITION_INTERVAL) {
    led1State = !led1State;
    lastSuperpositionToggle = millis();
    
    if (led1State) {
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, LOW);
    } else {
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, HIGH);
    }
  }
}

// Function to trigger measurement (button pressed during superposition)
void triggerMeasurement() {
  inSuperposition = false;
  requestQuantumMeasurement();
}

// Function to request quantum measurement from Python
void requestQuantumMeasurement() {
  if (waitingForQuantumResult) {
    Serial.println("Already waiting for quantum result");
    return;
  }
  
  Serial.println("MEASURE"); // Send MEASURE signal to Python
  waitingForQuantumResult = true;
  quantumRequestTime = millis();
  
  updateLCDMessage("Measuring...");
  Serial.println("Quantum measurement requested");
}

// Function to handle quantum measurement results
void handleQuantumResult(String result) {
  waitingForQuantumResult = false;
  
  Serial.print("Received quantum result: ");
  Serial.println(result);
  
  // Turn off both LEDs first
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  
  // Control LEDs based on quantum result
  if (result == "0") {
    digitalWrite(ledPin1, HIGH); // Turn LED1 on for |0⟩ state
    digitalWrite(ledPin2, LOW);  // Ensure LED2 is off
    currentLEDStateMessage = "|0⟩ LED1 ON!";
    count0++;
    Serial.println("LED1 is ON (|0⟩ state)");
  } else if (result == "1") {
    digitalWrite(ledPin1, LOW);  // Ensure LED1 is off
    digitalWrite(ledPin2, HIGH); // Turn LED2 on for |1⟩ state
    currentLEDStateMessage = "|1⟩ LED2 ON!";
    count1++;
    Serial.println("LED2 is ON (|1⟩ state)");
  }
  
  // Update the LCD display
  updateLCDMessage(currentLEDStateMessage);
  
  // Show progress
  Serial.print("Progress: ");
  Serial.print(currentIteration);
  Serial.print("/");
  Serial.print(TOTAL_ITERATIONS);
  Serial.print(" (|0⟩: ");
  Serial.print(count0);
  Serial.print(", |1⟩: ");
  Serial.print(count1);
  Serial.println(")");
  
  // Wait a moment before allowing next measurement
  delay(2000);
  
  // Reset for next iteration
  inSuperposition = false;
  updateLCDMessage("Press for next");
}

// Function to show final results
void showFinalResults() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Final Results:");
  lcd.setCursor(0, 1);
  lcd.print("|0⟩:");
  lcd.print(count0);
  lcd.print(" |1⟩:");
  lcd.print(count1);
  
  Serial.println("=== FINAL RESULTS ===");
  Serial.print("|0⟩ states: ");
  Serial.print(count0);
  Serial.print(" (");
  Serial.print((float)count0 / TOTAL_ITERATIONS * 100);
  Serial.println("%)");
  Serial.print("|1⟩ states: ");
  Serial.print(count1);
  Serial.print(" (");
  Serial.print((float)count1 / TOTAL_ITERATIONS * 100);
  Serial.println("%)");
  Serial.println("===================");
  
  // Keep LEDs in final state
  if (count0 > count1) {
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, LOW);
  } else {
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, HIGH);
  }
}

// Function to update LCD message
void updateLCDMessage(String message) {
  lcd.setCursor(0, 1); // Move cursor to the start of the second row
  lcd.print("                "); // Clear the second line by printing spaces
  lcd.setCursor(0, 1); // Move cursor back to the start of the second row
  lcd.print(message); // Print the new message
} 