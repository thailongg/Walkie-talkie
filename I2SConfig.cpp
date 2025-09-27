#include<I2SConfig.h>

// #define  SAMPLE_RATE        44100
int SAMPLE_RATE = 8000;

const i2s_config_t i2s_rx_config = {
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

const i2s_pin_config_t i2s_rx_pins = {
    .bck_io_num = I2S_MIC_SERIAL_CLOCK_RX,
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK_RX,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_MIC_SERIAL_DATA_RX};
// Speaker
const i2s_config_t i2s_tx_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = I2S_COMM_FORMAT_I2S_MSB,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    // .dma_buf_count = 4,
    // .dma_buf_len = 1024,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
    };

const i2s_config_t DAC_i2s_tx_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_8BIT, /* the DAC module will only take the 8bits from MSB */
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = I2S_COMM_FORMAT_I2S_MSB,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = true
};
// and don't mess around with this
const i2s_pin_config_t i2s_tx_pins = {
    .bck_io_num = I2S_MIC_SERIAL_CLOCK_TX,
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK_TX,
    .data_out_num = I2S_MIC_SERIAL_DATA_TX,
    .data_in_num = I2S_PIN_NO_CHANGE
  };
// void I2SConfig::start() override {
//   i2s_driver_install(I2S_NUM_0, &i2s_tx_config, 0, NULL);
//   i2s_set_pin(I2S_NUM_0, &i2s_tx_pins);
//     // set up the I2S configuration from the subclass
//   // configureI2S();
// };

// void I2SConfig::stop() override {
//   i2s_driver_uninstall(I2S_NUM_0, &i2s_tx_config, 0, NULL);
//     // set up the I2S configuration from the subclass
//   // configureI2S();
// };
// I2S::I2S(i2s_port_t i2sPort, const i2s_config_t &i2sConfig) {
//   m_i2sPort
// }