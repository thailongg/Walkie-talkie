#include <driver/i2s.h>
#include <espnow_data.h>

#define esp32_wroom
// #define esp32_s3
#if !defined(esp32_s3) && !defined(esp32_wroom)
  #error "Bạn phải define esp32_s3 hoặc esp32_wroom trước khi biên dịch!"
#endif

#define music_pin 15
#define tx_pin 2
#define rx_pin 4
#define SAMPLE_BUFFER_SIZE 1024
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_LEFT

#ifdef esp32_wroom
  #define I2S_MIC_SERIAL_CLOCK_RX GPIO_NUM_18
  #define I2S_MIC_LEFT_RIGHT_CLOCK_RX GPIO_NUM_19
  #define I2S_MIC_SERIAL_DATA_RX GPIO_NUM_5

  #define I2S_MIC_SERIAL_CLOCK_TX GPIO_NUM_25
  #define I2S_MIC_LEFT_RIGHT_CLOCK_TX GPIO_NUM_32
  #define I2S_MIC_SERIAL_DATA_TX GPIO_NUM_33
#endif

#ifdef esp32_s3
  #define I2S_MIC_SERIAL_CLOCK_RX GPIO_NUM_41
  #define I2S_MIC_LEFT_RIGHT_CLOCK_RX GPIO_NUM_42
  #define I2S_MIC_SERIAL_DATA_RX GPIO_NUM_40

  #define I2S_MIC_SERIAL_CLOCK_TX GPIO_NUM_18
  #define I2S_MIC_LEFT_RIGHT_CLOCK_TX GPIO_NUM_16 
  #define I2S_MIC_SERIAL_DATA_TX GPIO_NUM_17
#endif

int SAMPLE_RATE = 11025;
int16_t data_out[SAMPLE_BUFFER_SIZE];
int samples_read = 0;

i2s_config_t i2s_rx_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX ),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
};

i2s_pin_config_t i2s_rx_pins = {
    .bck_io_num = I2S_MIC_SERIAL_CLOCK_RX,
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK_RX,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_MIC_SERIAL_DATA_RX};
#ifdef esp32_wroom
const i2s_config_t i2s_tx_config_DAC = {
  .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
  .sample_rate = SAMPLE_RATE,
  .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
  .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
  .intr_alloc_flags = 0, // default interrupt priority
  .dma_buf_count = 8,
  .dma_buf_len = 64,
  .use_apll = false,    
  .tx_desc_auto_clear = false,          
};
#endif
// Speaker
i2s_config_t i2s_tx_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = I2S_COMM_FORMAT_I2S_MSB,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
    };

