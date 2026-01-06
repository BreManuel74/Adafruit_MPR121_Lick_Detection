import serial
import time
from datetime import datetime

SERIAL_PORT = 'COM4'
BAUD_RATE = 115200
timestamp_str = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
CSV_FILENAME = f"capacitive_log_{timestamp_str}.csv"
BUFFER_SIZE = 100

try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)
    print(f"Connected to {SERIAL_PORT}")
except serial.SerialException as e:
    print(f"Error: {e}")
    exit()

with open(CSV_FILENAME, mode='w', newline='') as file:
    file.write("Arduino_Timestamp," + ",".join(f"Sensor_{i+1}" for i in range(24)) + "\n")

    buffer = []

    try:
        while True:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            if not line.startswith("T:") or "B#1:" not in line or "B#2:" not in line:
                continue

            parts = line.split()
            timestamp = parts[0][2:]

            numbers = []
            for part in parts[1:]:
                if part == "B#1:" or part == "B#2:":
                    continue
                try:
                    numbers.append(float(part))
                except ValueError:
                    continue

            if len(numbers) != 24:
                continue

            buffer.append(f"{timestamp}," + ",".join(map(str, numbers)))

            if len(buffer) >= BUFFER_SIZE:
                file.write("\n".join(buffer) + "\n")
                file.flush()
                buffer.clear()

    except KeyboardInterrupt:
        print("Logging stopped.")
    finally:
        ser.close()
        print("Serial connection closed.")
