#include <Streaming.h>

String air_quality_data = "";

void setup() {
  Serial.begin(9600);
  Serial5.begin(9600); // uRADMonitor Air Quality sensor
  Serial5.setTimeout(100);

  Serial << "uRADMonitor Test" << endl;

}

void loop() {
  
  // Updating our air quality sensor logger
  int bytes_available = Serial5.available();
  if(bytes_available > 50) {
    Serial << "Updating AQ sensor..." << endl;
    air_quality_data = Serial5.readString();
    air_quality_data.remove(air_quality_data.length()-2);
  }
  
  if(air_quality_data.length() > 0) {
    Serial << air_quality_data << endl;
    //logger.setLogData_s(LOG_AQ_DATA, air_quality_data);
  }

}
