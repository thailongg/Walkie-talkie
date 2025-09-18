#include <driver/i2s.h>
#include <math.h>
#include "evil_laugh.h"

// I2S pins
#define I2S_DOUT 5
#define I2S_BCLK 18
#define I2S_LRC  19
#define SAMPLE_RATE 44100
// I2S config
void setupI2S() {
  const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, /* the DAC module will only take the 8bits from MSB */
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .intr_alloc_flags = 0, // default interrupt priority
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
    .use_apll = false
  };

  // const i2s_pin_config_t pin_config = {
  //   .bck_io_num = I2S_BCLK,
  //   .ws_io_num = I2S_LRC,
  //   .data_out_num = I2S_DOUT,
  //   .data_in_num = I2S_PIN_NO_CHANGE
  // };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, NULL);
  // i2s_zero_dma_buffer(I2S_NUM_0);
}

// Tạo sóng sin và phát 1 nốt nhạc
void playTone(float freq, int duration_ms = 500) {
  const int sample_rate = SAMPLE_RATE;
  const int samples = (sample_rate * duration_ms) / 1000;
  int16_t* buffer = (int16_t*)malloc(sizeof(int16_t) * samples);

  for (int i = 0; i < samples; i++) {
    float t = (float)i / sample_rate;
    buffer[i] = (int16_t)(sin(2.0 * PI * freq * t) * 16000);  // 16-bit PCM
  }

  size_t bytes_written;
  i2s_write(I2S_NUM_0, buffer, samples * sizeof(int16_t), &bytes_written, portMAX_DELAY);
  // for (int i = 0; i < samples; i++) {
  //   // float t = (float)i / sample_rate;
  //   // buffer[i] = (int16_t)(sin(2.0 * PI * freq * t) * 16000);  // 16-bit PCM
  //   Serial.println(buffer[i]);
  // }
  
  free(buffer);
}

// Mảng tần số các nốt nhạc: Do Re Mi Fa Sol La Si Do
float notes[] = {
  261.63,  // Do (C4)
  293.66,  // Re (D4)
  329.63,  // Mi (E4)
  349.23,  // Fa (F4)
  392.00,  // Sol (G4)
  440.00,  // La (A4)
  493.88,  // Si (B4)
  523.25   // Do (C5)
};

void play_doremi() {  
  i2s_start(I2S_NUM_0);
  delay(500);

  Serial.println("Start Playing!");
  //do re mi 
  for (int i = 0; i < 8; i++) {
    playTone(notes[i], 200);
    delay(200);
  }
  for (int i = 8; i > 0; i--) {
    playTone(notes[i], 200);
    delay(200);
  }
  Serial.println("Done");
  i2s_stop(I2S_NUM_0);
}
void setup() {
  Serial.begin(115200);
  setupI2S();
  // delay(500);

  // Serial.println("Playing Do Re Mi Fa Sol La Si Do");

  // for (int i = 0; i < 8; i++) {
  //   playTone(notes[i], 400);
  //   delay(100);
  // }

  // Serial.println("Done");
}

void loop() {
  // nothing
  // setupI2S();
  // delay(1000);

  // Serial.println("Playing Do Re Mi Fa Sol La Si Do");

  // for (int i = 0; i < 8; i++) {
  //   playTone(notes[i], 200);
  //   delay(200);
  // }     
  // // for (int i = 0; i < 8; i++) {
  // //   Serial.println(notes[i]);
  // //   // playTone(notes[i], 200);
  // //   // delay(100);
  // // }  
  // Serial.println("Done");
  // delay(1000);
  // i2s_driver_uninstall(I2S_NUM_0);

  size_t bytes_written;
  i2s_start(I2S_NUM_0);
  i2s_write(I2S_NUM_0, evil_laugh_sample, sizeof(evil_laugh_sample), &bytes_written, portMAX_DELAY);
  i2s_stop(I2S_NUM_0);
  delay(2000);

  play_doremi();
  delay(2000);
  
}
