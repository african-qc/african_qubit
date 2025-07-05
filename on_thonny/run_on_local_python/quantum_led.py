import qiskit
from qiskit import QuantumCircuit
from qiskit_aer import AerSimulator
import serial
import time

# Parameters
x = 10  # Number of iterations
com_port = "COM3"  # Replace with your ESP32's COM port
baud_rate = 115200  # MicroPython default

# Create a one-qubit circuit
qc = QuantumCircuit(1, 1)
qc.h(0)  # Hadamard gate for 50% |0⟩, 50% |1⟩
# Optional: qc.rx(0.927, 0) for ~30% |1⟩ probability
qc.measure(0, 0)

# Initialize simulator
simulator = AerSimulator()

# Initialize serial connection
try:
    ser = serial.Serial(com_port, baud_rate, timeout=1)
    time.sleep(2)  # Wait for ESP32
    print(f"Connected to ESP32 on {com_port}")
except serial.SerialException as e:
    print(f"Serial error: {e}")
    exit(1)

# Flush input to avoid garbled data
ser.reset_input_buffer()
ser.reset_output_buffer()

# Counters
count_0 = 0
count_1 = 0

print(f"Starting quantum circuit simulation for {x} iterations")

for i in range(x):
    try:
        # Run circuit
        job = simulator.run(qc, shots=1)
        result = job.result()
        counts = result.get_counts(qc)
        outcome = list(counts.keys())[0]
        print(f"Iteration {i+1}, outcome: {outcome}")
        
        # Update counters
        if outcome == "0":
            count_0 += 1
        else:
            count_1 += 1
        
        # Send to ESP32
        ser.write(outcome.encode() + b"\n")
        ser.flush()
        print(f"Sent {outcome} to ESP32")
        time.sleep(1.5)  # Sync with ESP32 timing
    except serial.SerialException as e:
        print(f"Serial write error: {e}")
        break
    except Exception as e:
        print(f"Simulation error: {e}")
        break

# Print summary
print("Sequence complete")
print(f"Outcome 0 (LED 1): {count_0} times ({count_0/x:.2%})")
print(f"Outcome 1 (LED 2): {count_1} times ({count_1/x:.2%})")

# Close serial
ser.close()
print("Serial connection closed")