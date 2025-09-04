#pragma once

#include<I2SConfig.h>

// extern i2s_config_t i2s_tx_config;
// extern i2s_pin_config_t i2s_tx_pins;

class AudioRx : public I2SConfig {
  protected: 
      

  public:
      // I2S_RX(i2s_port_t port = I2S_NUM_0, const i2s_config_t cfg = i2s_rx_config) : I2SConfig(port, cfg) {}
      // esp_err_t init();
     
      esp_err_t start() override;
      esp_err_t stop() override;
      size_t read( int32_t *sample, int count);

};