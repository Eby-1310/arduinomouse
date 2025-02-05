import serial
import time
from pynput.mouse import Button, Controller

# Initialize mouse controller
mouse = Controller()

# Replace 'COM3' with your Arduino's port (e.g., '/dev/ttyUSB0' for Linux/Mac)
arduino_port = 'COM3'
baud_rate = 9600

# Connect to Arduino
try:
    arduino = serial.Serial(arduino_port, baud_rate, timeout=0.1)
    time.sleep(2)  # Allow time for the Arduino to reset
    print(f"Connected to Arduino on {arduino_port}")
except serial.SerialException:
    print(f"Failed to connect to Arduino on {arduino_port}")
    exit()

# Function to process incoming data
def process_data(data):
    try:
        # Parse incoming data (format: x,y,leftClick,rightClick)
        x, y, left_click, right_click = map(int, data.strip().split(","))
        return x, y, left_click, right_click
    except ValueError:
        return None

# Main loop
def main():
    print("Mouse control script running. Press Ctrl+C to exit.")
    while True:
        if arduino.in_waiting > 0:
            data = arduino.readline().decode('utf-8').strip()
            processed = process_data(data)
            
            if processed:
                x, y, left_click, right_click = processed

                # Move the mouse
                mouse.move(x, -y)  # Invert Y-axis for natural movement
                
                # Handle left click
                if left_click:
                    mouse.press(Button.left)
                else:
                    mouse.release(Button.left)
                
                # Handle right click
                if right_click:
                    mouse.press(Button.right)
                else:
                    mouse.release(Button.right)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nExiting...")
        arduino.close()
    except Exception as e:
        print(f"An error occurred: {e}")
        arduino.close()
