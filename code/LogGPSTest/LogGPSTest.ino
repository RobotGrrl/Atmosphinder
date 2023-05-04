#include <Adafruit_GPS.h>
#include <Streaming.h>
#include "AtmosphinderLogger.h"

#define CONSOLE_DEBUG true
#define GPSSerial Serial4
#define GPSECHO true

Adafruit_GPS GPS(&GPSSerial);
AtmosphinderLogger logger = AtmosphinderLogger();

// GPS related
uint32_t timer = millis();
long last_print = 0;

void setup() {
  Serial.begin(9600);

  initTime(); // should always be first

  logger.begin();
  logger.setLoggingLed(13);
  logger.initLogging();

  gpsBegin();

}

void loop() {

  gpsUpdate();
  
  // Updating our logging sensors
  if(CONSOLE_DEBUG == true && millis()-last_print >= 100) {
    Serial << "Updating logging sensors..." << endl;
    last_print = millis();
  }
  logger.setLogData_t(LOG_TIME, now());
  //logger.setLogData_f(LOG_IMU_PITCH, compass_pitch);
  //logger.setLogData_f(LOG_IMU_ROLL, compass_roll);
  //logger.setLogData_f(LOG_IMU_YAW, 0.0);
  //logger.setLogData_f(LOG_COMPASS_HEADING, compass_heading);
  logger.setLogData_u8(LOG_GPS_SATS, (int)GPS.satellites);
  logger.setLogData_f(LOG_GPS_HDOP, GPS.HDOP); // TODO: find HDOP in this library
  logger.setLogData_f(LOG_GPS_LATITUDE, (float)GPS.latitudeDegrees);
  logger.setLogData_f(LOG_GPS_LONGITUDE, (float)GPS.longitudeDegrees);
  logger.setLogData_f(LOG_GPS_ALTITUDE, GPS.altitude);
  //logger.setLogData_u16(LOG_BATTERY_SENSOR, batt_val); // 1.33 for single cell, 2.76 for both
  
  logger.updateLogging();

}



