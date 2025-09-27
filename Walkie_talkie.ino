#include<I2SConfig.h>
#include<AudioRx.h>

#include<ESPNOWTransport.h>
#include<Button.h>
#include<ok_88k2_8b.h>

#define use_DAC_TX
#ifdef use_DAC_TX 
  #include<DAC_AudioTx.h>
#else 
  #include<AudioTx.h>
// #include<audio_engine_start_88k2_8b.h>
#endif

#define buf_size        1024
#define music_pin       GPIO_NUM_19
#define packet_pin      GPIO_NUM_14

volatile bool Music_play = false;
volatile bool send_packet = false;



void IRAM_ATTR handleButton_1() {
  Music_play = true; // chỉ gán cờ, không xử lý lâu
}
void IRAM_ATTR handleButton_2() {
  send_packet = true; // chỉ gán cờ, không xử lý lâu
}

int32_t buf_read[buf_size];

AudioTx PCM5102A;
AudioRx INMP441;
Espnow  Transport;

// Button btn1(14);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(music_pin, INPUT_PULLUP);
  pinMode(serial_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(music_pin), handleButton_1, FALLING);
  attachInterrupt(digitalPinToInterrupt(packet_pin), handleButton_2, FALLING);                             
  PCM5102A.start();
  // INMP441.start();
  Transport.start_recv_mode();
}

void loop() {
  if( send_packet) {
    if(Transport.sendCommand(1) == ESP_OK)
      send_packet = false;
  }
  // if( Music_play) {
    // PCM5102A.play_doremi();
    // delay(1500);
  //   Music_play = false;
  // }
  // PCM5102A.write()
  size_t bytes_write = 0;
  // i2s_write(I2S_NUM_1, ok_8k_16b_volume, sizeof(ok_8k_16b_volume), &bytes_write, portMAX_DELAY);
  // i2s_write(I2S_NUM_1, ok_8k_16b, sizeof(ok_8k_16b), &bytes_write, portMAX_DELAY);


  Serial.println("Play 16b !");
  // i2s_write(I2S_NUM_1, audio_engine_start_88k2_8b, sizeof(audio_engine_start_88k2_8b), &bytes_write, portMAX_DELAY);
  PCM5102A.write((uint8_t *)ok_88k2_8b, sizeof(ok_88k2_8b));
  delay(1500);

  // Serial.println("Play 16b !");
  // // i2s_write(I2S_NUM_1, audio_engine_start_88k2_8b, sizeof(audio_engine_start_88k2_8b), &bytes_write, portMAX_DELAY);
  // PCM5102A.DAC_write((uint8_t *)audio_engine_start_88k2_8b, sizeof(audio_engine_start_88k2_8b));
  // delay(1500);


  // Serial.println("Play 8b !");
  // i2s_write(I2S_NUM_1, (uint8_t *)ok_44k1_16b, sizeof(ok_44k1_16b), &bytes_write, portMAX_DELAY);
  // delay(1500);
  
}
