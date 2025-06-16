#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

constexpr uint32_t BAUDRATE = 115200;

//0xa0, 0xdd, 0x6c, 0x10, 0x0e, 0x44
//0x40, 0x22, 0xd8, 0x3b, 0xed, 0x38
//0x9c, 0x9c, 0x1f, 0xc7, 0xa5, 0x20 matt
//0x14, 0x2b, 0x2f, 0xec, 0x28, 0x14 shiny
uint8_t peerAddress[] = {0x9c, 0x9c, 0x1f, 0xc7, 0xa5, 0x20};

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  String myData = "";
  memcpy(&myData, incomingData, sizeof(myData));

  Serial.println(myData);
  Serial.println(len);
}

void setup()
{
  // Init Serial Monitor
  Serial.begin(BAUDRATE);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  // Register peer
  memcpy(peerInfo.peer_addr, peerAddress, 6);
  peerInfo.channel = 1;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop()
{
  
}