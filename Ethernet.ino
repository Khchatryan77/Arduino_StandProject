// #include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 5, 111); // Use your IP
IPAddress dns(8, 8, 8, 8); // Google DNS
IPAddress gateway(192, 168, 5, 1); // Router IP
IPAddress subnet(255, 255, 255, 0);

EthernetServer server(80);

#define LED_PIN 7
#define STEP_PIN 2
#define DIR_PIN 3
#define ENA_PIN 4

#define W5500_CS 10
#define W5500_RST 5 

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

  //Initializes Ethernet with static IP.
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Error: W5500 hardware not detected");
    while (true);
  }

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
  digitalWrite(DIR_PIN, HIGH);  // direction

  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
  
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
  digitalWrite(ENA_PIN, HIGH);
  Serial.print("Motor Moved");
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Client connected");

    String request = client.readStringUntil('\r');
    Serial.print("Request: ");
    Serial.println(request);
    client.flush();

    // Track motor state
    static bool motorState = false;

    if (request.indexOf("GET /MOTOR=ON") >= 0) {
      digitalWrite(LED_PIN, HIGH);
      motorState = true;
      client.print("<p><strong>Motor Status:</strong> ");
      client.print(motorState ? "<span style='color:green;'>ON</span>" : "<span style='color:red;'>OFF</span>");
      client.println("</p>");
      Serial.println("Motor turned ON");
      rotateSteps(25600, 20, true);
    }

    if (request.indexOf("GET /MOTOR=OFF") >= 0) {
      digitalWrite(LED_PIN, LOW);
      motorState = false;
      client.print("<p><strong>Motor Status:</strong> ");
      client.print(motorState ? "<span style='color:green;'>ON</span>" : "<span style='color:red;'>OFF</span>");
      client.println("</p>");
      Serial.println("Motor turned OFF");
      rotateSteps(3200, 200, false);
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

    // Show current motor status
    // client.print("<p><strong>Motor Status:</strong> ");
    // client.print(motorState ? "<span style='color:green;'>ON</span>" : "<span style='color:red;'>OFF</span>");
    // client.println("</p>");

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
