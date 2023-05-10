void sensorUpdate() {

  wind_sensor_val = analogRead(WIND_SENSOR);

  if(!read_sensors) return;

  batt_level = analogRead(BATT_LEVEL_PIN);
  current_sensor_val = analogRead(CURRENT_SENSOR);

  if(millis()-last_batt_level_print >= 500) {
    Serial << "batt level: " << batt_level << endl;
    Serial << "current sensor: " << current_sensor_val << endl;
    last_batt_level_print = millis();
  }

  if(pht_enabled) {

    ms8607.getEvent(&pressure, &temp, &humidity);

    if(millis()-last_sensor_print >= 500) {
      Serial << "pressure: " << pressure.pressure << " hPa" << endl;
      Serial << "humidity: " << humidity.relative_humidity << " %rH" << endl;
      Serial << "temperature: " << temp.temperature << " deg C" << endl;
      last_sensor_print = millis();
    }

  }

  read_sensors = false;

}


void aqSensorUpdate() {

  // Updating our air quality sensor logger
  // Serial5 is 9600 baud
  int bytes_available = Serial5.available();
  if(bytes_available > 50) {
    Serial << "Updating AQ sensor..." << endl;
    air_quality_data = Serial5.readString();
    air_quality_data.remove(air_quality_data.length()-2);
  }
  
  if(air_quality_data.length() > 0) {
    if(millis()-last_aq_print >= 1000) {
      Serial << air_quality_data << endl;
      last_aq_print = millis();
    }
    logger.setLogData_s(LOG_AQ_DATA, air_quality_data);
  }
  
}


void sensorBegin() {

  pinMode(BATT_LEVEL_PIN, INPUT);
  pinMode(CURRENT_SENSOR, INPUT);

  if(!ms8607.begin()) {
    Serial.println("Failed to find MS8607 chip");
    pht_enabled = false;
  } else {
    Serial.println("MS8607 Found!");
    ms8607.setHumidityResolution(MS8607_HUMIDITY_RESOLUTION_OSR_8b);
    pht_enabled = true;
  }

}

