#include <Arduino.h>
#include "high_temp_sensor.h"

HighTemp ht(0, A0);

using namespace esphomelib;


class FirewatchSensor : public PollingComponent, public sensor::Sensor {
 public:

  FirewatchSensor() : PollingComponent(10000) { }

  void setup() override {
    ht.begin();
  }

  void update() override {
    int stoveTemp = ht.getThermoCoupleTemperature();
    // int roomTemp = ht.getRoomTemperature();

    publish_state(stoveTemp);
  }
};
