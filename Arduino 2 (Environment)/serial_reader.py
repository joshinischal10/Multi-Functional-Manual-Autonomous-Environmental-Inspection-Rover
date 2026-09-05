# serial_reader.py
import serial
import time

PORT = 'COM11'  # Update to match your Arduino COM port
BAUD_RATE = 9600

def read_serial():
    try:
        ser = serial.Serial(PORT, BAUD_RATE, timeout=2)
        print(f"Connected to {PORT} at {BAUD_RATE} baud.")
        time.sleep(2)

        while True:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                if line:
                    parts = line.split(',')
                    if len(parts) == 4:
                        try:
                            temp = float(parts[0])
                            hum = float(parts[1])
                            motion = "Detected" if parts[2] == "1" else "Clear"
                            gas = int(parts[3])
                            print(f"Temp: {temp:.1f}°C | Humidity: {hum:.1f}% | Motion: {motion} | Gas: {gas}")
                        except ValueError:
                            print(f"Malformed data received: {line}")
                    else:
                        print(f"Raw Output: {line}")

    except serial.SerialException as e:
        print(f"Serial Error: {e}")
        print("Ensure the Arduino IDE Serial Monitor is CLOSED.")
    except KeyboardInterrupt:
        print("\nExiting serial reader.")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()

if __name__ == "__main__":
    read_serial()