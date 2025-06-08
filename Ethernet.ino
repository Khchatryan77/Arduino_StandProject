#include <SPI.h>
#include <Ethernet.h>
#include <AccelStepper.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 5, 111); // Use your IP
IPAddress dns(8, 8, 8, 8); // Google DNS
IPAddress gateway(192, 168, 5, 1); // Router IP
IPAddress subnet(255, 255, 255, 0);

EthernetServer server(80);

const int start_Stop = 2;
const int end_Stop = 3;

#define LED_PIN 5
#define STEP_PIN 6
#define DIR_PIN 8
#define ENA_PIN 4

#define motorInterfaceType 1  // 1 = DRIVER (step/dir interface)

AccelStepper stepper(motorInterfaceType, STEP_PIN, DIR_PIN);

#define End_stop1 2
#define End_stop2 3

#define W5500_CS 10
#define W5500_RST 9 

static bool motorState = false;

void setup() {
  
  Serial.begin(9600);

  // Optional W5500 reset
  pinMode(W5500_RST, OUTPUT);
  digitalWrite(W5500_RST, HIGH);
  delay(250);
  digitalWrite(W5500_RST, LOW);
  delay(250);
  digitalWrite(W5500_RST, HIGH);
  delay(250);

  // // Serial.println("Initializing Ethernet...");
  // Ethernet.begin(mac, ip, dns, gateway, subnet);
  // delay(2000); // Wait for Ethernet to stabilize

  // if (Ethernet.begin(mac) == 0) {
  //   Serial.println("DHCP failed, using static IP...");
  //   Ethernet.begin(mac, ip, dns, gateway, subnet); // fallback to static
  // }

  // Initializes Ethernet with static IP.
  // if (Ethernet.hardwareStatus() == EthernetNoHardware) {
  //   Serial.println("Error: W5500 hardware not detected");
  //   while (true);
  // }

  Serial.print("Ethernet initialized, IP: ");
  Serial.println(Ethernet.localIP());
  Serial.print("Link status: ");
  Serial.println(Ethernet.linkStatus() == LinkON ? "ON" : "OFF");


  Ethernet.begin(mac, ip);
  server.begin();
  pinMode(LED_PIN, OUTPUT);

  pinMode(STEP_PIN, OUTPUT);
  pinMode(ENA_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  // digitalWrite(DIR_PIN, HIGH);  // direction

  // Set end stop pins as input with internal pull-up resistors
  pinMode(start_Stop, INPUT_PULLUP);
  pinMode(end_Stop, INPUT_PULLUP);

  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());

  stepper.setMaxSpeed(1000000);      // Steps per second /1000000
  stepper.setAcceleration(10000);   // Steps per second^2 /50000
  stepper.moveTo(-1400);          

  digitalWrite(ENA_PIN, LOW); 

}


void rotateSteps(int steps, int pulseDelayMicros, bool direction) {
  digitalWrite(DIR_PIN, direction);
  digitalWrite(ENA_PIN, LOW);
  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(pulseDelayMicros);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(pulseDelayMicros);
  }
  // digitalWrite(ENA_PIN, HIGH);
  // Serial.print("Motor Moved");
}

