#pragma once

#include<I2SConfig.h>

// extern i2s_config_t i2s_tx_config;
// extern i2s_pin_config_t i2s_tx_pins;

extern int SAMPLE_RATE;

class AudioTx : public I2SConfig {
  protected: 
      void playTone(float freq, int duration_ms = 500);
  public:
      // I2S_RX(i2s_port_t port = I2S_NUM_0, const i2s_config_t cfg = i2s_rx_config) : I2SConfig(port, cfg) {}
      // esp_err_t init();
      void play_doremi();
      esp_err_t start() override;
      esp_err_t stop() override;
      static size_t write( int16_t *sample, int count);

};