import serial
import time
from pynput.mouse import Button, Controller

# Initialize mouse controller
mouse = Controller()

# Replace 'COM3' with your Arduino's port (e.g., '/dev/ttyUSB0' for Linux/Mac)
port = 'COM5'
rate = 9600

# Connect to Arduino
try:
    arduino = serial.Serial(port, rate, timeout=0.1)
    time.sleep(2)
    print(f"Connected to Arduino on {port}")
except serial.SerialException:
    print(f"Failed to connect to Arduino on {port}")
    exit()

# Function to process incoming data
def dataMap(data):
    try:
        x, y, leftC, rightC = map(int, data.strip().split(","))
        return x, y, leftC, rightC
    except ValueError:
        return None

# Main loop
def main():
    print("Mouse control script running. Press Ctrl+C to exit.")

    # Track previous button states
    leftCState = False
    rightCState = False

    while True:
        if arduino.in_waiting > 0:
            data = arduino.readline().decode('utf-8').strip()
            mapped = dataMap(data)

            if mapped:
                x, y, leftC, rightC = mapped

                mouse.move(x, -y)

                
                if leftC and not leftCState:
                    mouse.press(Button.left)
                    print("Left button pressed")
                elif not leftC and leftCState:
                    mouse.release(Button.left)  
                    print("Left button released")

                
                if rightC and not rightCState:
                    mouse.press(Button.right)
                    print("Right button pressed")
                elif not rightC and rightCState:
                    mouse.release(Button.right)
                    print("Right button released")

                # Store previous button states
                leftCState = leftC
                rightCState = rightC

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nExiting...")
        arduino.close()
    except Exception as e:
        print(f"An error occurred: {e}")
        arduino.close()
        
