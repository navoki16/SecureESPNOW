#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

constexpr uint32_t BAUDRATE = 115200;
constexpr uint8_t KEY_LENGTH = 7;

uint8_t peerAddress[] = {0xa0, 0xdd, 0x6c, 0x10, 0x0e, 0x44};

const uint8_t PMK_KEY_STR[KEY_LENGTH] = {5, 0, 3, 1, 6, 4, 8};
const uint8_t LMK_KEY_STR[KEY_LENGTH] = {7, 1, 1, 0, 6, 8, 2};

esp_now_peer_info_t peerInfo;

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
    String myData = "";
    memcpy(&myData, incomingData, sizeof(myData));

    Serial.println(myData);
    Serial.println(len);
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

    esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop()
{
    // String myData = "Hack me.";
    // esp_err_t result = esp_now_send(peerAddress, (uint8_t *)&myData, sizeof(myData));
    // if (result == ESP_OK)
    // {
    //     Serial.println("Sent with success");
    // }
    // else
    // {
    //     Serial.println("Error sending the data");
    // }
}