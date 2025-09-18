#ifdef esp32_wroom_is_sender
  const uint8_t peerAddress[] = {0x08, 0xd1, 0xf9, 0x99, 0xf7, 0x38}; //  => this is peerAddress of esp32
  void print_infor() {
    Serial.println("esp32 wroom is sender");
    Serial.printf("peerAddress of revecier ( esp32-s3) : %02x:%02x:%02x:%02x:%02x:%02x\n",
                  peerAddress[0], peerAddress[1], peerAddress[2],
                  peerAddress[3], peerAddress[4], peerAddress[5]);
  }
  
#else //(esp32_s3_is_rx)
  const uint8_t peerAddress[] = {0x34, 0x85, 0x18, 0xb5, 0x34, 0xec}; //  => this is peerAddress of esp32_s3
  void print_infor() {
    Serial.println("esp32-s3 is sender");
    Serial.printf("peerAddress of revecier ( esp32 wroom) : %02x:%02x:%02x:%02x:%02x:%02x\n",
                    peerAddress[0], peerAddress[1], peerAddress[2],
                    peerAddress[3], peerAddress[4], peerAddress[5]);
  }
#endif

#define ESPNOW_MAX_SIZE 250
#define CHUNK_SIZE      200  // bytes mỗi gói gửi


// void onRecv(const uint8_t* mac, const uint8_t* incomingData, int len) {
//   int16_t sample_received[len / 2];
//   for (int i = 0; i < len / 2; i++) {
//     sample_received[i] = (int16_t)(incomingData[i * 2] | (incomingData[i * 2 + 1] << 8));
//     if (Serial_ON)
//       Serial.printf("%ld\n", sample_received[i]);
//   }
//   size_t bytes_written;
//   i2s_write(I2S_NUM_0, sample_received, len / 2 * sizeof(int16_t), &bytes_written, portMAX_DELAY);
// }

// void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//   // Có thể in log nếu cần
//   // Serial.print("Done !");
//   if (status == ESP_NOW_SEND_SUCCESS) {
//       Serial.println(" → Thành công");

//   } else {
//       Serial.println(" → Thất bại");
//   }
// }