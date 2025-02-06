// Joystick pins
const int Vx = A0;  // Joystick X-axis
const int Vy = A1;  // Joystick Y-axis
const int SW = 2;   // Joystick push button

// Button pins
const int leftC = 5;  // Left-click button
const int rightC = 6; // Right-click button

// Dead zone threshold to eliminate stick drift
const int drift = 3;

void setup() {
    // Configure joystick and button pins
    pinMode(SW, INPUT_PULLUP);
    pinMode(leftC, INPUT_PULLUP);
    pinMode(rightC, INPUT_PULLUP);

    // Start serial communication
    Serial.begin(9600);
}

void loop() {
    // Read joystick values
    int x = analogRead(Vx);
    int y = analogRead(Vy);

    // Map joystick values to a range
    int mappedX = map(x, 0, 1023, -10, 10);
    int mappedY = map(y, 0, 1023, 10, -10);  // Inverted Y-axis for natural movement

    // Apply dead zone filter
    if (abs(mappedX) < drift) mappedX = 0;
    if (abs(mappedY) < drift) mappedY = 0;

    // Read buttons directly (Active LOW)
    bool leftClick = digitalRead(leftC) == LOW;
    bool rightClick = digitalRead(rightC) == LOW;

    // Send data to serial (Make sure Python can read it)
    Serial.print(mappedX);
    Serial.print(",");
    Serial.print(mappedY);
    Serial.print(",");
    Serial.print(leftClick);
    Serial.print(",");
    Serial.println(rightClick);  // Use println() to signal end of message

    delay(10); // Reduce data flood
}
