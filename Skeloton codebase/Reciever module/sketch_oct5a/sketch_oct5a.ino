//include the libraires
#include <SPI.h>;
#include <nRF24L01.h>;
#include <RF24.h>;
#include <Servo.h>;

//PINS
int ch_width_1 = 0;
int ch_width_2 = 0;
int ch_width_3 = 0;
int ch_width_4 = 0;
Servo ch1;
Servo ch2;
Servo ch3;
Servo ch4;

//signals
struct Signal {
  byte throttle;
  byte pitch;
  byte roll;
  byte yaw;
};

Signal Data;
const uint64_t pipeIn = 0xE9E8F0F0E1LL;
RF24 radio(7,8);

void ResetData() {
  // Define the inicial value of each data input.
  // The middle position for Potenciometers. (254/2=127)
  Data.throttle = 127; // Motor Stop | Motor Kapalı
  Data.pitch = 127;  // Center
  Data.roll = 127;   // Center
  Data.yaw = 127;   // Center 

}

void setup() {
  //Set the pins for each PWM signal.
  ch1.attach(2);
  ch2.attach(3);
  ch3.attach(4);
  ch4.attach(5);
  //Configure the NRF24 module
  ResetData();
  radio.begin();
  radio.openReadingPipe(1,pipeIn);

  radio.startListening(); //start the radio comunication for receiver

}
unsigned long lastRecvTime = 0;

void recvData() {
  while (radio.available()) {
    radio.read(&Data, sizeof(Signal));
    lastRecvTime = millis();
  }
}

void loop() {
  recvData();
  unsigned long now = millis();
  if ( now - lastRecvTime > 1000 ) {
    ResetData(); // Signal lost.. Reset data.
  }

  ch_width_1 = map(Data.throttle, 0, 255, 1000, 2000);     // pin D2 (PWM signal)
  ch_width_2 = map(Data.pitch,    0, 255, 1000, 2000);     // pin D3 (PWM signal)
  ch_width_3 = map(Data.roll,     0, 255, 1000, 2000);     // pin D4 (PWM signal)
  ch_width_4 = map(Data.yaw,      0, 255, 1000, 2000);     // pin D5 (PWM signal)

  // Write the PWM signal | PWM sinyaller çıkışlara gönderiliyor
  ch1.writeMicroseconds(ch_width_1);
  ch2.writeMicroseconds(ch_width_2);
  ch3.writeMicroseconds(ch_width_3);
  ch4.writeMicroseconds(ch_width_4);

}

