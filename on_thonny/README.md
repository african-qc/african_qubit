# African Qubit - Thonny/MicroPython Quantum Measurement System

A quantum measurement system that demonstrates quantum superposition and measurement using an ESP32 microcontroller with MicroPython, LEDs, LCD display, and Python quantum simulation.

## 🌟 Overview

This project creates a hands-on quantum computing experience using **MicroPython on ESP32** where:
- **LEDs represent quantum states** in superposition and collapsed states
- **Button press triggers measurement** to collapse the quantum superposition
- **Python runs quantum simulations** using Qiskit for authentic quantum randomness
- **LCD displays real-time feedback** and measurement statistics
- **Serial communication** connects ESP32 hardware with Python quantum simulation

## 🎯 Key Features

- ✅ **MicroPython Implementation**: Uses Thonny IDE and MicroPython for ESP32
- ✅ **Quantum Superposition Visualization**: LEDs and LCD show quantum states
- ✅ **Measurement Collapse**: Button press triggers quantum measurement
- ✅ **Iteration Control**: Configurable number of measurements
- ✅ **Real Quantum Simulation**: Uses Qiskit for authentic quantum randomness
- ✅ **Real-time Feedback**: LCD shows current state and progress
- ✅ **Statistics Tracking**: Final results show quantum state distribution
- ✅ **Robust Communication**: Error handling and reconnection logic

## 📁 Project Structure

```
on_thonny/
├── README.md                           # This file
├── run_on_thonny/
│   └── main.py                        # Main ESP32 MicroPython code
└── run_on_local_python/
    └── quantum_led.py                 # Python quantum simulation script
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
| Push Button | GPIO 13 | Triggers measurement |
| LCD RS | GPIO 19 | LCD Register Select |
| LCD EN | GPIO 23 | LCD Enable |
| LCD D4 | GPIO 18 | LCD Data 4 |
| LCD D5 | GPIO 17 | LCD Data 5 |
| LCD D6 | GPIO 16 | LCD Data 6 |
| LCD D7 | GPIO 15 | LCD Data 7 |
| Backlight | GPIO 5 | LCD Backlight PWM |
| 5V Output | GPIO 0 | 5V Power Output |

## 🚀 Setup Instructions

### Step 1: Hardware Assembly

1. **Connect LEDs**:
   - LED 1: GPIO 2 → 220Ω resistor → LED → GND
   - LED 2: GPIO 4 → 220Ω resistor → LED → GND

2. **Connect Push Button**:
   - One terminal to GPIO 13
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
   - Backlight to GPIO 5 (PWM control)

### Step 2: Software Setup

#### ESP32 Setup (Thonny IDE)

1. **Install Thonny IDE** from https://thonny.org/
2. **Install MicroPython** on ESP32:
   - Connect ESP32 to computer
   - Open Thonny IDE
   - Go to Tools → Options → Interpreter
   - Select "MicroPython (ESP32)"
   - Click "Install or update firmware"
3. **Upload MicroPython code**:
   - Open `run_on_thonny/main.py`
   - Save to ESP32 (Ctrl+S)
   - Run the script (F5)

#### Python Setup

1. **Install Python dependencies**:
   ```bash
   pip install pyserial qiskit qiskit-aer
   ```

2. **Configure COM port**:
   - Edit `run_on_local_python/quantum_led.py`
   - Change `com_port = "COM3"` to match your ESP32's port
   - Common ports: `COM3` (Windows), `/dev/ttyUSB0` (Linux), `/dev/cu.SLAB_USBtoUART` (Mac)

### Step 3: Running the System

1. **Start Python script**:
   ```bash
   cd run_on_local_python
   python quantum_led.py
   ```

2. **ESP32 will automatically**:
   - Initialize LCD and LEDs
   - Wait for serial commands
   - Display quantum measurement results

## 🎮 Usage Guide

### Quantum Measurement Workflow

1. **Initial State**: ESP32 shows "Quantum Demo" and "Waiting..."
2. **Python sends commands**:
   - `M1` or `M2`: Set measurement mode
   - `S`: Start superposition state
3. **Superposition State**:
   - LCD shows "Superposition" with animation
   - LEDs ready for measurement
4. **Button Press**:
   - Triggers measurement
   - Sends "MEASURE" to Python
5. **Quantum Result**:
   - Python sends "0" or "1"
   - ESP32 lights appropriate LED
   - LCD shows outcome and statistics

### Understanding the Quantum States

- **|0⟩ State**: LED 1 lights (represents quantum state |0⟩)
- **|1⟩ State**: LED 2 lights (represents quantum state |1⟩)
- **Superposition**: LCD animation shows quantum superposition
- **Measurement**: Button press collapses superposition to definite state

### Expected Results

With a proper quantum simulation, you should see approximately:
- **50% |0⟩ states** (LED 1)
- **50% |1⟩ states** (LED 2)

This demonstrates the fundamental quantum principle of superposition and measurement.

## 🔧 Configuration Options

### ESP32 Configuration (`main.py`)

```python
# LED pins
led1 = machine.Pin(2, machine.Pin.OUT)
led2 = machine.Pin(4, machine.Pin.OUT)

