#include <Arduino.h>

#define logging 1
#define PxMATRIX_COLOR_DEPTH 4
#define TIME_OFFSET -5UL * 3600UL 

#ifdef ARDUINO_ARCH_ESP32

#define P_LAT 22
#define P_A 19
#define P_B 23
#define P_C 18
#define P_D 5
#define P_E 15
#define P_OE 2
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#include <WiFi.h>

#endif

#ifdef ARDUINO_ARCH_ESP8266

#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_D 12
#define P_E 0
#define P_OE 10

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#endif


#define logging 1
#define PxMATRIX_COLOR_DEPTH 4

#include <Arduino.h>
#include <PxMatrix.h>
#include <Ticker.h>
// #include "esphomelib/component.h"
#include "esphomelib.h"
#include <JSON_Decoder.h>
#include <DarkSkyWeather.h>
#include <Fonts/TomThumb.h>

using namespace esphomelib;

const char* tag = "weatherstation";
int brightness=0;
int dimm=0;

// =====================================================
// ========= User configured stuff starts here =========
// Further configuration settings can be found in the
// DarkSkyWeather library "User_Setup.h" file

#define TIME_OFFSET -5UL * 3600UL // UTC + 0 hour

// Change to suit your WiFi router
#define SSID "39"
#define SSID_PASSWORD "thirtyninecheminriver"

// Dark Sky API Details, replace x's with your API key
String api_key = "be8a6d8b3475725b8db20ae90c78376c"; // Obtain this from your Dark Sky account

// Set both longitude and latitude to at least 4 decimal places
String latitude =  "45.548135"; // 90.0000 to -90.0000 negative for Southern hemisphere
String longitude = "-75.789721"; // 180.000 to -180.000 negative for West

String units = "si";  // See notes tab
String language = ""; // See notes tab

DS_Weather dsw;

PxMATRIX screenmatrix(32, 32, P_LAT, P_OE, P_A, P_B, P_C, P_D);

// time::HomeAssistantTime *homeassistant_time = App.make_homeassistant_time_component();

DSW_current *current = new DSW_current;
DSW_hourly *hourly = new DSW_hourly;
DSW_daily  *daily = new DSW_daily;

// time_t time;

uint16_t myRED = screenmatrix.color565(255, 0, 0);
uint16_t myGREEN = screenmatrix.color565(0, 255, 0);
uint16_t myBLUE = screenmatrix.color565(0, 0, 255);
uint16_t myWHITE = screenmatrix.color565(255, 255, 255);
uint16_t myYELLOW = screenmatrix.color565(255, 255, 0);
uint16_t myCYAN = screenmatrix.color565(0, 255, 255);
uint16_t myMAGENTA = screenmatrix.color565(255, 0, 255);
uint16_t myBLACK = screenmatrix.color565(0, 0, 0);

uint16 myCOLORS[8]={myRED,myGREEN,myBLUE,myWHITE,myYELLOW,myCYAN,myMAGENTA,myBLACK};

/*
 * Draw time
 */
void draw_time () {
  /*
	// auto time = homeassistant_time->now();
  
	char dateString [10];
	char timeString [10];

  // Setup display
  screenmatrix.setFont(&TomThumb);
  screenmatrix.setTextColor(myWHITE);

  // Draw time
	time.strftime(timeString, 10, "%R");
  screenmatrix.setCursor(1,31);
	screenmatrix.print(timeString);

  // Draw date
	time.strftime(dateString, 10, "%d");
  screenmatrix.setCursor(27,31);
	screenmatrix.print(dateString);
  */

}

// Ticker draw_time_ticker(draw_time, 100);

/*
 * Draw weather
 */
void draw_weather () {
  screenmatrix.setFont(&TomThumb);
  screenmatrix.setTextColor(myWHITE);

	// time.strftime(timeString, 10, "%R");
  screenmatrix.setCursor(1,1);
	screenmatrix.print(String(current->temperature));
  ESP_LOGD(tag, "%f", current->temperature);

  // screenmatrix.drawRect(0,0,32,32, myRED);
}

// Ticker draw_weather_ticker(draw_weather, 2000);

/*
 * Display updater
 */