// and don't mess around with this
i2s_pin_config_t i2s_tx_pins = {
    .bck_io_num = I2S_MIC_SERIAL_CLOCK_TX,
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK_TX,
    .data_out_num = I2S_MIC_SERIAL_DATA_TX,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

esp_err_t init_RX() {
  if(i2s_driver_install(I2S_NUM_1, &i2s_rx_config, 0, NULL) == ESP_OK)
  {
    if(i2s_set_pin(I2S_NUM_1, &i2s_rx_pins) == ESP_OK)
    {
      Serial.println("Init RX success !");
      return ESP_OK;
    }
    else
      Serial.println("Init RX fail !");
  }
  else
  {
    Serial.println("Init RX fail !");
    return ESP_FAIL;
  }
}
esp_err_t deinit_RX() {
  if(i2s_driver_uninstall(I2S_NUM_1) == ESP_OK)
  {
    Serial.println("Deinit RX success !");
    return ESP_OK;
  }
  else
  {
    Serial.println("Deinit RX fail !");
    return ESP_FAIL;
  }
}
esp_err_t deinit_TX() {
  if(i2s_driver_uninstall(I2S_NUM_0) == ESP_OK)
  {
    Serial.printf("Deinit TX success !");
    return ESP_OK;
  }
  else
  {
    Serial.println("Deinit TX fail !");
    return ESP_FAIL;
  }
}
esp_err_t deinit_TX_2() {
  if(i2s_driver_uninstall(I2S_NUM_0) == ESP_OK)
  {
    Serial.println("Deinit TX success !");
    return ESP_OK;
  }
  else
  {
    Serial.printf("Deinit TX fail !");
    return ESP_FAIL;
  }
}
esp_err_t init_TX() {
  if(i2s_driver_install(I2S_NUM_0, &i2s_tx_config, 0, NULL) == ESP_OK)
  {
    if(i2s_set_pin(I2S_NUM_0, &i2s_tx_pins) == ESP_OK)
    {
      Serial.println("Init TX success !");
      // int16_t silence[SAMPLE_RATE / 10] = {0};
      // size_t bytes_written;
      // i2s_write(I2S_NUM_0, silence, sizeof(silence), &bytes_written, portMAX_DELAY);
      return ESP_OK;
    }
    else
      Serial.println("Init TX fail !");
  }
  else
  {
    Serial.println("Init TX fail !");
    return ESP_FAIL;
  }
}
#ifdef esp32_wroom
esp_err_t init_TX_DAC() {
  if(i2s_driver_install(I2S_NUM_0, &i2s_tx_config_DAC, 0, NULL) == ESP_OK)
  {
    if(i2s_set_pin(I2S_NUM_0, NULL) == ESP_OK)
    {
      Serial.println("Init TX DAC success !");
      return ESP_OK;
    }
    else
      Serial.println("Init TX DAC fail !");
  }
  else
  {
    Serial.println("Init TX DAC fail !");
    return ESP_FAIL;
  }
}
#endif

void check_ram() {
  Serial.print("Free heap RAM: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");
  // delay(1000);
}
void play_tx_record(int16_t *data_out, int data_size) //hàm này phát record thu được từ rx
{
    deinit_RX();
    init_TX();
    size_t bytes_written = 0;
    esp_err_t result = i2s_write(I2S_NUM_0, data_out, data_size * sizeof(int16_t), &bytes_written, portMAX_DELAY);
    deinit_TX();
    init_RX();
}
int read() {
  int32_t data_in[SAMPLE_BUFFER_SIZE];
  // read from the I2S device
  size_t bytes_read = 0;
  esp_err_t result = i2s_read(I2S_NUM_1, data_in, SAMPLE_BUFFER_SIZE * sizeof(int32_t), &bytes_read, portMAX_DELAY);
  if( result == ESP_OK) {
    int samples_read = bytes_read /  sizeof(int32_t);
    // for (int i = 0; i < samples_read; i++)
    // {
    //   Serial.printf("%ld\n", data_in[i]);
    // };
    for (size_t i = 0; i < samples_read; ++i ){
      int16_t temp = data_in[i] >> 16;
      data_out[i] = constrain(temp, -32768, 32767);
      // data_out[i] = data_in[i] >> 16;  // dịch phải 16 bit, lấy MSB
    }
    Serial.printf("Read end - bytes_read: %d\n", samples_read);
    // play_tx_record(data_out, samples_read);
    return samples_read;
  }
  else {
    return 0;
  }
}

void write(int16_t *data_out, int data_size)
{
    size_t bytes_written = 0;
    esp_err_t result = i2s_write(I2S_NUM_0, data_out, data_size * sizeof(int16_t), &bytes_written, portMAX_DELAY);
    if((bytes_written == data_size * sizeof(int16_t)) && result == ESP_OK ) {           
      for(int i = 0; i< data_size; ++i) {
        Serial.println(data_out[i]);
      }
      Serial.printf("Transmitted success - bytes_written: %ld\n", bytes_written/sizeof(int16_t)); 
    }
    else
      Serial.printf("Transmitted fail\n");
}

// void sendLargeData(const uint8_t *data, size_t len, const uint8_t *peer_addr) {
//   size_t offset = 0;
//   while (offset < len) {
//     if(!send_ON)
//       break;
//     size_t chunk = (len - offset > ESPNOW_MAX_SIZE) ? ESPNOW_MAX_SIZE : (len - offset);
//     esp_now_send(peer_addr, data + + offset, chunk);
//     offset += chunk;
//     // delay(10);
//   }
// } 
void sendLargeData_16bit(int16_t *data, size_t len, const uint8_t *peer_addr) {
  size_t send_index = len * 2;
  uint8_t send_buff[send_index];
  // int send_index = 0;
  for (int i = 0; i < send_index; i++) {
    // int16_t sample16 = (int16_t)(*((int32_t*)&i2s_read_buff[i]) >> 16);
    send_buff[i*2] = data[i] & 0xFF;
    send_buff[i*2 + 1] = (data[i] >> 8) & 0xFF;      
  }
  size_t offset = 0;
  while (offset < send_index) {
    size_t chunk = (send_index - offset > ESPNOW_MAX_SIZE) ? ESPNOW_MAX_SIZE : (send_index - offset);
    esp_now_send(peer_addr, send_buff + + offset, chunk);
    offset += chunk;
    // delay(10);
  }
} 