void loop() {

  stepper.run(); 
  bool start_Stop1 = digitalRead(start_Stop) == LOW;
  bool end_Stop1 = digitalRead(end_Stop) == LOW;


  // if (start_Stop1 == 1){
  //   stepper.setCurrentPosition(stepper.currentPosition()); //stoped quickly
  //   stepper.stop();
  //   while (stepper.isRunning()) {
  //     stepper.run();                      
  //   }
  //   // digitalWrite(ENA_PIN, HIGH); 
  // }
  // if (end_Stop1 == 1){
  //   stepper.setCurrentPosition(stepper.currentPosition()); //stoped quickly
  //   stepper.stop();
  //   while (stepper.isRunning()) {
  //     stepper.run();                      
  //   }
  //   // digitalWrite(ENA_PIN, HIGH); 
  // }
  // Serial.print("start_Stop: ");
  // Serial.print(start_Stop1 ? "PRESSED" : "released");
  // Serial.print(" | end_Stop: ");
  // Serial.println(end_Stop1 ? "PRESSED" : "released");
 
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Client connected");

    String request = client.readStringUntil('\r');
    Serial.print("Request: ");
    Serial.println(request);
    client.flush();

    // // Track motor state
    // static bool motorState = false;

    // if (start_Stop1 == 1){
    //   // stepper.stop(); //stoped quickly
    //   stepper.setCurrentPosition(stepper.currentPosition()); //stoped quickly
    // }
      // stepper.setCurrentPosition(stepper.currentPosition()); //stoped quickly
    // if (request.indexOf("GET /MOTOR=OFF") >= 0 && start_Stop1 == 1) {
    //   digitalWrite(ENA_PIN, LOW);
    //   motorState = false;
    //   client.print("<p><strong>Motor Status:</strong> ");
    //   client.print(motorState ? "<span style='color:green;'>ON</span>" : "<span style='color:red;'>OFF</span>");
    //   client.println("</p>");
    //   Serial.println("Motor turned OFF");
    //   stepper.moveTo(-15000);
    //   if (start_Stop1 == 1) {
    //     stepper.stop();
    //     stepper.setCurrentPosition(0);
    //   } else {
    //     stepper.runToPosition();
    //   }
    // }

    // if (request.indexOf("GET /MOTOR=ON") >= 0 && end_Stop1 == 1) {
    //   digitalWrite(ENA_PIN, LOW);
    //   motorState = true;
    //   client.print("<p><strong>Motor Status:</strong> ");
    //   client.print(motorState ? "<span style='color:green;'>ON</span>" : "<span style='color:red;'>OFF</span>");
    //   client.println("</p>");
    //   Serial.println("Motor turned ON");
    //   stepper.moveTo(1500);
    //   // stepper.runToPosition();
    //   if (stepper.distanceToGo() != 0)
    //      stepper.run();
    // }
    
    // if (request.indexOf("GET /MOTOR=ON") >= 0) {
    //   motorState = true;
    //   client.print("<p><strong>Motor Status:</strong> ");
    //   client.print(motorState ? "<span style='color:green;'>ON</span>" : "<span style='color:red;'>OFF</span>");
    //   client.println("</p>");
    //   Serial.println("Motor turned ON");
    //   stepper.move(1500);
    //   // stepper.runToPosition();
    // }

    if (request.indexOf("GET /MOTOR=ON") >= 0) {
      motorState = true;
      stepper.setCurrentPosition(0);

      client.print("<p><strong>Motor Status:</strong> ");
      client.print(motorState ? "<span style='color:green;'>ON</span>" : "<span style='color:red;'>OFF</span>");
      client.println("</p>");

      digitalWrite(ENA_PIN, LOW);
      
      if (request.indexOf("GET /MOTOR=ON") >= 0 && end_Stop1 == 1) {
        
        Serial.println("Motor turned ON && start_Stop1");
        stepper.move(35000);
        while (digitalRead(start_Stop) == HIGH) {
          stepper.run();
        } 
        stepper.stop();
        while (stepper.isRunning()) {
          stepper.run();
        stepper.setCurrentPosition(0);  //break
        }
      } else{
        Serial.println("Motor turned ON");
        stepper.move(4500);
        while (digitalRead(start_Stop) == HIGH) {
          stepper.run();
        // stepper.setCurrentPosition(0); // break
        }
      }
      digitalWrite(ENA_PIN, HIGH);
    }

    if (request.indexOf("GET /MOTOR=OFF") >= 0) {
      motorState = false;
      stepper.setCurrentPosition(0);

      client.print("<p><strong>Motor Status:</strong> ");
      client.print(motorState ? "<span style='color:green;'>ON</span>" : "<span style='color:red;'>OFF</span>");
      client.println("</p>");

      digitalWrite(ENA_PIN, LOW);
      
      if (request.indexOf("GET /MOTOR=OFF") >= 0 && start_Stop1 == 1) {
        
        Serial.println("Motor turned OFF && start_Stop1");
        stepper.move(-35000);
        while (digitalRead(end_Stop) == HIGH) {
          stepper.run();
        } 
        stepper.stop();
        while (stepper.isRunning()) {
          stepper.run();
          stepper.setCurrentPosition(0);  //break
        }
      } else{
        Serial.println("Motor turned OFF");
        stepper.move(-4500);
        while (digitalRead(end_Stop) == HIGH) {
          stepper.run();
          // stepper.setCurrentPosition(0); // break
        }
      }
      digitalWrite(ENA_PIN, HIGH);
    }

    // Build and send HTML response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head><title>Motor Controller</title></head>");
    client.println("<body>");
    client.println("<h1>MOTOR Controller</h1>");

    // Control links
    client.println("<p><a href=\"/MOTOR=ON\"><button>Turn ON</button></a></p>");
    client.println("<p><a href=\"/MOTOR=OFF\"><button>Turn OFF</button></a></p>");

    client.println("</body>");
    client.println("</html>");

    delay(1);
    client.stop();
    Serial.println("Client disconnected");
  }
}
