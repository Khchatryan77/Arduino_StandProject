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

unsigned long startTime;
const unsigned long timeout = 5000;

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

  // Set end stop pins as input with internal pull-up resistors
  pinMode(start_Stop, INPUT_PULLUP);
  pinMode(end_Stop, INPUT_PULLUP);

  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());

  stepper.setMaxSpeed(1000000);      // Steps per second /1000000
  stepper.setAcceleration(10000);   // Steps per second^2 /50000
  delay(500);
  stepper.move(-1000);  
  delay(500); 
  stepper.move(1000);          
  Serial.print("Void Loop: ");
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
}

void loop() {

  stepper.run(); 
  bool start_Stop1 = digitalRead(start_Stop) == LOW;
  bool end_Stop1 = digitalRead(end_Stop) == LOW;

  EthernetClient client = server.available();
  if (client) {
    Serial.println("Client connected");

    String request = client.readStringUntil('\r');
    Serial.print("Request: ");
    Serial.println(request);
    client.flush();

    startTime = millis();  // timmer started

    if (request.indexOf("GET /MOTOR=START_POS") >= 0) {
      stepper.setCurrentPosition(0);

      digitalWrite(ENA_PIN, LOW);
      startTime = millis();
      if (request.indexOf("GET /MOTOR=START_POS") >= 0 && end_Stop1 == 1) {
        Serial.println("Motor START_POS && start_Stop1");
        stepper.move(350000);
        while ((digitalRead(start_Stop) == HIGH) and bool(millis() - startTime < timeout)){ 
          // timer and startstop check conditions 
          stepper.run();
        } 
        stepper.stop();
        while (stepper.isRunning()) {
          stepper.run();
        stepper.setCurrentPosition(0);  //break
        }
      } else{
        Serial.println("Motor START_POS");
        stepper.move(50000);
        while ((digitalRead(start_Stop) == HIGH) and bool(millis() - startTime < timeout)) { 
          stepper.run();
        // stepper.setCurrentPosition(0); // break
        }
      }
      digitalWrite(ENA_PIN, HIGH);
    }

    if (request.indexOf("GET /MOTOR=END_POS") >= 0) {
      stepper.setCurrentPosition(0);

      digitalWrite(ENA_PIN, LOW);
      
      if (request.indexOf("GET /MOTOR=END_POS") >= 0 && start_Stop1 == 1) {
        
        Serial.println("Motor END_POS && start_Stop1");
        stepper.move(-350000);
        while ((digitalRead(end_Stop) == HIGH) and bool(millis() - startTime < timeout)) {
          // timer and end_Stop check conditions
          stepper.run();
        } 
        stepper.stop();
        while (stepper.isRunning()) {
          stepper.run();
          stepper.setCurrentPosition(0);  //break
        }
      } else{
        Serial.println("Motor END_POS");
        stepper.move(-50000);
        while ((digitalRead(end_Stop) == HIGH) and bool(millis() - startTime < timeout)) {
          // timer and end_Stop check conditions 
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
    client.println("<h1>Motor Controller</h1>");

    bool start = digitalRead(start_Stop);
    bool end = digitalRead(end_Stop);

    if (start && end) {
      client.print("<p><strong>Motor In:</strong> ");
      client.print("<span style='color:red;'>Time Out</span>");
      client.println("</p>");

    }
    else if (end) {
      client.print("<p><strong>Motor In:</strong> ");
      client.print("<span style='color:green;'>START_POS</span>");
      client.println("</p>");
    }
    else if (start) {
      client.print("<p><strong>Motor In:</strong> ");
      client.print("<span style='color:blue;'>END_POS</span>");
      client.println("</p>");
    }
    else {
      client.print("<p><strong>Motor In:</strong> ");
      client.print("<span style='color:red;'>ERROR</span>");
      client.println("</p>");
    }

    // Control links
    client.println("<p><a href=\"/MOTOR=START_POS\"><button>START_POS</button></a></p>");
    client.println("<p><a href=\"/MOTOR=END_POS\"><button>END_POS</button></a></p>");

    client.println("</body>");
    client.println("</html>");

    delay(1);
    client.stop();
    Serial.println("Client disconnected");
    }
}