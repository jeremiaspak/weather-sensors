#include <Arduino.h>
#include <RCSwitch.h>

void publish(String data);

// --------------------- RC Switch ---------------------

#define RF_TRANSMITTER_PIN 10
RCSwitch rf = RCSwitch();
void setupRF();

// ----------------------- Setup ------------------------

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  setupRF();
}

// ----------------------- Loop ------------------------

void loop() {
  publish("1234");
  delay(1000);
}

// --------------------- PubSub ------------------------

void publish(String data) {
  rf.send(data.toInt(), 32);
  Serial.println(data.toInt());
}

// -----------------------------------------------------
//                      RC Switch
// -----------------------------------------------------

void setupRF() {
  rf.enableTransmit(RF_TRANSMITTER_PIN);
  rf.setProtocol(1);
  rf.setPulseLength(350);
}
