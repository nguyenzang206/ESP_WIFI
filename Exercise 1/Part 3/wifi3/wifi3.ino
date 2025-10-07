#include <WiFi.h>
#include <esp_wifi.h>

const char* ssid = "Vicashome 2";
const char* password = "18822956";
const char* ssidAP = "FAN MU";
const char* passwordAP = "mai dinh 123";

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP_STA);

  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to the WiFi Network...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nConnected to the WiFi network!");
  Serial.print("STA IP (connected to router): ");
  Serial.println(WiFi.localIP());

  // Lấy kênh của Wi-Fi chính để dùng cho AP (tránh xung đột)
  int channel = WiFi.channel();

  // Tạo Access Point riêng
  if (!WiFi.softAP(ssidAP, passwordAP, channel)) {
    Serial.println("Soft AP creation failed!");
    while (1);
  }

  Serial.println("\nSuccessfully created Soft AP!");
  Serial.print("AP IP (ESP32's Wi-Fi network): ");
  Serial.println(WiFi.softAPIP());

  Serial.println("\n=== ESP32 Wi-Fi Information ===");
  Serial.print("STA SSID: "); Serial.println(ssid);
  Serial.print("STA IP: "); Serial.println(WiFi.localIP());
  Serial.print("AP SSID: "); Serial.println(ssidAP);
  Serial.print("AP IP: "); Serial.println(WiFi.softAPIP());
  Serial.println("===============================");
}

void loop() {
  // Lấy danh sách thiết bị đang kết nối đến ESP32 AP
  wifi_sta_list_t stationList;
  esp_wifi_ap_get_sta_list(&stationList);

  static int lastCount = -1;
  int count = stationList.num;

  if (count != lastCount) {
    Serial.print("Number of connected devices: ");
    Serial.println(count);

    // In địa chỉ MAC của từng thiết bị
    for (int i = 0; i < count; i++) {
      wifi_sta_info_t station = stationList.sta[i];
      Serial.printf("Device %d MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
                    i + 1,
                    station.mac[0], station.mac[1], station.mac[2],
                    station.mac[3], station.mac[4], station.mac[5]);
    }
    Serial.println();
  }

  lastCount = count;
  delay(2000);
}
