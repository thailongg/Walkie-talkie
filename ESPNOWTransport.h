#pragma once

#include <esp_now.h>
#include <WiFi.h>
#include<AudioTx.h>

#ifdef CONFIG_IDF_TARGET_ESP32  //(esp32_classic_is_sender)
  const uint8_t peerAddress[6] = {0x34, 0x85, 0x18, 0xb5, 0x34, 0xec}; //  => this is peerAddress of esp32_s3
  
#elif CONFIG_IDF_TARGET_ESP32S3 //(esp32_s3_is_sender)
  const uint8_t peerAddress[6] = {0x08, 0xd1, 0xf9, 0x99, 0xf7, 0x38}; //  => this is peerAddress of esp32
#endif

#define ESPNOW_MAX_SIZE 200
#define CHUNK_SIZE      200  // bytes mỗi gói gửi

extern volatile bool serial_on;

enum PacketType : uint8_t {
    PKT_CMD   = 0x01,   // Gói lệnh (bật phát, dừng, ... )
    PKT_AUDIO = 0x02    // Gói dữ liệu audio
};
typedef struct EspNowPacket {
    uint8_t   type;       // Loại gói
    uint8_t   src;        // ID thiết bị (0=Master, 1=Slave)
    // uint16_t  seq;        // Sequence number
    uint16_t  length;     // Độ dài payload
    // uint8_t   payload[200]; // Dữ liệu
}EspNowPacket;

class Espnow {
  private:
      uint8_t peerAddress[6];
  public:
      void print_infor();
      void start_send_mode();
      void start_recv_mode();    
      void sendLargeData(const uint8_t *data, size_t len);
      static void onRecv(const uint8_t *mac, const uint8_t *incomingData, int len);
      static void onSent(const uint8_t *mac_addr, esp_now_send_status_t status);
      esp_err_t sendCommand(uint8_t cmd);
};