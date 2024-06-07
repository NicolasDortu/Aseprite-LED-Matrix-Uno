import serial
import time

try:
    ser = serial.Serial("COM3", 9600)  # Update with your serial port
    time.sleep(2)  # Wait for the connection to establish

    with open("./aseprite/frames.bin", "rb") as f:
        while byte := f.read(1):
            ser.write(byte)
            print(byte)
            time.sleep(0.01)  # Add a small delay to ensure data is transmitted properly

    ser.close()
except serial.SerialException as e:
    print(f"Error: {e}")
