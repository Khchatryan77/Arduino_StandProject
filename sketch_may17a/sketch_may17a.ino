#include <SPI.h>

// W5500 SPI CS pin
const int W5500_CS = 10;

// W5500 Register Addresses (Socket 0)
#define COMMON_BASE      0x0000
#define SOCKET0_BASE     0x0400
#define Sn_MR            0x0000  // Mode
#define Sn_PORT0         0x0004  // Source Port
#define Sn_CR            0x0001  // Command
#define Sn_IR            0x0002  // Interrupt
#define Sn_SR            0x0003  // Status
#define Sn_DIPR          0x000C  // Destination IP
#define Sn_DPORT         0x0010  // Destination Port

// Command values
#define OPEN             0x01
#define CONNECT          0x04
#define SEND             0x20
#define CLOSE            0x10

void writeW5500Reg(uint16_t addr, uint8_t ctrlByte, uint8_t data) {
  digitalWrite(W5500_CS, LOW);
  SPI.transfer16(addr);
  SPI.transfer(ctrlByte);
  SPI.transfer(data);
  digitalWrite(W5500_CS, HIGH);
}

void writeW5500Buf(uint16_t addr, uint8_t ctrlByte, uint8_t *buf, uint16_t len) {
  digitalWrite(W5500_CS, LOW);
  SPI.transfer16(addr);
  SPI.transfer(ctrlByte);
  for (int i = 0; i < len; i++) {
    SPI.transfer(buf[i]);
  }
  digitalWrite(W5500_CS, HIGH);
}

uint8_t readW5500Reg(uint16_t addr, uint8_t ctrlByte) {
  digitalWrite(W5500_CS, LOW);
  SPI.transfer16(addr);
  SPI.transfer(ctrlByte);
  uint8_t value = SPI.transfer(0x00);
  digitalWrite(W5500_CS, HIGH);
  return value;
}

void sendCommand(uint8_t command) {
  writeW5500Reg(SOCKET0_BASE + Sn_CR, 0x04, command);
  while (readW5500Reg(SOCKET0_BASE + Sn_CR, 0x04) != 0);
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  pinMode(W5500_CS, OUTPUT);
  digitalWrite(W5500_CS, HIGH);

  delay(1000);
  Serial.println("Starting W5500 RAW TCP Client Example");

  // 1. Set mode register: TCP
  writeW5500Reg(SOCKET0_BASE + Sn_MR, 0x04, 0x01); // TCP

  // 2. Set source port
  writeW5500Reg(SOCKET0_BASE + Sn_PORT0, 0x04, 0x13);       // Port High Byte (5000 >> 8)
  writeW5500Reg(SOCKET0_BASE + Sn_PORT0 + 1, 0x04, 0x88);   // Port Low Byte (5000 & 0xFF)

  // 3. Open socket
  sendCommand(OPEN);
  delay(100);

  // 4. Set destination IP
  uint8_t destIP[4] = {192, 168, 1, 100}; // Replace with your server IP
  writeW5500Buf(SOCKET0_BASE + Sn_DIPR, 0x04, destIP, 4);

  // 5. Set destination port (e.g., 8080)
  writeW5500Reg(SOCKET0_BASE + Sn_DPORT, 0x04, 0x1F);     // High byte of 8080
  writeW5500Reg(SOCKET0_BASE + Sn_DPORT + 1, 0x04, 0x90); // Low byte of 8080

  // 6. Connect to server
  sendCommand(CONNECT);
  delay(500);

  // Check status
  uint8_t status = readW5500Reg(SOCKET0_BASE + Sn_SR, 0x04);
  if (status == 0x17) { // SOCK_ESTABLISHED
    Serial.println("TCP Connection established!");

    // Send "Hello" to server
    uint8_t message[] = "Hello from Arduino!";
    // (Skipping pointer management, direct writing)
    writeW5500Buf(0x4000, 0x14, message, sizeof(message) - 1); // 0x14 = Write to TX buffer
    sendCommand(SEND);
  } else {
    Serial.print("Connection failed. Status: 0x");
    Serial.println(status, HEX);
  }
}

void loop() {
}
