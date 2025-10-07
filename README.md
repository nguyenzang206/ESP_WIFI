# ESP32 Wi-Fi

## Mục lục
- [Bài tập 1: ESP32 Wi-Fi Client (Station Mode)](https://github.com/nguyenzang206/ESP_WIFI/tree/a3a7206de7db08e7813ce89997f2ff754d4c28fe/Exercise%201/Part%201)

- [Bài tập 2: ESP32 Wi-Fi Access Point (AP Mode)](https://github.com/nguyenzang206/ESP_WIFI/tree/a3a7206de7db08e7813ce89997f2ff754d4c28fe/Exercise%201/Part%202)

- [Bài tập 3: ESP32 Dual Mode (AP + STA)](https://github.com/nguyenzang206/ESP_WIFI/tree/a3a7206de7db08e7813ce89997f2ff754d4c28fe/Exercise%201/Part%203)

## Phần cứng & phần mềm chung
- **Phần cứng:** ESP32 (bất kỳ board nào hỗ trợ Arduino IDE).
- **Phần mềm:**
  - Arduino IDE với ESP32 core (cài qua Board Manager).
  - Serial Monitor (trong Arduino IDE).
  - Python 3.x trên PC để chạy server mẫu. (Part 1)
- **Môi trường:** ESP32 và PC (Part 1) cùng mạng LAN (Wi-Fi).
---

# Bài tập 1: ESP32 Wi-Fi Client (Station Mode)

Tài liệu này mô tả bài tập thực hành với ESP32: Kết nối vào mạng Wi-Fi hiện có (station mode), nhận IP qua DHCP, và hoạt động như TCP client để gửi dữ liệu đến server (PC). Code còn hỗ trợ nhận chuỗi từ Serial Monitor và chuyển tiếp lên server.

Qua bài tập:
- Hiểu cách ESP32 kết nối Wi-Fi và nhận IP DHCP.
- Quan sát kết nối TCP đến server và gửi dữ liệu.
- Theo dõi việc chuyển tiếp dữ liệu từ Serial Monitor lên server.

## Mã nguồn hoàn chỉnh (ESP32 Arduino Sketch)

```cpp
#include <WiFi.h>
const char* ssid = "W_I_F_I";
const char* password = "P_A_S_S";

//TCP sever
const char* host = "IP_PC";
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
```

### Hướng dẫn upload và chạy code

1. Mở Arduino IDE.
2. Chọn board ESP32.
3. Kết nối ESP32 qua USB.
4. Paste code vào, chỉnh `ssid`, `password`, `host` (IP PC), `port` cho phù hợp.
5. Upload (Ctrl+U).
6. Mở Serial Monitor (baud 115200) để theo dõi.

## Giải thích mã nguồn
### Thư viện

- `#include <WiFi.h>`: Thư viện Wi-Fi.

### Khối include & cấu hình

- `const char* ssid = "...";`, `const char* password = "...";`: Chuỗi C bất biến. Dùng cho `WiFi.begin()` để setup AP.
- `const char* host = "IP_PC";`: IP IPv4 của PC chạy server.
- `const uint16_t port = 6000;`: Cổng TCP server lắng nghe.
- `WiFiClient client;`: Đối tượng TCP client, xử lý và hợp lý hóa dữ liệu.

### Cấu hình STA (trong setup())

- `Serial.begin(115200);`: Mở UART CDC với baud 115200 (đồng bộ Serial Monitor).
- `WiFi.begin(ssid, password);`: Bắt đầu kết nối STA mode.
- `while (WiFi.status() != WL_CONNECTED) { ... }`: Poll trạng thái (blocking). In "." mỗi 500ms.
- `Serial.print("ESP32 IP: "); Serial.println(WiFi.localIP());`: In IP do DHCP cấp.
- `Serial.printf(("Kết nối với server TCP ..."));`: Log đích TCP.
- `if (client.connect(host, port)) { ... }`: Kết nối TCP (true nếu ESTABLISHED). Gửi "Hello from ESP32!" nếu thành công.

### Theo dõi và gửi dữ liệu (trong loop())

- `if (Serial.available()) { ... }`: Kiểm tra dữ liệu từ Serial (USB).
- `String input = Serial.readStringUntil('\n');`: Đọc một dòng (blocking đến timeout 1s).
- `input.trim();`: Xóa khoảng trắng đầu/cuối.
- `if (input.length() > 0) { ... }`: Bỏ qua dòng rỗng.
- `if (client.connected()) { ... }`: Kiểm tra socket còn mở. Gửi `client.println(input);` nếu OK.

## Luồng hoạt động (tóm tắt)
1. **Boot → Setup Serial.**
2. **Kết nối Wi-Fi:** `WiFi.begin()` → Đợi `WL_CONNECTED` → Nhận IP DHCP.
3. **In IP:** `WiFi.localIP()`.
4. **TCP Connect:** `client.connect(host, port)` → Gửi "Hello".
5. **Loop:** Nhận dòng từ Serial Monitor → Gửi lên server nếu connected.

## Server mẫu để test (Python)
Chạy trên PC để nhận dữ liệu từ ESP32.

```python
import socket

HOST = "0.0.0.0"   # lắng nghe trên mọi interface
PORT = 6000

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((HOST, PORT))
server.listen(1)
print(f"Server listening on {HOST}:{PORT}")

conn, addr = server.accept()
print("Kết nối từ:", addr)

while True:
    data = conn.recv(1024)
    if not data:
        break
    msg = data.decode(errors="ignore").strip()  # bỏ CRLF
    if msg:
        print("Nhận:", msg)
```

### Chạy server

1. Lưu code vào file `server.py`.
2. Chạy: `python server.py`.
3. Kết quả: Server in "Kết nối từ: (IP_ESP32, port)" và các msg từ ESP32.

---

# Bài tập 2: ESP32 Wi-Fi Access Point (AP Mode)

Bài tập này hướng dẫn cấu hình ESP32 ở chế độ Access Point (AP), nơi ESP32 phát sóng một mạng Wi-Fi riêng biệt (SSID/PASS). Các thiết bị khác như điện thoại, laptop hoặc ESP32 khác có thể kết nối trực tiếp mà không cần router trung gian.

Qua bài tập:
- Hiểu cách ESP32 tạo mạng Wi-Fi cục bộ.
- Quan sát địa chỉ IP mặc định của AP (192.168.4.1).
- Theo dõi danh sách các client (thiết bị) kết nối, bao gồm MAC ID và IP của chúng.

## Mã nguồn hoàn chỉnh (ESP32 Arduino Sketch)

```cpp
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
  delay(2000);
}
```

### Hướng dẫn upload và chạy code

1. Mở Arduino IDE.
2. Chọn board ESP32 (Tools > Board > esp32 > DOIT ESP32 DEVKIT V1).
3. Kết nối ESP32 qua USB.
4. Paste code vào, chỉnh `ssid` và `password` nếu cần.
5. Upload (Ctrl+U).
6. Mở Serial Monitor (baud 115200) để theo dõi log.

## Giải thích mã nguồn

### Thư viện

- `#include <WiFi.h>`: Thư viện Wi-Fi.
- `#include <esp_wifi.h>`: Thư viện nâng cao để truy cập hàm ESP-IDF như lấy danh sách client và MAC.

### Cấu hình AP (trong setup())

- `WiFi.softAP(ssid, pass);`: Khởi tạo ESP32 thành Access Point với SSID và mật khẩu.
- `WiFi.softAPIP();`: Trả về IP của AP

### Lấy danh sách client (trong loop())

- `esp_wifi_ap_get_sta_list(&stationList);`: Lấy danh sách các client.
- `station.mac[]`: Mảng chứa địa chỉ MAC của client.

### In log khi số client thay đổi

- `lastClientCount`: Biến lưu số client vòng lặp trước.
- Nếu `currentCount != lastClientCount`: In số lượng và chi tiết (MAC, IP) của từng client qua vòng lặp `for`. Cập nhật `lastClientCount` để tránh lặp log.

---

# Bài tập 3: ESP32 Dual Mode (AP + STA)

Bài tập này hướng dẫn cấu hình ESP32 ở chế độ Dual Mode, kết hợp cả Access Point (AP) và Station (STA). ESP32 sẽ vừa kết nối tới router Wi-Fi như một thiết bị thông thường (STA), vừa phát sóng mạng Wi-Fi riêng (AP). Điều này biến ESP32 thành một gateway/bridge nhỏ: liên lạc với internet qua router và cho phép các thiết bị khác kết nối trực tiếp vào AP của ESP32.

Qua bài tập:
- Hiểu cách kích hoạt chế độ kép (AP + STA).
- Quan sát IP của cả AP (192.168.4.1) và STA (từ router).
- Theo dõi số lượng client kết nối vào AP.

## Mã nguồn hoàn chỉnh (ESP32 Arduino Sketch)

```cpp
include <WiFi.h>
#include <esp_wifi.h>

const char* ssid = "Vicashome 2";
const char* password = "18822956";
const char* ssidAP = "FAN MU"; // SSID router
const char* passwordAP = "mai dinh 123"; // Pass router

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
  Serial.print("STA SSID: ");
  Serial.println(ssid);
  Serial.print("STA IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("AP SSID: ");
  Serial.println(ssidAP);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

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
```

### Hướng dẫn upload và chạy code

1. Mở Arduino IDE.
2. Chọn board ESP32 (Tools > Board > esp32 > DOIT ESP32 DEVKIT V1).
3. Kết nối ESP32 qua USB.
4. Paste code vào, chỉnh `ssid`, `password`, `ssidAP`, `passwordAP` cho phù hợp.
5. Upload (Ctrl+U).
6. Mở Serial Monitor (baud 115200) để theo dõi log.

## Giải thích mã nguồn

### Thư viện

- `#include <WiFi.h>`: Thư viện Wi-Fi.
- `#include <esp_wifi.h>`: Thư viện nâng cao để truy cập API ESP-IDF như lấy số lượng client.

### Cấu hình Dual Mode (trong setup())

- `WiFi.mode(WIFI_AP_STA);`: Kích hoạt chế độ kép, cho phép ESP32 hoạt động đồng thời như AP và STA.
- `WiFi.softAP(ssidaAP, passwordAP, channel);`: Khởi tạo AP với SSID và mật khẩu.

### Theo dõi client (trong loop())

- `wifi_sta_list_t stationList`: Trả về số lượng client đang kết nối vào AP.
- `static int lastCount = -1;`: Biến tĩnh lưu số client vòng lặp trước (khởi tạo -1 để in lần đầu).
- Nếu `count != lastCount`: In số lượng client và cập nhật biến để tránh spam log.

