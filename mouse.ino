// Joystick pins
const int VRx = A0;  // Joystick X-axis
const int VRy = A1;  // Joystick Y-axis
const int joystickButton = 2;  // Joystick push button

// Button pins
const int leftClickButton = 5;  // Left-click button
const int rightClickButton = 6; // Right-click button

// Dead zone threshold to eliminate stick drift
const int DEADZONE = 3;

// Button state tracking
bool leftClickState = false;
bool rightClickState = false;

void setup() {
    // Configure joystick and button pins
    pinMode(joystickButton, INPUT_PULLUP);
    pinMode(leftClickButton, INPUT_PULLUP);
    pinMode(rightClickButton, INPUT_PULLUP);

    // Start serial communication
    Serial.begin(9600);
}

void loop() {
    // Read joystick values
    int xValue = analogRead(VRx);
    int yValue = analogRead(VRy);

    // Map joystick values to a range
    int mappedX = map(xValue, 0, 1023, -10, 10);
    int mappedY = map(yValue, 0, 1023, 10, -10); // Inverted Y-axis

    // Apply dead zone filter
    if (abs(mappedX) < DEADZONE) mappedX = 0;
    if (abs(mappedY) < DEADZONE) mappedY = 0;

    // Read buttons (debounced)
    bool leftClick = debounce(leftClickButton, &leftClickState, 0);
    bool rightClick = debounce(rightClickButton, &rightClickState, 1);

    // Send data
    Serial.print(mappedX);
    Serial.print(",");
    Serial.print(mappedY);
    Serial.print(",");
    Serial.print(leftClick);
    Serial.print(",");
    Serial.println(rightClick);

    delay(10); // Reduce data flood
}

// **Fixed Debounce Function**
bool debounce(int pin, bool* state, int index) {
    static unsigned long lastPressTime[2] = {0, 0};  // Array size matches number of buttons
    const unsigned long debounceDelay = 50; // 50ms debounce time

    bool reading = digitalRead(pin) == LOW; // Button is active LOW

    if (reading != *state) {  // State change detected
        unsigned long currentTime = millis();
        if (currentTime - lastPressTime[index] > debounceDelay) {
            lastPressTime[index] = currentTime;  // Update last press time
            *state = reading;  // Update stored state
        }
    }

    return *state; // Return stable state
}
