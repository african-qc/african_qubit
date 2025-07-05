# African Qubit - Quantum Measurement System

A hands-on quantum computing demonstration using ESP32 microcontrollers, LEDs, LCD displays, and Python quantum simulation.

## 🌟 Overview

This project demonstrates quantum superposition and measurement through physical hardware:
- **LEDs represent quantum states** (superposition and collapsed states)
- **Button press triggers measurement** to collapse quantum superposition
- **Python runs quantum simulations** using Qiskit for authentic randomness
- **LCD displays real-time feedback** and measurement statistics

## 📁 Project Structure

```
african_qubit/
├── README.md                    # This file
├── on_arduino_ide/             # Arduino C++ implementation
│   ├── README.md               # Detailed Arduino setup guide
│   ├── run_on_esp32/          # ESP32 Arduino code
│   └── run_on_local_python/   # Python quantum simulation
└── on_thonny/                  # MicroPython implementation
    ├── README.md               # Detailed MicroPython setup guide
    ├── run_on_thonny/         # ESP32 MicroPython code
    └── run_on_local_python/   # Python quantum simulation
```

## 🚀 Quick Start

### Choose Your Implementation

#### Option 1: Arduino (Recommended for Beginners)
```bash
# 1. Upload Arduino code to ESP32
# 2. Install Python dependencies
pip install pyserial qiskit qiskit-aer

# 3. Run quantum simulation
cd on_arduino_ide/run_on_local_python
python quantum_arduino.py
```

#### Option 2: MicroPython (Python on ESP32)
```bash
# 1. Install MicroPython on ESP32 using Thonny
# 2. Upload MicroPython code
# 3. Install Python dependencies
pip install pyserial qiskit qiskit-aer

# 4. Run quantum simulation
cd on_thonny/run_on_local_python
python quantum_led.py
```

## 🔧 Hardware Requirements

- **ESP32 Development Board**
- **2 LEDs** with 220Ω resistors
- **1 Push Button** (momentary switch)
- **16x2 LCD Display** (HD44780 compatible)
- **Breadboard and jumper wires**

## 🎯 Key Features

- ✅ **Quantum Superposition Visualization**: LEDs blink to show superposition
- ✅ **Measurement Collapse**: Button press triggers quantum measurement
- ✅ **Real Quantum Simulation**: Uses Qiskit for authentic randomness
- ✅ **Iteration Control**: Configurable number of measurements
- ✅ **Statistics Tracking**: Final results show quantum state distribution
- ✅ **Educational Value**: Demonstrates quantum computing concepts

## 🎮 How It Works

1. **Press button** → Start quantum measurement sequence
2. **LEDs blink** → Shows quantum superposition state
3. **Press button again** → Triggers quantum measurement
4. **One LED lights** → Shows collapsed quantum state (|0⟩ or |1⟩)
5. **Repeat** until all iterations complete
6. **Final results** → Statistics on LCD and console

## 📊 Expected Results

With proper quantum simulation, you should see approximately:
- **50% |0⟩ states** (LED 1 lights)
- **50% |1⟩ states** (LED 2 lights)

This demonstrates the fundamental quantum principle of superposition and measurement.

## 🔄 Implementation Comparison

| Feature | Arduino | MicroPython |
|---------|---------|-------------|
| **Programming** | C++ | Python |
| **IDE** | Arduino IDE | Thonny |
| **Development** | Compile & upload | Interactive REPL |
| **Learning Curve** | Steeper | Easier |
| **Performance** | Faster | Interpreted |
| **Debugging** | Serial monitor | Real-time feedback |

## 📚 Educational Value

This project teaches:
- **Quantum Superposition**: Physical representation with LEDs
- **Measurement Collapse**: Button press triggers measurement
- **Quantum Randomness**: True randomness from quantum mechanics
- **State Representation**: Physical LEDs represent quantum states
- **Embedded Systems**: Hardware control with microcontrollers

## 🛠️ Configuration

### Arduino Version
- **File**: `on_arduino_ide/run_on_esp32/simulator_code_quantum.cpp`
- **Settings**: Change `TOTAL_ITERATIONS = 10` to your desired number
- **Pins**: Verify pin assignments match your wiring

### MicroPython Version
- **File**: `on_thonny/run_on_local_python/quantum_led.py`
- **Settings**: Change `x = 10` to your desired number
- **COM Port**: Update `com_port = "COM3"` to match your ESP32

## 🐛 Troubleshooting

### Common Issues
1. **ESP32 not detected**: Check USB cable and drivers
2. **LEDs not working**: Verify wiring and pin assignments
3. **LCD not displaying**: Check all 7-8 wire connections
4. **Button not responding**: Verify pull-up resistor and wiring
5. **Serial communication**: Check COM port and baud rate (115200)

### Debug Mode
- **Arduino**: Use `simulator_code_no_quantum.cpp` for hardware testing
- **MicroPython**: Use Thonny's Shell for real-time debugging

## 📖 Detailed Documentation

- **[Arduino Setup Guide](on_arduino_ide/README.md)** - Complete Arduino implementation guide
- **[MicroPython Setup Guide](on_thonny/README.md)** - Complete MicroPython implementation guide

## 🤝 Contributing

Feel free to extend this project:
- Add more quantum gates and algorithms
- Implement multi-qubit systems
- Improve hardware design
- Add web interface for remote control

## 📄 License

This project is open source. Feel free to use, modify, and distribute.

## 🙏 Acknowledgments

- **Qiskit** for quantum simulation capabilities
- **Arduino community** for ESP32 support
- **MicroPython community** for embedded Python
- **Quantum computing community** for educational resources

---

**Happy Quantum Computing! 🌌**

*Choose your preferred implementation and start exploring quantum mechanics with physical hardware!*