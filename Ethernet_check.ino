// // Pin assignments for X, Y, Z end stops
// const int start_Stop = 2;
// const int end_Stop = 3;


// void setup() {
//   Serial.begin(9600);

//   // Set end stop pins as input with internal pull-up resistors
//   pinMode(start_Stop, INPUT_PULLUP);
//   pinMode(end_Stop, INPUT_PULLUP);


//   Serial.println("End Stop Test Ready");
// }

// void loop() {
//   // Read end stop states (LOW means pressed if NO type)
//   bool start_Stop1 = digitalRead(start_Stop) == LOW;
//   bool end_Stop1 = digitalRead(end_Stop) == LOW;

//   // Report status
//   Serial.print("start_Stop: ");
//   Serial.print(start_Stop1 ? "PRESSED" : "released");
//   Serial.print(" | Y: ");
//   Serial.println(end_Stop1 ? "PRESSED" : "released");


//   delay(50); // Update every 0.5s
// }
