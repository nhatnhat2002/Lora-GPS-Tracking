#include <SoftwareSerial.h>

SoftwareSerial LoRaSerial(18,19); // RX, TX

void setup() {
  Serial.begin(19200); // USB Serial
  delay(1000);
  
  LoRaSerial.begin(19200); // LoRa Software Serial 
}

void loop() {
  // Check if data is available on Software Serial
  while (LoRaSerial.available()) {
    String data = LoRaSerial.readString();
    //String response = LoRaSerial.readStringUntil('\n');
    Serial.println(data); // Send data to USB Serial
  }
}