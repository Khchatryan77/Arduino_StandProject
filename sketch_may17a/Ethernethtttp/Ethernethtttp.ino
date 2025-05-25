#include <SPI.h>
#include <Ethernet.h>

#define W5500_CS 5
#define W5500_RST 34
#define LED_PIN 2 // Change this to your LED GPIO pin

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 5, 3);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192, 168, 5, 1);
IPAddress dns(8, 8, 8, 8);

EthernetServer server(80); // HTTP listens on port 80

void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // LED OFF initially

  // Ethernet setup
  SPI.begin();
  Ethernet.init(W5500_CS);

  pinMode(W5500_RST, OUTPUT);
  digitalWrite(W5500_RST, HIGH); delay(250);
  digitalWrite(W5500_RST, LOW); delay(250);
  digitalWrite(W5500_RST, HIGH); delay(250);

  Ethernet.begin(mac, ip, dns, gateway, subnet);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("W5500 not found!");
    while (true);
  }

  while (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("No Ethernet cable!");
    delay(500);
  }

  Serial.print("Server IP: ");
  Serial.println(Ethernet.localIP());

  server.begin();
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("New client");
    String req = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        req += c;

        if (c == '\n') {
          // End of header
          if (req.indexOf("GET /on") >= 0) {
            digitalWrite(LED_PIN, HIGH);
          } else if (req.indexOf("GET /off") >= 0) {
            digitalWrite(LED_PIN, LOW);
          }

          // Respond
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<html><body>");
          client.println("<h1>ESP32 W5500 LED</h1>");
          client.println("<p><a href='/on'>Turn ON</a></p>");
          client.println("<p><a href='/off'>Turn OFF</a></p>");
          client.println("</body></html>");
          break;
        }
      }
    }

    delay(1);
    client.stop();
    Serial.println("Client disconnected");
  }
}
