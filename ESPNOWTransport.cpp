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

void Espnow::start_send_mode() {
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

void Espnow::start_recv_mode() {
  WiFi.mode(WIFI_STA); 
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(Espnow::onRecv));
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

// void Espnow::onRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {  
//   int buf_size = len/2;
//   int16_t buf_recv[buf_size];  
//   for(int i = 0; i < buf_size; i++) {
//     buf_recv[i] = (int16_t)(incomingData[i*2] | (incomingData[i*2 + 1] << 8));
//     // if(Serial_ON)
//       // Serial.printf("%ld\n", buf_recv[i]);
//   }
//   // size_t bytes_written;
//   // i2s_write(I2S_NUM_0, sample_received, len/2 * sizeof(int16_t), &bytes_written, portMAX_DELAY);
//   AudioTx::write(buf_recv, buf_size);
// }

void Espnow::onRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {  // => 16 bit 
  int buf_size = len;
  int16_t buf_recv[buf_size];  
  for(int i = 0; i < buf_size; i++) {
    buf_recv[i] = (int16_t)(incomingData[i*2] | (incomingData[i*2 + 1] << 8));
    if(serial_on)
      Serial.printf("%ld\n", buf_recv[i]);
  }
  size_t bytes_written;
  // i2s_write(I2S_NUM_0, sample_received, len/2 * sizeof(int16_t), &bytes_written, portMAX_DELAY);
  AudioTx::write(buf_recv, buf_size);

  // int16_t sample_received[len/2];  
  // for(int i = 0; i < len/2; i++) {
  //   sample_received[i] = (int16_t)(incomingData[i*2] | (incomingData[i*2 + 1] << 8));
  //   if(Serial_ON)
  //     Serial.printf("%ld\n", sample_received[i]);
  // }
  // size_t bytes_written;
  // i2s_write(I2S_NUM_0, sample_received, len/2 * sizeof(int16_t), &bytes_written, portMAX_DELAY);
}

esp_err_t Espnow::sendCommand(uint8_t cmd) {
  EspNowPacket pkt;
  pkt.type = cmd;
  pkt.src = 0; // Master
  // pkt.seq = seqCounter++;
  pkt.length = 1;
  // pkt.payload[0] = cmd; // Ví dụ: 0x01 = START_AUDIO

  esp_err_t result = esp_now_send(slaveMac, (uint8_t *)&pkt, sizeof(pkt));
  if (result == ESP_OK) {
      Serial.println("CMD sent OK");
  } else {
      Serial.println("Error sending CMD");
  }
  return result;
}
