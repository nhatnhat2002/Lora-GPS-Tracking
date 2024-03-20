#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

#define L_RX 2
#define L_TX 3
SoftwareSerial LoRaSerial(L_RX, L_TX); // RX, TX

#define BAUDRATE 19200

float latitude = 10.850014259532362; // Vĩ độ ban đầu10.850014259532362, 106.77320598602572
float longitude = 106.77320598602572; // Kinh độ ban đầu
unsigned long lastUpdateTime = 0; // Thời điểm cuối cùng cập nhật tọa độ
const float metersPerDegree = 111111; // Khoảng cách (m) tương ứng với mỗi độ

void setup() {
  Serial.begin(9600); // USB Serial
  delay(1000);
  
  LoRaSerial.begin(BAUDRATE); // LoRa Software Serial
}

void loop() {
  // Kiểm tra nếu đã đến thời điểm cập nhật tọa độ mới
  if (millis() - lastUpdateTime >= 5000) {
    // Cập nhật tọa độ mới
    updateCoordinates(10); // Di chuyển 10 mét
    // Gửi dữ liệu tọa độ mới qua LoRa
     String data = "\"lat\":\"";
    data += String(latitude, 6);
    data += "\",\"lng\":\"";
    data += String(longitude, 6);
    data += "\"";
    sendLoraData(data, 2);
    lastUpdateTime = millis(); // Cập nhật thời điểm cuối cùng cập nhật tọa độ
  }

  // Các công việc khác trong loop có thể được thêm ở đây
}

void updateCoordinates(float meters) {
  // Tính toán sự thay đổi trong vĩ độ và kinh độ dựa trên khoảng cách (m)
  // (Đơn giản hóa: không tính hiệu ứng của việc di chuyển theo đường vòng)
  float deltaLatitude = meters / metersPerDegree;
  float deltaLongitude = meters / (metersPerDegree * cos(latitude * PI / 180));

  // Cập nhật tọa độ mới
  latitude += deltaLatitude;
  longitude += deltaLongitude;
}

void sendLoraData(String data, int address) {
  LoRaSerial.println(data); // Gửi dữ liệu qua LoRa
}
