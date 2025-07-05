# African Qubit - Arduino Quantum Measurement System

A quantum measurement system that demonstrates quantum superposition and measurement using an ESP32 microcontroller, LEDs, LCD display, and Python quantum simulation.

## 🌟 Overview

This project creates a hands-on quantum computing experience where:
- **LEDs represent quantum states** in superposition (blinking) and collapsed states (single LED lit)
- **Button press triggers measurement** to collapse the quantum superposition
- **Python runs quantum simulations** using Qiskit for authentic quantum randomness
- **LCD displays real-time feedback** and final statistics
- **Serial communication** connects ESP32 hardware with Python quantum simulation

## 🎯 Key Features

- ✅ **Quantum Superposition Visualization**: LEDs blink alternately to show quantum superposition
- ✅ **Measurement Collapse**: Button press triggers quantum measurement and collapses to definite state
- ✅ **Iteration Control**: Configurable number of measurements with progress tracking
- ✅ **Real Quantum Simulation**: Uses Qiskit for authentic quantum randomness
- ✅ **Real-time Feedback**: LCD shows current state and progress
- ✅ **Statistics Tracking**: Final results show quantum state distribution
- ✅ **Robust Communication**: Error handling and reconnection logic

## 📁 Project Structure

```
on_arduino_ide/
├── README.md                           # This file
├── run_on_esp32/
│   ├── simulator_code_quantum.cpp     # Main ESP32 code with quantum integration
│   └── simulator_code_no_quantum.cpp  # Standalone version without Python
└── run_on_local_python/
    └── quantum_arduino.py             # Python quantum simulation script
```

## 🔧 Hardware Requirements

### ESP32 Development Board
- Any ESP32 board (ESP32 DevKit, NodeMCU-32S, etc.)
- USB cable for programming and power

### Components
- **2 LEDs** (any color) with current-limiting resistors (220Ω)
- **1 Push Button** (momentary switch)
- **16x2 LCD Display** (HD44780 compatible)
- **Breadboard and jumper wires**

### Pin Connections

| Component | ESP32 Pin | Description |
|-----------|-----------|-------------|
| LED 1 | GPIO 2 | Represents |0⟩ state |
| LED 2 | GPIO 4 | Represents |1⟩ state |
| Push Button | GPIO 22 | Triggers measurement |
| LCD RS | GPIO 19 | LCD Register Select |
| LCD EN | GPIO 23 | LCD Enable |
| LCD D4 | GPIO 18 | LCD Data 4 |
| LCD D5 | GPIO 17 | LCD Data 5 |
| LCD D6 | GPIO 16 | LCD Data 6 |
| LCD D7 | GPIO 15 | LCD Data 7 |

## 🚀 Setup Instructions

### Step 1: Hardware Assembly

1. **Connect LEDs**:
   - LED 1: GPIO 2 → 220Ω resistor → LED → GND
   - LED 2: GPIO 4 → 220Ω resistor → LED → GND

2. **Connect Push Button**:
   - One terminal to GPIO 22
   - Other terminal to GND
   - Internal pull-up resistor is used

3. **Connect LCD Display**:
   - VCC to 3.3V
   - GND to GND
   - RS to GPIO 19
   - EN to GPIO 23
   - D4 to GPIO 18
   - D5 to GPIO 17
   - D6 to GPIO 16
   - D7 to GPIO 15

### Step 2: Software Setup

#### ESP32 Setup (Arduino IDE)

1. **Install Arduino IDE** and ESP32 board support
2. **Open** `run_on_esp32/simulator_code_quantum.cpp`
3. **Configure settings**:
   - Change `TOTAL_ITERATIONS = 10` to your desired number
   - Verify pin assignments match your wiring
4. **Upload** to ESP32

#### Python Setup

1. **Install Python dependencies**:
   ```bash
   pip install pyserial qiskit qiskit-aer
   ```

2. **Configure COM port**:
   - Edit `run_on_local_python/quantum_arduino.py`
   - Change `com_port = "COM3"` to match your ESP32's port
   - Common ports: `COM3` (Windows), `/dev/ttyUSB0` (Linux), `/dev/cu.SLAB_USBtoUART` (Mac)

### Step 3: Running the System

1. **Start Python script**:
   ```bash
   cd run_on_local_python
   python quantum_arduino.py
   ```

2. **Press button on ESP32** to start quantum measurement sequence

## 🎮 Usage Guide

### Quantum Measurement Workflow

