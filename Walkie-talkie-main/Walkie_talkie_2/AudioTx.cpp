#include<AudioTx.h>

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

void playTone(float freq, int duration_ms);
void play_doremi();

void AudioTx::play_doremi() {  
  delay(500);

  Serial.println("Start Playing!");
  //do re mi 
  for (int i = 0; i < 8; i++) {
    AudioTx::playTone(notes[i], 200);
    delay(200);
  }
  for (int i = 8; i > 0; i--) {
    AudioTx::playTone(notes[i], 200);
    delay(200);
  }
  Serial.println("Done");
}
void AudioTx::playTone(float freq, int duration_ms) {
  const int sample_rate = SAMPLE_RATE;//44100;
  const int samples = (sample_rate * duration_ms) / 1000;
  int16_t* buffer = (int16_t*)malloc(sizeof(int16_t) * samples);

  for (int i = 0; i < samples; i++) {
    float t = (float)i / sample_rate;
    buffer[i] = (int16_t)(sin(2.0 * PI * freq * t) * 16000);  // 16-bit PCM
  }

  size_t bytes_written;
  AudioTx::write(buffer, samples);
  // i2s_write(I2S_NUM_0, buffer, samples * sizeof(int16_t), &bytes_written, portMAX_DELAY);
  free(buffer);
}

esp_err_t AudioTx::start()  {
  if(i2s_driver_install(I2S_NUM_1, &i2s_tx_config, 0, NULL) == ESP_OK && i2s_set_pin(I2S_NUM_1, &i2s_tx_pins) == ESP_OK)
  {
    Serial.println("Init TX success !");
    return ESP_OK;
  }
  else
  {
    Serial.println("Init TX fail !");
    return ESP_FAIL;
  }
};

esp_err_t AudioTx::stop() {
  if(i2s_driver_uninstall(I2S_NUM_1) == ESP_OK)
  {
      Serial.println("Init TX success !");
      return ESP_OK;
  }
  else
  {
    Serial.println("Init TX fail !");
    return ESP_FAIL;
  }
    // set up the I2S configuration from the subclass
  // configureI2S();
};

void AudioTx::write( int16_t *sample, int count) {
  size_t bytes_write = 0;
  i2s_write(I2S_NUM_1, sample, count * sizeof(int16_t), &bytes_write, portMAX_DELAY);
  // return bytes_write;
};
