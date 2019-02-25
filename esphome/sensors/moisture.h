#include <Arduino.h>
#include "esphome.h"
#include <driver/adc.h>


// static const char *TAG = "moisture";

int vRefPin = 25;
int sensorPin = 34;

using namespace esphomelib;

class MoistureSensor : public PollingComponent, public sensor::Sensor {
 public:

  MoistureSensor() : PollingComponent(1800000) { }

  void setup() override {
    pinMode(sensorPin, ANALOG);
    pinMode(vRefPin, OUTPUT);
    digitalWrite(vRefPin, LOW);
  }

  void update() override {
    digitalWrite(vRefPin, HIGH);
    delay(1000);
    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_DB_11);
    int sensorValue = adc1_get_raw(ADC1_CHANNEL_6);
     publish_state(sensorValue);
    // publish_state((sensorValue / 1023) * 100);
    delay(1000);
    digitalWrite(vRefPin, LOW);
  }
};
