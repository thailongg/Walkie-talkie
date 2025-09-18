#include <esp_now.h>
#include <WiFi.h>
#include "Audio.h"
#include "evil_laugh.h"

#define serial_pin      GPIO_NUM_14
#define music_pin       GPIO_NUM_15
#define BUFFER_SIZE    250

// const uint16_t ok_audio[] = { 0x4e49,0x4f46,0x5349,0x5446,0xe,0x0,0x614c,0x6676,0x3036,0x312e,0x2e36,0x3031,0x30};
// const int16_t ok_audio[] = { 0x494e,0x464f,0x4953,0x4654,0xe00,0x0,0x4c61,0x7666,0x3630,0x2e31,0x362e,0x3130,0x3000};

volatile bool Serial_ON = false;
volatile bool Music_play = false;
volatile uint8_t count = 0;

typedef struct data_format{
  int seg = 0;
  int chunk = 0;
  int16_t data_send[BUFFER_SIZE];
} data_format;

void IRAM_ATTR handleButton_1() {
  // Serial_ON = !Serial_ON; // chỉ gán cờ, không xử lý lâu
  if(count < 4 )
    count ++;
  else
    count = 0;
  // Serial.println( Serial_ON ? "Serial ON" : "Serial OFF");
}
void IRAM_ATTR handleButton_2() {
  Music_play = !Music_play; // chỉ gán cờ, không xử lý lâu
}
void onRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {  
  int16_t sample_received[len/2];  
  for(int i = 0; i < len/2; i++) {
    sample_received[i] = (int16_t)(incomingData[i*2] | (incomingData[i*2 + 1] << 8));
    if(Serial_ON)
      Serial.printf("%ld\n", sample_received[i]);
  }
  size_t bytes_written;
  i2s_write(I2S_NUM_0, sample_received, len/2 * sizeof(int16_t), &bytes_written, portMAX_DELAY);
}

// void onRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {  
//   data_format package;
//   memcpy(&package, incomingData, sizeof(package));
//   // int16_t sample_received[len/2];  
//   if(count == 1) {
//     for(int i = 0; i < len; i++)
//       Serial.printf("%ld\n", package.data_send[i]);
//   } 
//   else if(count == 2) {
//     // for(int i = 0; i < len; i++)
//       Serial.printf("seg: %d - chuck: %d\n", package.seg, package.chunk);
//   }
//   else if(count == 3) {
//     for(int i = 0; i < len; i++) {
//       Serial.printf("seg: %d\n", package.seg);
//       Serial.printf("%ld\n", package.data_send[i]);
//     }
//   }
//   size_t bytes_written;
//   i2s_write(I2S_NUM_0,  package.data_send, len * sizeof(int16_t), &bytes_written, portMAX_DELAY);
// }

void setup() {
  Serial.begin(115200);
  pinMode(serial_pin, INPUT_PULLUP);
  pinMode(music_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(serial_pin), handleButton_1, FALLING);
  attachInterrupt(digitalPinToInterrupt(music_pin), handleButton_2, FALLING);
  init_TX();
  // init_RX();
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(onRecv));
  delay(1000);
}

void loop() {
  // Không cần code gì, callback sẽ lo phát âm thanh
  if(Music_play) {
    // i2s_start(I2S_NUM_0);
    play_doremi();
    Music_play = false;
    // i2s_stop(I2S_NUM_0);
  }

  size_t bytes_written;
  i2s_write(I2S_NUM_0, evil_laugh_sample, sizeof(evil_laugh_sample), &bytes_written, portMAX_DELAY);
  delay(1000);
  // int32_t i2s_read_buff[I2S_READ_LEN];
  // int16_t i2s_out_buff[I2S_READ_LEN];
  // size_t bytes_read;
  // i2s_read(I2S_NUM_1, (void*)i2s_read_buff, I2S_READ_LEN * sizeof(int32_t), &bytes_read, portMAX_DELAY);
  // // Chuyển từ 32-bit xuống 16-bit để tiết kiệm băng thông
  
  // size_t index = bytes_read / sizeof(int32_t);
  // if(index == I2S_READ_LEN) {  
  //   for (int i = 0; i < index; i++) {
  //     i2s_out_buff[i] = (int16_t)(*((int32_t*)&i2s_read_buff[i]) >> 16); 
  //     if(Serial_ON)
  //       Serial.printf("%ld\n", i2s_out_buff[i]);
  //   }
  //   size_t bytes_write;
  //   // i2s_start(I2S_NUM_0);
  //   i2s_write(I2S_NUM_0, i2s_out_buff, index * sizeof(int16_t), &bytes_write, portMAX_DELAY);
  //   // i2s_stop(I2S_NUM_0);
  // }
}
