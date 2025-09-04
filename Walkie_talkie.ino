#include<I2SConfig.h>
#include<AudioRx.h>
#include<AudioTx.h>
#include<ESPNOWTransport.h>

#define buf_size    1024

int32_t buf_read[buf_size];

AudioTx PCM5102A;
AudioRx INMP441;
Espnow  Transport;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  PCM5102A.start();
  INMP441.start();
  Transport.start();
}

void loop() {
  // put your main code here, to run repeatedly:
  size_t bytes_read;
  bytes_read = INMP441.read(buf_read, buf_size);
  if (bytes_read == buf_size) {
    int16_t buf_write[buf_size];
    // int send_index = 0;
    for (int i = 0; i < buf_size; i++) {      
      int32_t temp = (int32_t)(*((int32_t*)&buf_read[i]) >> 15);
      // buf_write[i] = (int16_t)temp; // => buffer of write         
      buf_write[i] = (temp > INT16_MAX) ? INT16_MAX : (temp < -INT16_MAX) ? -INT16_MAX : (int16_t)temp;
    }  
    PCM5102A.write(buf_write, buf_size);
  }
}
  // PCM5102A.play_doremi();
  // delay(1500);
