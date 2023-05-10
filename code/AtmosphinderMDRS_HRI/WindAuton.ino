
void windLEDs() {

  bool windLEDs_debug = false;

  uint16_t temp_wind_sensor_val = wind_sensor_val;
  if(wind_sensor_val > 200) temp_wind_sensor_val = 200;
  int leds_active = map(temp_wind_sensor_val, 100, 200, 4, 17);
  
  for(int i=4; i<LED_COUNT; i++) {
    
    if(i < WIND_LED_THRESH1) {
      strip.setPixelColor(i, blue_color);
    } else if(i >= WIND_LED_THRESH1 && i < WIND_LED_THRESH2) {
      strip.setPixelColor(i, green_color);
    } else if(i >= WIND_LED_THRESH2 && i < WIND_LED_THRESH3) {
      strip.setPixelColor(i, yellow_color);
    } else if(i >= WIND_LED_THRESH3 && i <= WIND_LED_THRESH4) {
      strip.setPixelColor(i, orange_color);
    } else if(i > WIND_LED_THRESH4) {
      strip.setPixelColor(i, red_color);
    }

    if(i > leds_active) {
      strip.setPixelColor(i, off_color);
    }
  }
  strip.show();

}



void windAuton1() {

  if(wind_sensor_val < WIND_THRESH1) {
    // do nothing - go to home pos
    if(DEBUG_WIND_AUTON) Serial << "windAuton1() - do nothing (" << wind_sensor_val << ") " << endl;
    uint16_t left_pos = LEFT_HOME_POS;
    uint16_t right_pos = RIGHT_HOME_POS;
    sailTrim(left_pos, right_pos);
    neopixelChange(left_pos, right_pos, false);

  } else if(wind_sensor_val >= WIND_THRESH1 && wind_sensor_val < WIND_THRESH3) {
    // turns, synchronised dirs
    if(DEBUG_WIND_AUTON) Serial << "windAuton1() - synchronised (" << wind_sensor_val << ") " << endl;
    MOVEMENT_TIME = 500;
    servoWindMovementsSynchronised();

  } else if(wind_sensor_val >= WIND_THRESH3) {
    // turns, opposite dirs
    if(DEBUG_WIND_AUTON) Serial << "windAuton1() - opposite (" << wind_sensor_val << ") " << endl;
    MOVEMENT_TIME = 500;
    servoWindMovementsOpposite();
  }

}


void windAuton2() {

  if(wind_sensor_val < WIND_THRESH1) {
    // do nothing
    if(DEBUG_WIND_AUTON) Serial << "windAuton2() - do nothing (" << wind_sensor_val << ") " << endl;
    uint16_t left_pos = LEFT_HOME_POS;
    uint16_t right_pos = RIGHT_HOME_POS;
    sailTrim(left_pos, right_pos);
    neopixelChange(left_pos, right_pos, false);

  } else if(wind_sensor_val >= WIND_THRESH1 && wind_sensor_val < WIND_THRESH2) {
    // gently turn, synchronised dirs
    if(DEBUG_WIND_AUTON) Serial << "windAuton2() - synchronised 1000 ms (" << wind_sensor_val << ") " << endl;
    MOVEMENT_TIME = 1000;
    servoWindMovementsSynchronised();

  } else if(wind_sensor_val >= WIND_THRESH2 && wind_sensor_val < WIND_THRESH3) {
    // faster turns, synchronised dirs
    if(DEBUG_WIND_AUTON) Serial << "windAuton2() - synchronised 500 ms (" << wind_sensor_val << ") " << endl;
    MOVEMENT_TIME = 500;
    servoWindMovementsSynchronised();

  } else if(wind_sensor_val >= WIND_THRESH3 && wind_sensor_val < WIND_THRESH4) {
    // gently turns, opposite dirs
    if(DEBUG_WIND_AUTON) Serial << "windAuton2() - opposite 1000 ms (" << wind_sensor_val << ") " << endl;
    MOVEMENT_TIME = 1000;
    servoWindMovementsOpposite();

  } else if(wind_sensor_val >= WIND_THRESH4) {
    // faster turns, opposite dirs
    if(DEBUG_WIND_AUTON) Serial << "windAuton2() - opposite 500 ms (" << wind_sensor_val << ") " << endl;
    MOVEMENT_TIME = 500;
    servoWindMovementsOpposite();

  }

}

