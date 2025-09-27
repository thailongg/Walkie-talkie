#include<DAC_AudioTx.h>

esp_err_t DACAudioTx::start()  {
  if(i2s_driver_install(I2S_NUM_0, &DAC_i2s_tx_config, 0, NULL) == ESP_OK && i2s_set_pin(I2S_NUM_0, NULL) == ESP_OK && i2s_set_dac_mode(I2S_DAC_CHANNEL_RIGHT_EN) == ESP_OK && i2s_zero_dma_buffer(I2S_NUM_0) == ESP_OK)
  {
    Serial.println("Init DAC TX success !");
    return ESP_OK;
  }
  else
  {
    Serial.println("Init DAC TX fail !");
    return ESP_FAIL;
  }
};
size_t DACAudioTx::write( uint8_t *sample, int count) {
  size_t bytes_write = 0;
  i2s_write(I2S_NUM_0, sample, count * sizeof(uint8_t), &bytes_write, portMAX_DELAY);
  return bytes_write;
};