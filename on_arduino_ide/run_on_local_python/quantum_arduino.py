import qiskit
from qiskit import QuantumCircuit
from qiskit_aer import AerSimulator
import serial
import time
import sys

# Configuration
com_port = "COM3"  # Replace with your ESP32's COM port
baud_rate = 115200

print("African Qubit - Arduino Quantum Integration")
print("=" * 50)

# Create a one-qubit circuit for superposition
qc = QuantumCircuit(1, 1)
qc.h(0)  # Hadamard gate for 50% |0⟩, 50% |1⟩ superposition
qc.measure(0, 0)

# Initialize simulator
simulator = AerSimulator()

# Initialize serial connection
def connect_serial():
    try:
        ser = serial.Serial(com_port, baud_rate, timeout=1)
        time.sleep(2)  # Wait for ESP32
        print(f"✅ Connected to ESP32 on {com_port}")
        return ser
    except serial.SerialException as e:
        print(f"❌ Serial error: {e}")
        return None

# Connect to ESP32
ser = connect_serial()
if not ser:
    print("❌ Failed to connect to ESP32")
    sys.exit(1)

# Flush input to avoid garbled data
ser.reset_input_buffer()
ser.reset_output_buffer()

# Statistics tracking
measurement_count = 0
count_0 = 0
count_1 = 0

print(f"🚀 Quantum circuit ready")
print("⏳ Waiting for MEASURE signals from ESP32...")
print("💡 Press button on ESP32 to start quantum measurement sequence")
print("-" * 50)

# Main loop - wait for commands from ESP32
while True:
    try:
        # Read command from ESP32
        if ser.in_waiting > 0:
            try:
                line = ser.readline().decode('utf-8').strip()
                print(f"📨 Received from ESP32: {line}")
                
                if line == "MEASURE":
                    measurement_count += 1
                    print(f"🔬 MEASURE signal #{measurement_count} received!")
                    print("🎲 Running quantum measurement...")
                    print("==================================================")
                    
                    # Run one quantum measurement
                    try:
                        job = simulator.run(qc, shots=1)
                        result = job.result()
                        counts = result.get_counts(qc)
                        outcome = list(counts.keys())[0]
                        
                        print(f"🎲 Quantum measurement outcome: {outcome}")
                        print(f"📈 This represents |{outcome}⟩ state")
                        
                        # Update counters
                        if outcome == "0":
                            count_0 += 1
                            print("💡 |0⟩ state detected (LED 1 will light)")
                        else:
                            count_1 += 1
                            print("💡 |1⟩ state detected (LED 2 will light)")
                        
                        # Send result to ESP32
                        ser.write(outcome.encode() + b"\n")
                        ser.flush()
                        print(f"📤 Sent {outcome} to ESP32")
                        
                        # Show current statistics
                        total = count_0 + count_1
                        if total > 0:
                            print(f"📊 Current stats: |0⟩: {count_0} ({count_0/total:.1%}), |1⟩: {count_1} ({count_1/total:.1%})")
                        
                    except Exception as e:
                        print(f"❌ Quantum measurement error: {e}")
                        # Send a default result to prevent ESP32 from hanging
                        try:
                            default_outcome = "0"
                            ser.write(default_outcome.encode() + b"\n")
                            ser.flush()
                            print(f"📤 Sent default {default_outcome} to ESP32")
                        except:
                            print("❌ Failed to send default result")
                    
                    print("==================================================")
                    
                elif line in ["0", "1"]:
                    # Ignore measurement results sent back to us
                    print(f"📝 Ignoring measurement result: {line}")
                    
                elif "Button Pressed" in line:
                    print("🔘 Button activity: " + line)
                    
                elif "Starting measurement" in line:
                    print("🔄 " + line)
                    
                elif "Progress:" in line:
                    print("📈 " + line)
                    
                elif "Final Results:" in line:
                    print("🏁 " + line)
                    print("🎯 Quantum measurement sequence completed!")
                    print(f"📊 Final Statistics:")
                    print(f"   Total measurements: {measurement_count}")
                    print(f"   |0⟩ states: {count_0} ({count_0/measurement_count:.1%})")
                    print(f"   |1⟩ states: {count_1} ({count_1/measurement_count:.1%})")
                    
                    # Check quantum randomness
                    if measurement_count > 0:
                        ratio = count_1 / measurement_count
                        if 0.4 <= ratio <= 0.6:
                            print("✅ Results show good quantum randomness (~50/50 distribution)")
                        else:
                            print("⚠️ Results may not show expected quantum randomness")
                    
                    print("🏁 Sequence complete, exiting...")
                    break
                    
                elif "Quantum measurement requested" in line:
                    print("ℹ️ Status: " + line)
                    
                elif "LED" in line:
                    print("💡 LED Status: " + line)
                    
                else:
                    # Ignore other status messages
                    print(f"ℹ️ Status message: {line}")
                    
            except UnicodeDecodeError as e:
                print(f"⚠️ Decode error: {e}")
                continue
                
        else:
            # No data available, small delay
            time.sleep(0.1)
            
    except serial.SerialException as e:
        print(f"❌ Serial error: {e}")
        print("⏳ Attempting to reconnect...")
        time.sleep(2)
        try:
            # Try to reconnect
            ser.close()
            time.sleep(1)
            ser = connect_serial()
            if not ser:
                print("❌ Failed to reconnect, exiting...")
                break
        except:
            print("❌ Failed to reconnect")
            break
    except Exception as e:
        print(f"❌ Unexpected error: {e}")
        break

print("🏁 Quantum simulation stopped")

# Close serial
try:
    ser.close()
except:
    pass 