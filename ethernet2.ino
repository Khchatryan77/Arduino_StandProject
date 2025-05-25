// #include <Ethernet.h>
// #include <ArduinoJson.h>

// byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
// IPAddress ip(192, 168, 137, 217); // Arduino IP
// IPAddress dns(8, 8, 8, 8); // Google DNS
// IPAddress gateway(192, 168, 5, 5); // Router IP
// IPAddress subnet(255, 255, 255, 0);

// EthernetServer server(80); // HTTP server on port 80

// void setup() {
//   Serial.begin(9600);
//   while (!Serial) continue;

//   // Serial.println("Initializing Ethernet...");
//   Ethernet.begin(mac, ip, dns, gateway, subnet);
//   delay(2000); // Wait for Ethernet to stabilize

//   // if (Ethernet.begin(mac) == 0) {
//   //   Serial.println("DHCP failed, using static IP...");
//   //   Ethernet.begin(mac, ip, dns, gateway, subnet); // fallback to static
//   // }

//   if (Ethernet.hardwareStatus() == EthernetNoHardware) {
//     Serial.println("Error: W5500 hardware not detected");
//     while (true); // Halt
//   }

//   Serial.print("Ethernet initialized, IP: ");
//   Serial.println(Ethernet.localIP());
//   Serial.print("Link status: ");
//   Serial.println(Ethernet.linkStatus() == LinkON ? "ON" : "OFF");

//   server.begin(); // Start the server
//   Serial.println("Server started at 192.168.5.6:80");
// }

// void loop() {
//   EthernetClient client = server.available();
//   if (client) {
//     Serial.println("Client connected");

//     String request = "";
//     String body = "";
//     bool isBody = false;
//     while (client.available()) {
//       char c = client.read();
//       request += c;
//       if (isBody) {
//         body += c; // Collect body after headers
//       }
//       if (c == '\n' && request.endsWith("\r\n\r\n")) {
//         isBody = true; // Headers end with double CRLF
//       }
//     }
//     Serial.print("Full request: [");
//     Serial.print(request);
//     Serial.println("]");

//     if (isBody && body.length() > 0) {
//       StaticJsonDocument<200> doc;
//       DeserializationError error = deserializeJson(doc, body);
//       if (!error) {
//         const char* sensor = doc["sensor"];
//         float value = doc["value"];
//         Serial.print("Received JSON - Sensor: ");
//         Serial.print(sensor);
//         Serial.print(", Value: ");
//         Serial.println(value);
//       } else {
//         Serial.print("JSON parsing failed: ");
//         Serial.println(error.c_str());
//       }
//     } else {
//       Serial.println("No JSON body found");
//     }

//     // Send JSON response
//     StaticJsonDocument<200> responseDoc;
//     responseDoc["status"] = "received";
//     responseDoc["message"] = "Hello from Arduino!";
//     responseDoc["timestamp"] = millis();

//     String response;
//     serializeJson(responseDoc, response);

//     client.println("HTTP/1.1 200 OK");
//     client.println("Content-Type: application/json");
//     client.println("Connection: close");
//     client.print("Content-Length: ");
//     client.println(response.length());
//     client.println();
//     client.println(response);

//     delay(10); // Ensure response is sent
//     client.stop();
//     Serial.println("Client disconnected");
//   }
// }