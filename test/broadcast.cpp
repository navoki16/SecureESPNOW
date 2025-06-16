#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

constexpr uint32_t BAUDRATE = 115200;

//0xa0, 0xdd, 0x6c, 0x10, 0x0e, 0x44
//0x40, 0x22, 0xd8, 0x3b, 0xed, 0x38
//0xFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF
uint8_t broadcastAddress[] = {0xFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF};

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  uint8_t receivedMessage = 0;
  memcpy(&receivedMessage, incomingData, sizeof(&receivedMessage));
  Serial.print("Received: ");
  Serial.println(receivedMessage);
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
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
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
  String message = "Hack Me.";

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t*)message.c_str(), sizeof(message));
  Serial.println(result == ESP_OK ? "Sent" : "Error sending message");
  
  delay(1000);
}