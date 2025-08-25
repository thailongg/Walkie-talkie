#include<AudioRx.h>


esp_err_t AudioRx::start()  {
  if(i2s_driver_install(I2S_NUM_0, &i2s_rx_config, 0, NULL) == ESP_OK && i2s_set_pin(I2S_NUM_0, &i2s_rx_pins) == ESP_OK)
  {
    Serial.println("Init RX success !");
    return ESP_OK;
  }
  else
  {
    Serial.println("Init RX fail !");
    return ESP_FAIL;
  }
};

esp_err_t AudioRx::stop() {
  if(i2s_driver_uninstall(I2S_NUM_0) == ESP_OK)
  {
      Serial.println("Init RX success !");
      return ESP_OK;
  }
  else
  {
    Serial.println("Init RX fail !");
    return ESP_FAIL;
  }
    // set up the I2S configuration from the subclass
  // configureI2S();
};

size_t AudioRx::read( int32_t *sample, int count) {
  size_t bytes_read = 0;
  i2s_read(I2S_NUM_0, sample, count * sizeof(int32_t), &bytes_read, portMAX_DELAY);
  return bytes_read;
};
