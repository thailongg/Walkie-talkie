#include <i2s_mic_tetst.h>

#define I2S_READ_LEN    1024

extern int SAMPLE_RATE;
const float melody[] = { 262, 294, 330, 349, 392, 440, 494 };
const float notes[] = {
  261.63,  // Do (C4)
  293.66,  // Re (D4)
  329.63,  // Mi (E4)
  349.23,  // Fa (F4)
  392.00,  // Sol (G4)
  440.00,  // La (A4)
  493.88,  // Si (B4)
  523.25   // Do (C5)
};

//----------------------------------------------------------------------------------------------------

void playTone_tx(float freq, int duration_ms);
void play_doremi_tx();
void playTone(float freq, int duration_ms);
void play_doremi();
void messageTone();

//----------------------------------------------------------------------------------------------------
void play_doremi() {  
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
}
void playTone(float freq, int duration_ms = 500) {
  const int sample_rate = SAMPLE_RATE;//44100;
  const int samples = (sample_rate * duration_ms) / 1000;
  int16_t* buffer = (int16_t*)malloc(sizeof(int16_t) * samples);

  for (int i = 0; i < samples; i++) {
    float t = (float)i / sample_rate;
    buffer[i] = (int16_t)(sin(2.0 * PI * freq * t) * 16000);  // 16-bit PCM
  }

  size_t bytes_written;
  i2s_write(I2S_NUM_0, buffer, samples * sizeof(int16_t), &bytes_written, portMAX_DELAY);
  free(buffer);
}
void messageTone() {
  playTone(1000, 100);   // nốt cao ngắn
  delay(50);
  playTone(800, 100);    // nốt thấp hơn một chút
}
void play_doremi_tx() {  
  delay(500);

  Serial.println("Start Playing!");
  //do re mi 
  for (int i = 0; i < 8; i++) {
    playTone_tx(notes[i], 200);
    delay(200);
  }
  for (int i = 8; i > 0; i--) {
    playTone_tx(notes[i], 200);
    delay(200);
  }
  Serial.println("Done");
}
void playTone_tx(float freq, int duration_ms = 500) {
  const int sample_rate = SAMPLE_RATE;//44100;
  const int samples = (sample_rate * duration_ms) / 1000;
  int16_t* buffer = (int16_t*)malloc(sizeof(int16_t) * samples);

  for (int i = 0; i < samples; i++) {
    float t = (float)i / sample_rate;
    buffer[i] = (int16_t)(sin(2.0 * PI * freq * t) * 16000);  // 16-bit PCM
  }

  size_t bytes_written;
  // i2s_write(I2S_NUM_0, buffer, samples * sizeof(int16_t), &bytes_written, portMAX_DELAY);
  // sendLargeData(buffer, samples * sizeof(int16_t), peerAddress);
  sendLargeData_16bit(buffer, samples, peerAddress);
  free(buffer);
}
// void tx_rx_16bit() {
//   int32_t i2s_read_buff[I2S_READ_LEN];
//   int16_t i2s_out_buff[I2S_READ_LEN];
//   size_t bytes_read;
//   i2s_read(I2S_NUM_1, (void*)i2s_read_buff, I2S_READ_LEN * sizeof(int32_t), &bytes_read, portMAX_DELAY);
//   // Chuyển từ 32-bit xuống 16-bit để tiết kiệm băng thông
  
//   size_t index = bytes_read / sizeof(int32_t);
//   if(index == I2S_READ_LEN) {  
//     for (int i = 0; i < index; i++) {
//       i2s_out_buff[i] = (int16_t)(*((int32_t*)&i2s_read_buff[i]) >> 16); 
//       if(Serial_ON)
//         Serial.printf("%ld\n", i2s_out_buff[i]);
//     }
//     size_t bytes_write;
//     i2s_write(I2S_NUM_0, i2s_out_buff, index * sizeof(int16_t), &bytes_write, portMAX_DELAY);
//   }
// }
// void tx_rx_8bit() {
//   int32_t i2s_read_buff[I2S_READ_LEN];
//   int8_t i2s_out_buff[I2S_READ_LEN];
//   size_t bytes_read;
//   i2s_read(I2S_NUM_1, (void*)i2s_read_buff, I2S_READ_LEN * sizeof(int32_t), &bytes_read, portMAX_DELAY);
//   // Chuyển từ 32-bit xuống 16-bit để tiết kiệm băng thông
  
//   size_t index = bytes_read / sizeof(int32_t);
//   if(index == I2S_READ_LEN) {  
//     for (int i = 0; i < index; i++) {
//       i2s_out_buff[i] = (int8_t)(*((int32_t*)&i2s_read_buff[i]) >> 24); 
//       if(Serial_ON)
//         Serial.printf("%ld\n", i2s_out_buff[i]);
//     }
//     size_t bytes_write;
//     i2s_write(I2S_NUM_0, i2s_out_buff, index * sizeof(int8_t), &bytes_write, portMAX_DELAY);
//   }
// }