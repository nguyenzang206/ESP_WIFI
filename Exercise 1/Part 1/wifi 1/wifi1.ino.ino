#include <WiFi.h>
const char* ssid = "Vicashome 2";
const char* password = "18822956";

//TCP sever
const char* host = "192.168.102.10";
const uint16_t port = 6000;

WiFiClient client;

void setup(){
  Serial.begin(115200);
  delay(1000);

  
  WiFi.begin(ssid, password);
  Serial.println("\nKết nối mạng WiFi ..");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nĐã kết nối với mạng WiFi");
  Serial.println("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  Serial.println("Kết nối với server TCP ...");
  if (client.connect(host, port)) {
  Serial.println("Đã kết nối với server!");
  client.println("Hello from ESP32!");
  } else {
  Serial.println("Kết nối với server thất bại.");
  }
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.length() > 0) {
      if (client.connected()) {
        client.println(input);
        Serial.println("[ESP32] Đã gửi: " + input);
      } else {
        Serial.println("[ESP32] Mất kết nối server, không gửi được.");
      }
    }
  }
  delay(10);
}

