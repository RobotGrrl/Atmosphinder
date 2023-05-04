#include <Streaming.h>
#include "AtmosphinderLogger.h"

AtmosphinderLogger logger = AtmosphinderLogger();

void setup() {
  Serial.begin(9600);

  initTime(); // should always be first

  logger.begin();
  logger.setLoggingLed(13);
  logger.initLogging();

}

void loop() {

  String new_data = "a, " + (int)random(0, 100);
  new_data += ", b, " + (int)random(0, 10);
  
  logger.setLogData_s(LOG_MOTOR_A_SPEED, new_data);
  logger.updateLogging();

}



