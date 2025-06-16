#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

constexpr uint32_t BAUDRATE = 115200;
constexpr uint8_t MAC_LENGTH = 6;

esp_now_peer_info_t peerInfo;
uint8_t targetMAC[MAC_LENGTH] = {0x14, 0x2b, 0x2f, 0xec, 0x28, 0x14}; // input target mac address here
uint8_t channel = 0;
String payload = "";
bool isEncrypted = false;

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

void GetPayload()
{
    Serial.println("Payload:");
    while (!Serial.available())
    {
        // wait for input
    }

    payload = Serial.readString();
}

void GetEncryptionMode()
{
    Serial.println("Encryted? :");
    while (!Serial.available())
    {
        // wait for input
    }

    isEncrypted = Serial.parseInt();
}

void setup()
{
    Serial.begin(BAUDRATE);
    WiFi.mode(WIFI_STA);
    GetPayload();
    GetEncryptionMode();
    while (esp_now_init() != ESP_OK)
    {
        Serial.println("\rproblem with espnow.");
    }
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
    memcpy(peerInfo.peer_addr, targetMAC, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = isEncrypted;
    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
        return;
    }

    if (!payload.isEmpty())
    {
        esp_err_t result = esp_now_send(targetMAC, (uint8_t *)payload.c_str(), sizeof(payload));
        Serial.println(result == ESP_OK ? "Sent" : "Error sending message");
    }

    Serial.println("Attack done.");
}

void loop()
{
}