# Button pin
button = machine.Pin(13, machine.Pin.IN, machine.Pin.PULL_UP)

# LCD pins
lcd = LCD(rs=19, en=23, d4=18, d5=17, d6=16, d7=15)

# Backlight control
backlight = machine.Pin(5, machine.Pin.OUT)
backlight_pwm = machine.PWM(backlight, freq=1000)
backlight_pwm.duty_u16(16384)  # 25% brightness
```

### Python Configuration (`quantum_led.py`)

```python
# Serial communication
com_port = "COM3"  # Change to your ESP32's port
baud_rate = 115200

# Number of iterations
x = 10  # Change to your desired number

# Quantum circuit (modify for different quantum states)
qc = QuantumCircuit(1, 1)
qc.h(0)  # Hadamard gate for 50/50 superposition
qc.measure(0, 0)
```

## 🐛 Troubleshooting

### Common Issues

1. **ESP32 not detected in Thonny**:
   - Check USB cable and drivers
   - Try different USB port
   - Install CP210x or CH340 drivers if needed

2. **MicroPython not installing**:
   - Hold BOOT button while connecting ESP32
   - Try different firmware versions
   - Check board selection in Thonny

3. **LEDs not working**:
   - Check wiring and resistor connections
   - Verify pin assignments in code
   - Test with simple LED blink program

4. **LCD not displaying**:
   - Check all 8 wire connections (including backlight)
   - Verify pin assignments
   - Test with simple LCD program

5. **Button not responding**:
   - Check wiring (one terminal to GPIO, other to GND)
   - Verify internal pull-up is enabled
   - Test with simple button program

6. **Serial communication issues**:
   - Check COM port setting
   - Ensure baud rate matches (115200)
   - Try reconnecting USB cable

### Debug Mode

For troubleshooting, you can:
- **Test hardware separately** with simple MicroPython scripts
- **Check serial output** in Thonny's Shell window
- **Verify connections** with multimeter

## 📊 Understanding Quantum Results

### Quantum Randomness Validation

The system provides quantum randomness through:
- **Qiskit simulation**: True quantum randomness from superposition
- **Statistical analysis**: Distribution should be ~50/50
- **Educational value**: Demonstrates quantum vs classical randomness

### Expected vs Classical Randomness

- **Quantum simulation**: True quantum randomness from superposition
- **Classical random**: Pseudo-random number generation
- **Real quantum hardware**: Would use actual quantum computers

## 🔬 Educational Value

This project demonstrates key quantum computing concepts:

1. **Superposition**: LCD animation represents quantum superposition
2. **Measurement**: Button press collapses superposition to definite state
3. **Quantum Randomness**: True randomness from quantum mechanics
4. **State Representation**: Physical LEDs represent quantum states
5. **Measurement Statistics**: Distribution of quantum measurement outcomes
6. **MicroPython Programming**: Embedded systems with Python

## 🤝 Contributing

Feel free to modify and extend this project:

- **Add more quantum gates** (X, Y, Z, CNOT, etc.)
- **Implement multi-qubit systems**
- **Add different quantum algorithms**
- **Improve hardware design**
- **Add web interface for remote control**
- **Enhance MicroPython features**

## 📄 License

This project is open source. Feel free to use, modify, and distribute.

## 🙏 Acknowledgments

- **Qiskit** for quantum simulation capabilities
- **MicroPython community** for ESP32 support
- **Thonny IDE** for excellent MicroPython development environment
- **Quantum computing community** for educational resources

## 🔄 Comparison with Arduino Version

| Feature | Thonny/MicroPython | Arduino |
|---------|-------------------|---------|
| **Programming** | Python syntax | C++ syntax |
| **IDE** | Thonny | Arduino IDE |
| **Development** | Interactive REPL | Compile & upload |
| **Debugging** | Real-time feedback | Serial monitor |
| **Complexity** | Easier for beginners | More control |
| **Performance** | Interpreted | Compiled |
| **Libraries** | MicroPython modules | Arduino libraries |

## 🚀 Getting Started Quick

1. **Flash MicroPython** to ESP32 using Thonny
2. **Upload** `run_on_thonny/main.py` to ESP32
3. **Install Python dependencies**: `pip install pyserial qiskit qiskit-aer`
4. **Run** `run_on_local_python/quantum_led.py`
5. **Press button** on ESP32 to start quantum measurements!

---

**Happy Quantum Computing with MicroPython! 🌌🐍** 