include <Arduino.h>
#include "esphomelib.h"
#include "high_temp_sensor.h"

HighTemp ht(0, A0);

static const char *TAG = "firewatch";

using namespace esphomelib;


class MoistureSensor : public PollingComponent, public sensor::Sensor {
 public:

  MoistureSensor() : PollingComponent(10000) { }

  void setup() override {
    ht.begin();
  }

  void update() override {
    int stoveTemp = ht.getThermoCoupleTemperature();
    // int roomTemp = ht.getRoomTemperature();

    publish_state(stoveTemp);
  }
};
/*

Innovativetom.com
stevebrace.co.uk

Flower Pot Soil Mosture Sensor

A0 - Soil Mosture Sensor

D2:D6 - LEDS 1,2,3,4,5

LED1 - Green

LED2 - Green

LED3 - Green

LED4 - YELLOW

LED5 - RED

Connect the Soil Mosture Sensor to anolog input pin 0,

and your 5 led to digital out 2-6


int led1 = 2;

int led2 = 3;

int led3 = 4;

int led4 = 5;

int led5 = 6;

int probe = 7;

int mostureSensor = 0;

void setup() {

// Serial Begin so we can see the data from the mosture sensor in our serial input window.

Serial.begin(9600);

// setting the led pins to outputs

pinMode(led1, OUTPUT);

pinMode(led2, OUTPUT);

pinMode(led3, OUTPUT);

pinMode(led4, OUTPUT);

pinMode(led5, OUTPUT);

pinMode(probe, OUTPUT);

}

// the loop routine runs over and over again forever:

void loop() {

// Power-up the probe and pause for the driver

digitalWrite(probe, HIGH);

delay(1000);

// read the input on analog pin 0:

int sensorValue = analogRead(mostureSensor);

// print out the value you read:

Serial.println(sensorValue);

if (sensorValue >= 820)

{

digitalWrite(led1, HIGH);

digitalWrite(led2, HIGH);

digitalWrite(led3, HIGH);

digitalWrite(led4, HIGH);

digitalWrite(led5, HIGH);

}

else if (sensorValue >= 615 && sensorValue < 820)

{

digitalWrite(led1, HIGH);

digitalWrite(led2, HIGH);

digitalWrite(led3, HIGH);

digitalWrite(led4, HIGH);

digitalWrite(led5, LOW);

}

else if (sensorValue >= 410 && sensorValue < 615)

{

digitalWrite(led1, HIGH);

digitalWrite(led2, HIGH);

digitalWrite(led3, HIGH);

digitalWrite(led4, LOW);

digitalWrite(led5, LOW);

}

else if (sensorValue >= 250 && sensorValue < 410)

{

digitalWrite(led1, HIGH);

digitalWrite(led2, HIGH);

digitalWrite(led3, LOW);

digitalWrite(led4, LOW);

digitalWrite(led5, LOW);

}

else if (sensorValue >= 0 && sensorValue < 250)

{

digitalWrite(led1, HIGH);

digitalWrite(led2, LOW);

digitalWrite(led3, LOW);

digitalWrite(led4, LOW);

digitalWrite(led5, LOW);

}

// Power-down the probe

digitalWrite(probe, LOW);

delay(1800000); // wait half an hour

}
*/
