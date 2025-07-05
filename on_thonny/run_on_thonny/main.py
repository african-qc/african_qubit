
import machine
import time
import sys

# LCD Driver for HD44780 in 4-bit mode
class LCD:
    def __init__(self, rs, en, d4, d5, d6, d7):
        self.rs = machine.Pin(rs, machine.Pin.OUT)
        self.en = machine.Pin(en, machine.Pin.OUT)
        self.data = [
            machine.Pin(d4, machine.Pin.OUT),
            machine.Pin(d5, machine.Pin.OUT),
            machine.Pin(d6, machine.Pin.OUT),
            machine.Pin(d7, machine.Pin.OUT)
        ]
        self.init_display()
    
    def init_display(self):
        time.sleep_ms(50)
        self.write_nibble(0x03)
        time.sleep_ms(5)
        self.write_nibble(0x03)
        time.sleep_ms(1)
        self.write_nibble(0x03)
        time.sleep_ms(1)
        self.write_nibble(0x02)
        self.command(0x28)
        self.command(0x0C)
        self.command(0x06)
        self.command(0x01)
        time.sleep_ms(2)
    
    def write_nibble(self, nibble):
        for i in range(4):
            self.data[i].value((nibble >> i) & 1)
        self.en.value(1)
        time.sleep_ms(1)
        self.en.value(0)
        time.sleep_ms(1)
    
    def command(self, cmd):
        self.rs.value(0)
        self.write_nibble(cmd >> 4)
        self.write_nibble(cmd & 0x0F)
    
    def write_char(self, char):
        self.rs.value(1)
        self.write_nibble(ord(char) >> 4)
        self.write_nibble(ord(char) & 0x0F)
    
    def write_string(self, text):
        for char in text:
            self.write_char(char)
    
    def clear(self):
        self.command(0x01)
        time.sleep_ms(2)
    
    def set_cursor(self, col, row):
        offsets = [0x00, 0x40]
        self.command(0x80 | (offsets[row] + col))

# Initialize LEDs, backlight PWM, LCD, and button
led1 = machine.Pin(2, machine.Pin.OUT)
led2 = machine.Pin(4, machine.Pin.OUT)
vout_5v = machine.Pin(0, machine.Pin.OUT)
led1.value(0)
led2.value(0)
vout_5v.value(1)

backlight = machine.Pin(5, machine.Pin.OUT)
backlight_pwm = machine.PWM(backlight, freq=1000)
backlight_pwm.duty_u16(16384)  # 25% brightness

lcd = LCD(rs=19, en=23, d4=18, d5=17, d6=16, d7=15)

button = machine.Pin(13, machine.Pin.IN, machine.Pin.PULL_UP)

# Counters
count_0 = 0
count_1 = 0
current_mode = "Unknown"

# Initialize LCD
lcd.clear()
lcd.set_cursor(0, 0)
lcd.write_string("Quantum Demo")
lcd.set_cursor(0, 1)
lcd.write_string("Waiting...")
time.sleep(2)

print("ESP32 ready, waiting for serial commands...")

# Superposition animation
def animate_superposition():
    frames = ["~    ", " ~   ", "  ~  ", "   ~ ", "    ~"]
    for frame in frames:
        lcd.set_cursor(11, 0)
        lcd.write_string(frame)
        backlight_pwm.duty_u16(16384)  # Keep backlight ON
        time.sleep(0.3)

# Button interrupt for measurement
measure_flag = False
def button_handler(pin):
    global measure_flag
    measure_flag = True

button.irq(trigger=machine.Pin.IRQ_FALLING, handler=button_handler)

while True:
    try:
        backlight_pwm.duty_u16(16384)  # Ensure backlight stays ON
        if 'stdin' in dir(sys):
            line = sys.stdin.readline().strip()
            if line in ["M1", "M2"]:
                current_mode = "Fidelity" if line == "M1" else "Superposition"
                print(f"Mode set: {current_mode}")
                lcd.clear()
                lcd.set_cursor(0, 0)
                lcd.write_string(f"Mode: {current_mode[:9]}")
                lcd.set_cursor(0, 1)
                lcd.write_string(f"0:{count_0} 1:{count_1}")
                count_0 = 0
                count_1 = 0
            elif line == "S" and current_mode == "Superposition":
                print("Superposition state")
                lcd.clear()
                lcd.set_cursor(0, 0)
                lcd.write_string("Superposition")
                lcd.set_cursor(0, 1)
                lcd.write_string("Press button...")
                measure_flag = False
                while not measure_flag:
                    animate_superposition()
                print("Sending MEASURE")
                sys.stdout.write(b"MEASURE\n")
                sys.stdout.flush()
            elif line in ["0", "1"]:
                print(f"Received: {line}")
                lcd.clear()
                lcd.set_cursor(0, 0)
                lcd.write_string(f"Outcome: {line}")
                lcd.set_cursor(0, 1)
                
                if line == "0":
                    count_0 += 1
                    led1.value(1)
                    led2.value(0)
                    print("LED 1 ON")
                else:
                    count_1 += 1
                    led1.value(0)
                    led2.value(1)
                    print("LED 2 ON")
                
                lcd.write_string(f"0:{count_0} 1:{count_1}")
                time.sleep(1)
                led1.value(0)
                led2.value(0)
                print("LEDs OFF")
                time.sleep(0.5)
            else:
                print(f"Invalid: {line}")
        else:
            print("No stdin, waiting...")
            time.sleep(0.1)
    except Exception as e:
        print(f"Error: {e}")
        lcd.clear()
        lcd.set_cursor(0, 0)
        lcd.write_string("Error")
        time.sleep(1)
        lcd.clear()
        lcd.set_cursor(0, 0)
        lcd.write_string(f"Mode: {current_mode[:9]}")
        time.sleep(0.1)
        