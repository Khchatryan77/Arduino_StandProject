// #include <AccelStepper.h>

// const int start_Stop = 2;
// const int end_Stop = 3;

// // Define stepper motor connections and motor interface type
// #define dirPin  8
// #define stepPin 6
// #define MOTOR_ENABLE_PIN 5
// #define motorInterfaceType 1  // 1 = DRIVER (step/dir interface)

// AccelStepper stepper(motorInterfaceType, stepPin, dirPin);

// void setup() {
//   Serial.begin(9600);
//   pinMode(start_Stop, INPUT_PULLUP);
//   pinMode(end_Stop, INPUT_PULLUP);
//   stepper.setMaxSpeed(16000);      // Steps per second /1000000
//   stepper.setAcceleration(4000);   // Steps per second^2 /50000
//   stepper.moveTo(14000);          
//   pinMode(MOTOR_ENABLE_PIN, OUTPUT);
//   digitalWrite(MOTOR_ENABLE_PIN, LOW); 
// }

// void loop() {

//   stepper.run();
//   // stepper.setMaxSpeed(stepper.maxSpeed() + 10000);   // Increase max speed by 100 steps/sec
//   bool start_Stop1 = digitalRead(start_Stop) == LOW;
//   bool end_Stop1 = digitalRead(end_Stop) == LOW;

//   stepper.move(15000);
//   if (start_Stop1 == 1) {
//     stepper.setCurrentPosition(stepper.currentPosition()); //stoped quickly
//     stepper.stop();
//     while (stepper.isRunning()) {
//       stepper.run();                      
//     }
//     digitalWrite(MOTOR_ENABLE_PIN, HIGH); 

//   }

//   if (end_Stop1 == 1) {
//     stepper.setCurrentPosition(stepper.currentPosition()); //stoped quickly
//     stepper.stop();
//     while (stepper.isRunning()) {
//       stepper.run();                    
//     }
//     digitalWrite(MOTOR_ENABLE_PIN, HIGH);  
//   }

// }
