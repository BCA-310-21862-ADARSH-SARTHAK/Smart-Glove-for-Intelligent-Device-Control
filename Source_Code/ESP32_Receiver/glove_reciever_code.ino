//reciever program.
#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
  char command;
} struct_message;

struct_message myData;

// --- UPDATED CALLBACK FOR ESP32 CORE 3.x ---
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  
  // Print ONLY the character. 
  // Your C++ program will read this stream.
  Serial.println(myData.command);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Updated registration function
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Everything happens automatically in the callback
}