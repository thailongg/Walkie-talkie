#include<ESPNOWTransport.h>

void Espnow::print_infor() {
  Serial.println("esp32 wroom is sender");
  Serial.printf("peerAddress of revecier ( esp32-s3) : %02x:%02x:%02x:%02x:%02x:%02x\n",
                peerAddress[0], peerAddress[1], peerAddress[2],
                peerAddress[3], peerAddress[4], peerAddress[5]);
}

void Espnow::onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Có thể in log nếu cần
  // Serial.print("Done !");
  // if(serial_send) {
    if (status == ESP_NOW_SEND_SUCCESS) {
        Serial.println(" → Thành công");

    } else {
        Serial.println(" → Thất bại");
    }
  // }
}

void Espnow::start() {
  WiFi.mode(WIFI_STA); 
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(Espnow::onSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, peerAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void Espnow::sendLargeData(const uint8_t *data, size_t len) {
  size_t offset = 0;
  while (offset < len) {
    // if(!send_ON)
    //   break;
    size_t chunk = (len - offset > ESPNOW_MAX_SIZE) ? ESPNOW_MAX_SIZE : (len - offset);
    esp_now_send(Espnow::peerAddress, data + offset, chunk);
    offset += chunk;
    // delay(10);
  }
}

void Espnow::onRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {  
  int buf_size = len/2;
  int16_t buf_recv[buf_size];  
  for(int i = 0; i < buf_size; i++) {
    buf_recv[i] = (int16_t)(incomingData[i*2] | (incomingData[i*2 + 1] << 8));
    // if(Serial_ON)
      // Serial.printf("%ld\n", buf_recv[i]);
  }
  // size_t bytes_written;
  // i2s_write(I2S_NUM_0, sample_received, len/2 * sizeof(int16_t), &bytes_written, portMAX_DELAY);
  AudioTx::write(buf_recv, buf_size);
}

// void Espnow::onRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {  // => 16 bit 
//   int buf_size = len;
//   int16_t buf_recv[buf_size];  
//   for(int i = 0; i < buf_size; i++) {
//     buf_recv[i] = (int16_t)(incomingData[i];
//     // if(Serial_ON)
//       // Serial.printf("%ld\n", buf_recv[i]);
//   }
  // size_t bytes_written;
//   // i2s_write(I2S_NUM_0, sample_received, len/2 * sizeof(int16_t), &bytes_written, portMAX_DELAY);
//   AudioTx::write(buf_recv, buf_size);
// }
