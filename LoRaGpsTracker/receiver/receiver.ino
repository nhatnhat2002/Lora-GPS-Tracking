#include <SoftwareSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h> 

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* ssid = "Dinhnhat";
const char* password = "25012002";

SoftwareSerial LoRaSerial(18,19); // RX, TX
String endpoint = "https://us-east-1.aws.data.mongodb-api.com/app/application-0-vqinw/endpoint/sensor";
HTTPClient http;

String check = "IN";

void setup() {
Serial.begin(9600); // USB Serial
delay(1000);
LoRaSerial.begin(19200); // LoRa Software Serial 
displaySet();
WiFi.begin(ssid, password);
connectToWiFi();
displayWiFiInfo();
http.begin(endpoint);
delay(5000);
}

void loop() {
  // Check if data is available on Software Serial
  while (LoRaSerial.available()) {
    String data = LoRaSerial.readString();
    setOled();
    String Adata = "{" + data + ", \"stt\": \"" + check + "\"}";
    Serial.println(Adata); // Send data to USB Serial
    displayJsonOnOled(Adata);
    display.display();
    sendData(Adata);
    delay(1000);
  }
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
}

void displayWiFiInfo() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Connected to WiFi:");
  display.println(ssid);
  display.println("IP Address:");
  display.println(WiFi.localIP());
  display.display();
}

void sendData(String dataToSend){
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(dataToSend);
}

void setOled(){
  display.clearDisplay(); 
  display.setCursor(0, 10);
}

void displayJsonOnOled(String jsonString) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, jsonString);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return;
  }

  // Lấy giá trị từ tài liệu JSON
  const char *latitude = doc["lat"];
  const char *longitude = doc["lng"];
  const char *status = doc["stt"];

  // Hiển thị giá trị lên màn hình OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Latitude: ");
  display.println(latitude);
  display.print("Longitude: ");
  display.println(longitude);
  display.print("Status: ");
  display.println(status);
  display.display();
}
void displaySet(){
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.print("Connecting...");
  display.display();
}