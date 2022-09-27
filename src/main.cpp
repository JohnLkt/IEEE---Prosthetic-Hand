
#include <esp_now.h>
#include <Wifi.h>

// Reciever MAC Address
uint8_t receiverAddress[] = {0x08, 0x3A, 0xF2, 0x52, 0x80, 0xB0};

// Message Format
typedef struct struct_message {
    bool state;
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;
// Callback When Data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup(){
    Serial.begin(115200);

    // Set wifi station
    WiFi.mode(WIFI_STA);

    // Init ESP NOW

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error ESP NOW Init");
        return;
    }
    
    esp_now_register_send_cb(OnDataSent);

    // Register Peer
    memcpy(peerInfo.peer_addr, receiverAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    // add Peer
    if(esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }
}

void loop() {
    // Set Values to send
    myData.state = false;

    // Send via ESP NOW
    esp_err_t result = esp_now_send(receiverAddress, (uint8_t *) &myData, sizeof(myData));

    if (result == ESP_OK) {
        Serial.println("Sent Success");
    } else {
        Serial.println("Error sending");
    }
    delay(2000);
}