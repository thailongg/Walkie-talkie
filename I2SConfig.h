#pragma once

// #include <freertos/FreeRTOS.h>
#include <driver/i2s.h>
#include <Arduino.h>
// #include "driver/gpio.h"
/**
 * Base Class for both the ADC and I2S sampler
 **/
extern const i2s_config_t i2s_rx_config;
extern const i2s_pin_config_t i2s_rx_pins;
extern const i2s_config_t i2s_tx_config;
extern const i2s_pin_config_t i2s_tx_pins;
extern const i2s_config_t DAC_i2s_tx_config;

#ifdef CONFIG_IDF_TARGET_ESP32
  #define I2S_MIC_SERIAL_CLOCK_RX GPIO_NUM_18
  #define I2S_MIC_LEFT_RIGHT_CLOCK_RX GPIO_NUM_19
  #define I2S_MIC_SERIAL_DATA_RX GPIO_NUM_5

  #define I2S_MIC_SERIAL_CLOCK_TX GPIO_NUM_25
  #define I2S_MIC_LEFT_RIGHT_CLOCK_TX GPIO_NUM_32
  #define I2S_MIC_SERIAL_DATA_TX GPIO_NUM_33
#endif

#ifdef CONFIG_IDF_TARGET_ESP32S3
  #define I2S_MIC_SERIAL_CLOCK_RX GPIO_NUM_18
  #define I2S_MIC_LEFT_RIGHT_CLOCK_RX GPIO_NUM_16
  #define I2S_MIC_SERIAL_DATA_RX GPIO_NUM_17

  #define I2S_MIC_SERIAL_CLOCK_TX GPIO_NUM_42
  #define I2S_MIC_LEFT_RIGHT_CLOCK_TX GPIO_NUM_40 
  #define I2S_MIC_SERIAL_DATA_TX GPIO_NUM_41
#endif


 


class I2SConfig {
protected:
    // i2s_port_t m_i2sPort;
    // i2s_config_t m_i2s_config;
    // i2s_pin_config_t m_i2s_pin_config;
    // virtual void configureI2S() = 0;
    // virtual void unConfigureI2S(){};
    // virtual void processI2SData(void *samples, size_t count){
        // nothing to do for the default case
    // };

public:
    // I2SSampler(i2s_port_t i2sPort, const i2s_config_t &i2sConfig);
    // I2SConfig(i2s_port_t i2sPort = I2S_NUM_0, const i2s_config_t i2sConfig = i2s_tx_config, )
    //     : m_i2sPort(i2sPort), m_i2s_config(i2sConfig) {}
    virtual esp_err_t start() = 0;
    // virtual int read(int16_t *samples, int count) = 0;
    virtual esp_err_t stop() = 0;
    // int sample_rate()
    // {
    //     return m_i2s_config.sample_rate;
    // }
};
