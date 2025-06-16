#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

constexpr uint32_t BAUDRATE = 115200;
constexpr uint8_t KEY_LENGTH = 7;

uint8_t peerAddress[] = {0x24, 0x0a, 0xc4, 0xfa, 0x80, 0x38};

const uint8_t PMK_KEY_STR[KEY_LENGTH] = {5, 0, 3, 1, 6, 4, 8};
const uint8_t LMK_KEY_STR[KEY_LENGTH] = {7, 1, 1, 0, 6, 8, 2};

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup()
{
    Serial.begin(BAUDRATE);
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("There was an error initializing ESP-NOW");
        return;
    }
    esp_now_set_pmk((uint8_t *)PMK_KEY_STR);

    memcpy(peerInfo.peer_addr, peerAddress, 6);
    peerInfo.channel = 1;
    for (uint8_t i = 0; i < KEY_LENGTH; i++)
    {
        peerInfo.lmk[i] = LMK_KEY_STR[i];
    }
    peerInfo.encrypt = true;

    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
        return;
    }

    esp_now_register_send_cb(OnDataSent);
}

void loop()
{
    String myData = "Hack me.";
    esp_err_t result = esp_now_send(peerAddress, (uint8_t *)&myData, sizeof(myData));
    if (result == ESP_OK)
    {
        Serial.println("Sent with success");
    }
    else
    {
        Serial.println("Error sending the data");
    }

    delay(3000);
}