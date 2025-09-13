#pragma once

#include <esp_now.h>
#include <WiFi.h>
#include<AudioTx.h>

#ifdef CONFIG_IDF_TARGET_ESP32  //(esp32_classic_is_sender)
  const uint8_t peerAddress[6] = {0x34, 0x85, 0x18, 0xb5, 0x34, 0xec}; //  => this is peerAddress of esp32_s3
  // void print_infor() {
  //   Serial.println("esp32 wroom is sender");
  //   Serial.printf("peerAddress of revecier ( esp32-s3) : %02x:%02x:%02x:%02x:%02x:%02x\n",
  //                 peerAddress[0], peerAddress[1], peerAddress[2],
  //                 peerAddress[3], peerAddress[4], peerAddress[5]);
  // }
  
#elif CONFIG_IDF_TARGET_ESP32S3 //(esp32_s3_is_sender)
  const uint8_t peerAddress[6] = {0x08, 0xd1, 0xf9, 0x99, 0xf7, 0x38}; //  => this is peerAddress of esp32
  // void print_infor() {
  //   Serial.println("esp32-s3 is sender");
  //   Serial.printf("peerAddress of revecier ( esp32 wroom) : %02x:%02x:%02x:%02x:%02x:%02x\n",
  //                   peerAddress[0], peerAddress[1], peerAddress[2],
  //                   peerAddress[3], peerAddress[4], peerAddress[5]);
  // }
#endif

#define ESPNOW_MAX_SIZE 250
#define CHUNK_SIZE      200  // bytes mỗi gói gửi

class Espnow {
  private:
      uint8_t peerAddress[6];
  public:
      void print_infor();
      void start();    
      void sendLargeData(const uint8_t *data, size_t len);
      static void onRecv(const uint8_t *mac, const uint8_t *incomingData, int len);
      static void onSent(const uint8_t *mac_addr, esp_now_send_status_t status);
};