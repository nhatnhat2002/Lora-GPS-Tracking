#include <SoftwareSerial.h>

#include <TinyGPSPlus.h>

#define L_RX 2
#define L_TX 3
SoftwareSerial LoRaSerial(L_RX, L_TX); // RX, TX

#define G_RX 5
#define G_TX 4
SoftwareSerial gpsSerial(G_RX, G_TX); // RX, TX

TinyGPSPlus gps;
#define BAUDRATE 19200

void setup() {
  Serial.begin(9600); // USB Serial
  delay(1000);
  
  LoRaSerial.begin(BAUDRATE); // LoRa Software Serial
  gpsSerial.begin(9600); // NEO6M Software Serial
  LoRaSerial.write("hello");
  
}

void loop() {
  // This sketch displays information every time a new sentence is correctly encoded.
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      displayInfo();
    }
  }
  
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
  
}

void displayInfo()
{
  if (gps.location.isValid())
  {    
    String data = "\"lat\":\"";
    data += String(gps.location.lat(), 6);
    data += "\",\"lng\":\"";
    data += String(gps.location.lng(), 6);
    data += "\"";
    
    char charArray[20];    
    data.toCharArray(charArray, sizeof(charArray));
    
    sendLoraData(data, 2);
    Serial.println(data);
    Serial.flush();
    delay(3000);
  }
}

void sendLoraData(String data, int address) {
  String myString =  data ;
  char* buf = (char*) malloc(sizeof(char) * myString.length() + 1);
  //Serial.println(myString);
  myString.toCharArray(buf, myString.length() + 1);
  LoRaSerial.write(buf);
  free(buf);
}


void flushSerialBuffer() {
  while (Serial.available()) {
    Serial.read();
  }
}