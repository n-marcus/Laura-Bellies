void showMacAdresses() {
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  // // Variable to store the MAC address
  // uint8_t baseMac[6];

  // // Get MAC address of the WiFi station interface
  // esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
  // Serial.print("Station MAC: ");
  // for (int i = 0; i < 5; i++) {
  //   Serial.printf("%02X:", baseMac[i]);
  // }
  // Serial.printf("%02X\n", baseMac[5]);

  // // Get the MAC address of the Wi-Fi AP interface
  // esp_read_mac(baseMac, ESP_MAC_WIFI_SOFTAP);
  // Serial.print("SoftAP MAC: ");
  // for (int i = 0; i < 5; i++) {
  //   Serial.printf("%02X:", baseMac[i]);
  // }
  // Serial.printf("%02X\n", baseMac[5]);
}