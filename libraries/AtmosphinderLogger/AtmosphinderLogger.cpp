#include "AtmosphinderLogger.h"

AtmosphinderLogger::AtmosphinderLogger() {

}

void AtmosphinderLogger::begin() {
  // how often to write a new file (mins)
  TIMEOUT_MINS = 15;
  
  // how often to write a new file (seconds)
  TIMEOUT_SECS = 0;
  
  // how often to log data (seconds)
  INTERVAL_SECS = 1;

  log_headers[0] = "Time";
  log_headers[1] = "Motor A Speed";
  log_headers[2] = "Motor A Dir";
  log_headers[3] = "Motor B Speed";
  log_headers[4] = "Motor B Dir";
  log_headers[5] = "Servo Rail Current Sensor";
  log_headers[6] = "Servo Pos - Arm L";
  log_headers[7] = "Servo Pos - Arm R";
  log_headers[8] = "Servo Pos - End";
  log_headers[9] = "Servo Pos - Hopper";

  log_headers[10] = "Servo Pos - Lid";
  log_headers[11] = "Servo Pos - Extra";
  log_headers[12] = "Servo Current";
  log_headers[13] = "LED - Front L";
  log_headers[14] = "LED - Front R";
  log_headers[15] = "LED - Back L";
  log_headers[16] = "LED - Back R";
  log_headers[17] = "IMU Pitch";
  log_headers[18] = "IMU Roll";
  log_headers[19] = "IMU Yaw";

  log_headers[20] = "Compass Heading";
  log_headers[21] = "GPS Sats";
  log_headers[22] = "GPS Latitude";
  log_headers[23] = "GPS Longitude";
  log_headers[24] = "GPS Altitude";
  log_headers[25] = "GPS Speed";
  log_headers[26] = "GPS Angle";
  log_headers[27] = "GPS HDOP";
  log_headers[28] = "GPS VDOP";
  log_headers[29] = "GPS PDOP";

  log_headers[30] = "Battery";
  log_headers[31] = "Wind";
  log_headers[32] = "Comm Xbee Latency";
  log_headers[33] = "Comm Arduino Latency";
  log_headers[34] = "Pressure";
  log_headers[35] = "Humidity";
  log_headers[36] = "Temperature";

  log_headers[37] = "uRADMonitor";

  logdata = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  LOGGING = false;
  logging_path = "";
  last_time_check = 0;
  last_log_ms = 0;

  LOG_LED = 13;
}

void AtmosphinderLogger::setLoggingLed(int pin) {
  LOG_LED = pin;
}

void AtmosphinderLogger::initLogging() {
  pinMode(LOG_LED, OUTPUT);

  if(!initSd()) {
    Serial.println("!!! Unable to start logging !!!");
    LOGGING = false;
  } else {
  
    // only log if we have space!
    if(getPercentAvailable() >= 5.0) {
      LOGGING = true;
      createLogFile();
    } else {
      Serial.println("!!! Not enough space available for logging !!!");
      LOGGING = false;
    }

  }
}

void AtmosphinderLogger::updateLogging() {
  if(LOGGING) {

    // TODO: This will be more precise as a timer interrupt

    if(millis()-last_time_check >= 1000) {
      logFileTimer();
      last_time_check = millis();
    }
  
    //if(difference_sec <= 0) {
    if(millis()-last_log_ms >= LOG_INTERVAL_MS) {

      // get the data
      // NOTE: we are assuming this is done from the sketch side
  
      // open the file
      openLogFile();
  
      // log it to file
      if(DEBUG_PRINTS) Serial.print(millis());
      if(DEBUG_PRINTS) Serial.println(" Logging data");
      writeLogData();
  
      // close the file
      closeLogFile();
      last_log_ms = millis();
       
    }

    digitalWrite(LOG_LED, HIGH);

  } else {
    digitalWrite(LOG_LED, LOW);
  }
}


bool AtmosphinderLogger::isLogging() {
  return LOGGING;
}


