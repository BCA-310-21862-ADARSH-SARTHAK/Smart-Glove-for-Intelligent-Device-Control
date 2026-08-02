//to find the esp32 mac address
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(1000); // Give the Serial Monitor a second to connect

  // Force the Wi-Fi radio to initialize
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100); // Brief pause to let the hardware catch up

  Serial.println();
  Serial.println("--- Boot Successful ---");
  Serial.print("Receiver MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  // Do nothing
}