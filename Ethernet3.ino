// #include <Ethernet.h>
// #include <ArduinoJson.h>

// byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
// IPAddress ip(192, 168, 137, 217); // Arduino IP
// IPAddress dns(8, 8, 8, 8); // Google DNS
// IPAddress gateway(192, 168, 5, 5); // Router IP
// IPAddress subnet(255, 255, 255, 0);

// EthernetServer server(80);
// EthernetClient googleClient;
// const char* serverHost = "192.168.137.217";      // Or IP like "192.168.1.100"
// const char* serverPath = "/api/receive-data";   // Your HTTP endpoint

// #define LED_PIN 7

// void setup() {
//   Serial.begin(9600);
//   while (!Serial) continue;

//   Serial.println("Initializing Ethernet...");
//   Ethernet.begin(mac, ip, dns, gateway, subnet);
//   delay(2000);

//   pinMode(LED_PIN, OUTPUT);
//   delay(50)

//   //Initializes Ethernet with static IP.
//   if (Ethernet.hardwareStatus() == EthernetNoHardware) {
//     Serial.println("Error: W5500 hardware not detected");
//     while (true);
//   }

//   Serial.print("Ethernet initialized, IP: ");
//   Serial.println(Ethernet.localIP());
//   Serial.print("Link status: ");
//   Serial.println(Ethernet.linkStatus() == LinkON ? "ON" : "OFF");

//   server.begin();
//   Serial.println("Server started at 192.168.1.6:80");
// }

// void loop() {
// // Update sensorValue (example: increment for testing)
//   sensorValue = (sensorValue % 100) + 1; // Cycles between 1 and 100
//   // Replace with sensor reading, e.g.:
//   // sensorValue = (int)(analogRead(A0) / 10.24); // Analog sensor
//   // sensorValue = (int)dht.readTemperature(); // DHT sensor

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
//         body += c;
//       }
//       if (c == '\n' && request.endsWith("\r\n\r\n")) {
//         isBody = true;
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

//     // Create JSON with variable integer
//     StaticJsonDocument<200> jsonDoc;
//     jsonDoc["sensor"] = "temperature";
//     jsonDoc["value"] = sensorValue; // Integer variable
//     jsonDoc["unit"] = "C";

//     String jsonData;
//     serializeJson(jsonDoc, jsonData);

//     // Send JSON to Google (requires internet)
//     if (googleClient.connect(serverHost, 80)) {
//       Serial.println("Connected to server");

//       googleClient.println("POST " + String(serverPath) + " HTTP/1.1");
//       googleClient.println("Host: " + String(serverHost));
//       googleClient.println("Content-Type: application/json");
//       googleClient.print("Content-Length: ");
//       googleClient.println(jsonData.length());
//       googleClient.println("Connection: close");
//       googleClient.println();
//       googleClient.println(jsonData);


//       while (googleClient.connected()) {
//         if (googleClient.available()) {
//           String line = googleClient.readStringUntil('\n');
//           Serial.println(line);
//         }
//       }
//       googleClient.stop();
//       Serial.println("Google connection closed");
//     } else {
//       Serial.println("Failed to connect to Google");
//     }

//     // Respond to client
//     client.println("HTTP/1.1 200 OK");
//     client.println("Content-Type: application/json");
//     client.println("Connection: close");
//     client.print("Content-Length: ");
//     client.println(jsonData.length());
//     client.println();
//     client.println(jsonData);

//     delay(10);
//     client.stop();
//     Serial.println("Client disconnected");
//   }
// }