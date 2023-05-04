#include "Board.h"
#include <Streaming.h>
#include "AtmosphinderLogger.h"
#include <Adafruit_GPS.h>
#include <Wire.h>
#include <Adafruit_MS8607.h>
#include <Adafruit_Sensor.h>

#define CONSOLE_DEBUG true
#define GPSSerial Serial4
#define GPSECHO true

Adafruit_GPS GPS(&GPSSerial);
AtmosphinderLogger logger = AtmosphinderLogger();

// -- gps related --
uint32_t timer = millis();
long last_print = 0;

// -- sensor related --
bool read_sensors = false;
Adafruit_MS8607 ms8607;
sensors_event_t temp, pressure, humidity;
bool pht_enabled = false;
long last_sensor_print = 0;
uint16_t batt_level = 0;
long last_batt_level_print = 0;
uint16_t current_sensor_val = 0;
uint16_t wind_sensor_val;

// -- led
bool hb_led = false;


void setup() {
  Serial.begin(9600);

  initTime(); // should always be first

  gpsBegin();

  sensorBegin();

  logger.begin();
  logger.setLoggingLed(13);
  logger.initLogging();

  pinMode(LED, OUTPUT);
  pinMode(HEARTBEAT_LED, OUTPUT);
  pinMode(AUX_LED, OUTPUT);
  pinMode(CENTRAL_LED, OUTPUT);
  pinMode(WIND_SENSOR, INPUT);

}

void loop() {

  gpsUpdate();

  sensorUpdate();
  
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

  logger.setLogData_f(LOG_PRESSURE, pressure.pressure);
  logger.setLogData_f(LOG_HUMIDITY, humidity.relative_humidity);
  logger.setLogData_f(LOG_TEMPERATURE, temp.temperature);
  
  logger.setLogData_u8(LOG_GPS_SATS, (int)GPS.satellites);
  logger.setLogData_f(LOG_GPS_LATITUDE, (float)GPS.latitudeDegrees);
  logger.setLogData_f(LOG_GPS_LONGITUDE, (float)GPS.longitudeDegrees);
  logger.setLogData_f(LOG_GPS_ALTITUDE, GPS.altitude);
  logger.setLogData_f(LOG_GPS_SPEED, GPS.speed);
  logger.setLogData_f(LOG_GPS_ANGLE, GPS.angle);
  logger.setLogData_f(LOG_GPS_HDOP, GPS.HDOP);
  logger.setLogData_f(LOG_GPS_VDOP, GPS.VDOP);
  logger.setLogData_f(LOG_GPS_PDOP, GPS.PDOP);

  logger.setLogData_u16(LOG_BATTERY_SENSOR, batt_level);
  logger.setLogData_u16(LOG_WIND_SENSOR, wind_sensor_val);
  logger.setLogData_u16(LOG_SERVO_CURRENT_SENSOR, current_sensor_val);
  
  logger.updateLogging();


  // heartbeat led (blue) = uc working
  uint16_t slice = millis() % 1000;
  digitalWrite(HEARTBEAT_LED, slice < 100 || (slice > 200 && slice < 300));

  // aux led (orange) = gps fix
  if(GPS.fix) {
    digitalWrite(AUX_LED, slice < 100 || (slice > 200 && slice < 300));
  }

  // central led (green) = logging
  if(logger.isLogging() == true) {
    digitalWrite(CENTRAL_LED, slice < 100 || (slice > 200 && slice < 300));
  }


}



