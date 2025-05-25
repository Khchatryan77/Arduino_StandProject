// const int STEP_PIN = 2;
// const int DIR_PIN = 3;

// void setup() {
//   Serial.begin(9600);
//   pinMode(STEP_PIN, OUTPUT);
//   pinMode(DIR_PIN, OUTPUT);

//   digitalWrite(DIR_PIN, HIGH);  // Set direction

//   long testSteps[] = {1000, 5000, 10000, 20000, 40000, 64000, 100000, 200000}; // Add more if needed

//   for (int i = 0; i < sizeof(testSteps) / sizeof(testSteps[0]); i++) {
//     long steps = testSteps[i];
//     Serial.print("Testing steps: ");
//     Serial.println(steps);
//     rotateSteps(steps, 100);  // 500 µs delay per step
//     delay(2000);              // Rest between tests
//   }

//   Serial.println("Test completed.");
// }

// void loop() {
//   // Nothing here
// }

// void rotateSteps(long steps, int delayMicros) {
//   for (long i = 0; i < steps; i++) {
//     digitalWrite(STEP_PIN, HIGH);
//     delayMicroseconds(delayMicros);
//     digitalWrite(STEP_PIN, LOW);
//     delayMicroseconds(delayMicros);
//   }
// }