void display_updater() {
  // screenmatrix.clearDisplay();

  draw_time();
  draw_weather();
  // draw_test_pixel();

  screenmatrix.display();
}

Ticker display_ticker(display_updater, 0.002);

/***************************************************************************************
**                          Convert unix time to a time string
***************************************************************************************/
String strTime(time_t unixTime)
{
  unixTime += TIME_OFFSET;
  return ctime(&unixTime);
}

/***************************************************************************************
**                          Get the icon file name from the index number
***************************************************************************************/
const char* getMeteoconIcon(uint8_t index)
{
  if (index > MAX_ICON_INDEX) index = 0;
  return dsw.iconName(index);
}

/***************************************************************************************
**                          Send weather info to serial port
***************************************************************************************/
void printCurrentWeather() {


  ESP_LOGD(tag, "%s", "\nRequesting weather information from DarkSky.net... ");


  ESP_LOGD(tag, "%s", "Weather from Dark Sky\n");

  // We can use the timezone to set the offset eventually...
  // ESP_LOGD(tag, "%s", "Timezone            : "); ESP_LOGD(tag, "%s", current->timezone);
  
  ESP_LOGD(tag, "%s", "############### Current weather ###############\n");
  ESP_LOGD(tag, "%s %s", "Current time             : " , strTime(current->time).c_str());
  ESP_LOGD(tag, "%s %s", "Current summary          : ", current->summary.c_str()); // ESP_LOGD(tag, "%s", current->summary.c_str());
  ESP_LOGD(tag, "%s %f", "Current temperature      : ", current->temperature);
  ESP_LOGD(tag, "%s %f", "Current precipIntensity  : ", current->precipIntensity);
  ESP_LOGD(tag, "%s %f", "Current humidity         : ", current->humidity);
  ESP_LOGD(tag, "%s %f", "Current precipProbability: ", current->precipProbability);
  ESP_LOGD(tag, "%s %f", "Current pressure         : ", current->pressure);
  ESP_LOGD(tag, "%s %f", "Current wind speed       : ", current->windSpeed);
  ESP_LOGD(tag, "%s %f", "Current wind gust        : ", current->windGust);
  ESP_LOGD(tag, "%s %f", "Current wind dirn        : ", current->windBearing);
  /*
  ESP_LOGD(tag, "%s", "Current precipType       : "); ESP_LOGD(tag, "%s", getMeteoconIcon(current->precipType));
  ESP_LOGD(tag, "%s", "Current icon             : "); ESP_LOGD(tag, "%s", getMeteoconIcon(current->icon));

  ESP_LOGD(tag, "%s", );

  ESP_LOGD(tag, "%s", "############### Hourly weather  ###############\n");
  ESP_LOGD(tag, "%s", "Overall hourly summary : "); ESP_LOGD(tag, "%s", hourly->overallSummary);
  for (int i = 0; i<MAX_HOURS; i++)
  {
    ESP_LOGD(tag, "%s", "Hourly summary  "); if (i<10) ESP_LOGD(tag, "%s", " ");
    ESP_LOGD(tag, "%s", i); ESP_LOGD(tag, "%s", " : "); ESP_LOGD(tag, "%s", hourly->summary[i]);
    ESP_LOGD(tag, "%s", "Time               : "); ESP_LOGD(tag, "%s", strTime(hourly->time[i]));
    ESP_LOGD(tag, "%s", "precipIntensity    : "); ESP_LOGD(tag, "%s", hourly->precipIntensity[i]);
    ESP_LOGD(tag, "%s", "precipProbability  : "); ESP_LOGD(tag, "%s", hourly->precipProbability[i]);
    ESP_LOGD(tag, "%s", "precipType         : "); ESP_LOGD(tag, "%s", hourly->precipType[i]);
    ESP_LOGD(tag, "%s", "precipAccumulation : "); ESP_LOGD(tag, "%s", hourly->precipAccumulation[i]);
    ESP_LOGD(tag, "%s", "temperature        : "); ESP_LOGD(tag, "%s", hourly->temperature[i]);
    ESP_LOGD(tag, "%s", "pressure           : "); ESP_LOGD(tag, "%s", hourly->pressure[i]);
    ESP_LOGD(tag, "%s", "cloudCover         : "); ESP_LOGD(tag, "%s", hourly->cloudCover[i]);
    ESP_LOGD(tag, "%s", );
  }

  ESP_LOGD(tag, "%s", "###############  Daily weather  ###############\n");
  ESP_LOGD(tag, "%s", "Daily summary     : "); ESP_LOGD(tag, "%s", daily->overallSummary);
  ESP_LOGD(tag, "%s", );

  for (int i = 0; i<MAX_DAYS; i++)
  {
    ESP_LOGD(tag, "%s", "Daily summary   ");
    ESP_LOGD(tag, "%s", i); ESP_LOGD(tag, "%s", " : "); ESP_LOGD(tag, "%s", daily->summary[i]);
    ESP_LOGD(tag, "%s", "time              : "); ESP_LOGD(tag, "%s", strTime(daily->time[i]));
    ESP_LOGD(tag, "%s", "Icon              : "); ESP_LOGD(tag, "%s", getMeteoconIcon(daily->icon[i]));
    ESP_LOGD(tag, "%s", "sunriseTime       : "); ESP_LOGD(tag, "%s", strTime(daily->sunriseTime[i]));
    ESP_LOGD(tag, "%s", "sunsetTime        : "); ESP_LOGD(tag, "%s", strTime(daily->sunsetTime[i]));
    ESP_LOGD(tag, "%s", "Moon phase        : "); ESP_LOGD(tag, "%s", daily->moonPhase[i]);
    ESP_LOGD(tag, "%s", "precipIntensity   : "); ESP_LOGD(tag, "%s", daily->precipIntensity[i]);
    ESP_LOGD(tag, "%s", "precipProbability : "); ESP_LOGD(tag, "%s", daily->precipProbability[i]);
    ESP_LOGD(tag, "%s", "precipType        : "); ESP_LOGD(tag, "%s", daily->precipType[i]);
    ESP_LOGD(tag, "%s", "precipAccumulation: "); ESP_LOGD(tag, "%s", daily->precipAccumulation[i]);
    ESP_LOGD(tag, "%s", "temperatureHigh   : "); ESP_LOGD(tag, "%s", daily->temperatureHigh[i]);
    ESP_LOGD(tag, "%s", "temperatureLow    : "); ESP_LOGD(tag, "%s", daily->temperatureLow[i]);
    ESP_LOGD(tag, "%s", "humidity          : "); ESP_LOGD(tag, "%s", daily->humidity[i]);
    ESP_LOGD(tag, "%s", "pressure          : "); ESP_LOGD(tag, "%s", daily->pressure[i]);
    ESP_LOGD(tag, "%s", "windSpeed         : "); ESP_LOGD(tag, "%s", daily->windSpeed[i]);
    ESP_LOGD(tag, "%s", "windGust          : "); ESP_LOGD(tag, "%s", daily->windGust[i]);
    ESP_LOGD(tag, "%s", "windBearing       : "); ESP_LOGD(tag, "%s", daily->windBearing[i]);
    ESP_LOGD(tag, "%s", "cloudCover        : "); ESP_LOGD(tag, "%s", daily->cloudCover[i]);
    ESP_LOGD(tag, "%s", );
  }

	*/

  // Delete to free up space and prevent fragmentation as strings change in length
}


/*
 * WeatherStation 
 */
class WeatherStation : public Component {
 public:


	void setup() override {
		Serial.printf("Connecting to %s\n", SSID);

		WiFi.begin(SSID, SSID_PASSWORD);
		 
		while (WiFi.status() != WL_CONNECTED) {
				delay(500);
				ESP_LOGD(tag, "%s", ".");
		}

		ESP_LOGD(tag, "%s", "Connected\n");

    ESP_LOGD(tag, "%s", "setup");

		dsw.getForecast(current, hourly, daily, api_key, latitude, longitude, units, language);

    // screenmatrix.setMuxPattern(BINARY);
    // screenmatrix.setScanPattern(LINE);
    screenmatrix.begin(16);
    screenmatrix.flushDisplay();

    display_ticker.start();
  }

  void loop() override {
	  display_ticker.update();
  }
};
