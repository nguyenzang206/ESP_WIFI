#include <WiFi.h>
#include <esp_wifi.h>

const char* ssid = "ESP32_AP";
const char* password = "12345678";

int lastStationCount = 0;

void setup() {
  Serial.begin(115200);

  // Tạo Wi-Fi Access Point
  WiFi.softAP(ssid, password);

  Serial.println("\nSuccessfully created soft AP");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // Lấy danh sách thiết bị đang kết nối
  wifi_sta_list_t stationList;
  esp_wifi_ap_get_sta_list(&stationList);

  int stationCount = stationList.num;

  // Nếu số lượng thay đổi thì in ra
  if (stationCount != lastStationCount) {
    Serial.print("Number of connected devices: ");
    Serial.println(stationCount);

    for (int i = 0; i < stationList.num; i++) {
      wifi_sta_info_t station = stationList.sta[i];
      Serial.printf("Device %d MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
                    i + 1,
                    station.mac[0], station.mac[1], station.mac[2],
                    station.mac[3], station.mac[4], station.mac[5]);
    }
    Serial.println();
  }

  lastStationCount = stationCount;
  delay(2000);  // kiểm tra mỗi 2 giây
}
