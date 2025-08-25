#include<I2SConfig.h>
#include<AudioRx.h>
#include<AudioTx.h>

AudioTx PCM5102A;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  PCM5102A.start();
}

void loop() {
  // put your main code here, to run repeatedly:
  PCM5102A.play_doremi();
  delay(1500);
}
