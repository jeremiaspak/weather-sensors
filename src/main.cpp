#include <Arduino.h>
#include <RCSwitch.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

uint32_t delayBetweenMeasurements;
const String TEMPERATURE_ID = "99";
const String PRESSURE_ID = "98";
const String ALTITUDE_ID = "97";
String transformData(float data);
void publish(String data);

// --------------------- RC Switch ---------------------

#define RF_TRANSMITTER_PIN 10
RCSwitch rf = RCSwitch();
void setupRF();

// --------------------- BMP Sensor ---------------------

Adafruit_BMP085 bmp;
void setupBMPSensor();
void logBMPTemperature();
void logBMPPressure();
void logBMPAltitude();

// ----------------------- Setup ------------------------

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  setupRF();
  setupBMPSensor();

  delayBetweenMeasurements = 10000;
}

// ----------------------- Loop ------------------------

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);

  logBMPTemperature();
  logBMPPressure();
  logBMPAltitude();

  digitalWrite(LED_BUILTIN, LOW);
  delay(delayBetweenMeasurements);
}

// --------------------- PubSub ------------------------

void publish(String data) {
  rf.send(data.toInt(), 32);
  Serial.println(data.toInt());
}

String transformData(String id, float data) {
  unsigned long intValue = (data * 100);
  return id + String(intValue);
}

// -----------------------------------------------------
//                      RC Switch
// -----------------------------------------------------

void setupRF() {
  rf.enableTransmit(RF_TRANSMITTER_PIN);
  rf.setProtocol(1);
  rf.setPulseLength(350);
}

// -----------------------------------------------------
//                      BMP Sensor
// -----------------------------------------------------

void setupBMPSensor() {
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) { }
  }
}

void logBMPTemperature() {
  float temperature = bmp.readTemperature();
  Serial.println("Temperature = " + String(temperature) + " °C");
  publish(transformData(TEMPERATURE_ID, temperature));
}

void logBMPPressure() {
  float preassure = bmp.readPressure();
  Serial.println("Pressure = " + String(preassure) + " Pa");
  publish(transformData(PRESSURE_ID, preassure));

  // preassure = bmp.readSealevelPressure();
  // Serial.println("Pressure at sealevel (calculated) = " + String(preassure) + " Pa");
}

void logBMPAltitude() {
  // Calculate altitude assuming 'standard' barometric
  // pressure of 1013.25 millibar = 101325 Pascal
  float altitude = bmp.readAltitude();
  Serial.println("Altitude = " + String(altitude) + " meters");
  publish(transformData(ALTITUDE_ID, altitude));

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
  // altitude = bmp.readAltitude(101500);
  // Serial.println("Real altitude = " + String(altitude) + " meters");
}