1. **Initial State**: LCD shows "Press to start"
2. **First Button Press**: 
   - LCD shows "Superposition"
   - LEDs start blinking alternately (quantum superposition)
3. **Second Button Press**:
   - LCD shows "Measuring..."
   - Python runs quantum simulation
   - One LED lights based on quantum result
4. **Result Display**:
   - LCD shows "|0⟩ LED1 ON!" or "|1⟩ LED2 ON!"
   - Progress statistics shown
5. **Next Iteration**: Wait 2 seconds, then repeat
6. **Final Results**: Complete statistics displayed

### Understanding the Quantum States

- **|0⟩ State**: LED 1 lights (represents quantum state |0⟩)
- **|1⟩ State**: LED 2 lights (represents quantum state |1⟩)
- **Superposition**: LEDs blink alternately (represents quantum superposition)
- **Measurement**: Button press collapses superposition to definite state

### Expected Results

With a proper quantum simulation, you should see approximately:
- **50% |0⟩ states** (LED 1)
- **50% |1⟩ states** (LED 2)

This demonstrates the fundamental quantum principle of superposition and measurement.

## 🔧 Configuration Options

### ESP32 Configuration (`simulator_code_quantum.cpp`)

```cpp
// Number of measurements
const int TOTAL_ITERATIONS = 10;

// LED pins
const int ledPin1 = 2;
const int ledPin2 = 4;

// Button pin
const int buttonPin = 22;

// LCD pins
const int LCD_RS_PIN = 19;
const int LCD_EN_PIN = 23;
const int LCD_D4_PIN = 18;
const int LCD_D5_PIN = 17;
const int LCD_D6_PIN = 16;
const int LCD_D7_PIN = 15;

// Timing settings
const unsigned long SUPERPOSITION_INTERVAL = 200; // LED blink speed (ms)
const unsigned long QUANTUM_TIMEOUT = 5000;       // Timeout for quantum results (ms)
```

### Python Configuration (`quantum_arduino.py`)

```python
# Serial communication
com_port = "COM3"  # Change to your ESP32's port
baud_rate = 115200

# Quantum circuit (modify for different quantum states)
qc = QuantumCircuit(1, 1)
qc.h(0)  # Hadamard gate for 50/50 superposition
qc.measure(0, 0)
```

## 🐛 Troubleshooting

### Common Issues

1. **ESP32 not detected**:
   - Check USB cable and drivers
   - Verify correct COM port in Python script
   - Try different USB port

2. **LEDs not working**:
   - Check wiring and resistor connections
   - Verify pin assignments in code
   - Test with simple LED blink program

3. **LCD not displaying**:
   - Check all 7 wire connections
   - Verify pin assignments
   - Test with simple LCD program

4. **Button not responding**:
   - Check wiring (one terminal to GPIO, other to GND)
   - Verify internal pull-up is enabled
   - Test with simple button program

5. **Serial communication issues**:
   - Check COM port setting
   - Ensure baud rate matches (115200)
   - Try reconnecting USB cable

### Debug Mode

For troubleshooting, use the standalone version:
- Upload `simulator_code_no_quantum.cpp` to test hardware without Python
- This version generates random results locally

## 📊 Understanding Quantum Results

### Quantum Randomness Validation

The system checks if results show proper quantum randomness:
- **Good randomness**: 40-60% |1⟩ states
- **Poor randomness**: Outside 40-60% range

### Expected vs Classical Randomness

- **Quantum simulation**: True quantum randomness from superposition
- **Classical random**: Pseudo-random number generation
- **Real quantum hardware**: Would use actual quantum computers

## 🔬 Educational Value

This project demonstrates key quantum computing concepts:

1. **Superposition**: LEDs blinking represents quantum superposition
2. **Measurement**: Button press collapses superposition to definite state
3. **Quantum Randomness**: True randomness from quantum mechanics
4. **State Representation**: Physical LEDs represent quantum states
5. **Measurement Statistics**: Distribution of quantum measurement outcomes

## 🤝 Contributing

Feel free to modify and extend this project:

- **Add more quantum gates** (X, Y, Z, CNOT, etc.)
- **Implement multi-qubit systems**
- **Add different quantum algorithms**
- **Improve hardware design**
- **Add web interface for remote control**

## 📄 License

This project is open source. Feel free to use, modify, and distribute.

## 🙏 Acknowledgments

- **Qiskit** for quantum simulation capabilities
- **Arduino community** for ESP32 support
- **Quantum computing community** for educational resources

---

**Happy Quantum Computing! 🌌** 