#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>

constexpr uint32_t BAUDRATE = 115200;
constexpr uint8_t LEN_FILTER = 75;
constexpr uint8_t MAC_LENGTH = 6;

uint8_t *mac_dest = nullptr;
uint8_t *mac_src = nullptr;

void wifi_sniffer_packet_handler(void *buff, wifi_promiscuous_pkt_type_t type)
{
    wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buff;

    // Check if the packet length matches the desired value
    if (pkt->rx_ctrl.sig_len == LEN_FILTER)
    {
        Serial.printf("Packet received: Length %d\n", pkt->rx_ctrl.sig_len);

        mac_dest = pkt->payload + 4; // Destination MAC address (offset 4)
        mac_src = pkt->payload + 10; // Source MAC address (offset 10)

        Serial.print("Payload: ");
        for (int i = 0; i < pkt->rx_ctrl.sig_len; i++)
        {
            char c = pkt->payload[i];
            if (isPrintable(c))
            { // Only print readable characters
                Serial.print(c);
            }
            else
            {
                Serial.print('.'); // Replace unreadable characters with dots
            }
        }
        Serial.print("Destination MAC: ");
        for (int i = 0; i < MAC_LENGTH; i++)
        {
            Serial.printf("%02X", mac_dest[i]);
            if (i < (MAC_LENGTH - 1))
            {
                Serial.print(':');
            }
        }
        Serial.println();

        Serial.print("Source MAC: ");
        for (int i = 0; i < MAC_LENGTH; i++)
        {
            Serial.printf("%02X", mac_src[i]);
            if (i < (MAC_LENGTH - 1))
            {
                Serial.print(':');
            }
        }
        Serial.println();
    }
}


void setup()
{
    Serial.begin(BAUDRATE);
    WiFi.mode(WIFI_STA);
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(wifi_sniffer_packet_handler);
}

void loop()
{

}