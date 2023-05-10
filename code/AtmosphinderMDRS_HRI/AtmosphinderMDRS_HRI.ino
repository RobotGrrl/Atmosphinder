#include "Board.h"
#include <Streaming.h>
#include "AtmosphinderLogger.h"
#include <Adafruit_GPS.h>
#include <Wire.h>
#include <Adafruit_MS8607.h>
#include <Adafruit_Sensor.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include <Bounce2.h>

#define CONSOLE_DEBUG true
#define OUTDOORS false
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
String air_quality_data = "";
long last_aq_print = 0;

// -- led
bool hb_led = false;

// -- servo related
Servo servo_left;
Servo servo_left_backup;
Servo servo_right;
Servo servo_right_backup;
uint16_t servo_left_pos = 0;
uint16_t servo_right_pos = 0;
uint8_t movement_stage = 0;
long last_movement = 0;
long last_servo_movement = 0;
long MOVEMENT_TIME = 500;
bool servos_attached = false;
 
// -- neopixel related
Adafruit_NeoPixel strip(LED_COUNT, NEOPIXEL_PIN, NEO_RGB + NEO_KHZ800);
long firstPixelHueRB = 0;
long last_rainbow = 0;
long last_move_blink = 0;
bool move_blink = false;
uint32_t red_color = strip.Color(255, 10, 10);
uint32_t orange_color = strip.Color(255, 165, 0);
uint32_t yellow_color = strip.Color(255, 234, 0);
uint32_t green_color = strip.Color(10, 255, 10);
uint32_t cyan_color = strip.Color(0, 255, 255);
uint32_t blue_color = strip.Color(10, 10, 255);
uint32_t purple_color = strip.Color(148, 0, 211);
uint32_t white_color = strip.Color(255, 255, 255);
uint32_t off_color = strip.Color(0, 0, 0);

// -- button related / state machine related
Bounce bounce = Bounce();
int MODE = 0;
bool mode_blink = true;
long last_mode_blink = 0;


void setup() {
  Serial.begin(9600);
  Serial5.begin(9600); // uRADMonitor Air Quality sensor
  Serial5.setTimeout(100);

  initTime(); // should always be first

  gpsBegin();

  sensorBegin();

  logger.begin();
  logger.initLogging();

  pinMode(LED, OUTPUT);
  pinMode(HEARTBEAT_LED, OUTPUT);
  pinMode(AUX_LED, OUTPUT);
  digitalWrite(LED, LOW);
  
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  if(OUTDOORS) {
    strip.setBrightness(200); // Set BRIGHTNESS to about 1/5 (max = 255)
  } else {
    strip.setBrightness(5); // Set BRIGHTNESS to about 1/5 (max = 255)
  }

  pinMode(CENTRAL_LED, OUTPUT);
  pinMode(WIND_SENSOR, INPUT);

  bounce.attach(BUTTON_PIN, INPUT);
  bounce.interval(5);
  
  servo_left.attach(SERVO1_PIN);
  servo_left_backup.attach(SERVO2_PIN);
  servo_right.attach(SERVO3_PIN);
  servo_right_backup.attach(SERVO4_PIN);

  sailTrim(LEFT_HOME_POS, RIGHT_HOME_POS);
  delay(100);

  servo_left.detach();
  servo_left_backup.detach();
  servo_right.detach();
  servo_right_backup.detach();

}

void loop() {

  gpsUpdate();

  sensorUpdate();

  bounce.update();

  windLEDs();


  // -------------------------------------------
  // -- button updating

  if(bounce.changed()) {
    int deboucedInput = bounce.read();
    if (deboucedInput == LOW) {
      movement_stage = 0;
      
      MODE++;
      if(MODE > 13) MODE = 0;

      // state machine upon entry
      switch(MODE) {
        case 0: // waiting
          servos_attached = false;
        break;
        case 1: // sail test
          servos_attached = true;
        break;
        case 2: // waiting - sail test
          servos_attached = false;
        break;
        case 3: // sail test
          servos_attached = true;
        break;
        case 4: // waiting - sail test
          servos_attached = false;
        break;
        case 5: // sail test
          servos_attached = true;
        break;
        case 6: // waiting - wind auton test
          servos_attached = false;
        break;
        case 7: // wind auton test
          servos_attached = true;
          movement_stage = 0;
        break;
        case 8: // waiting - wind auton test
          servos_attached = false;
        break;
        case 9: // wind auton test
          servos_attached = true;
          movement_stage = 0;
        break;
        case 10: // waiting - camera test
          servos_attached = false;
        break;
        case 11: // camera test
          servos_attached = true;
        break;
        case 12: // waiting - camera test
          servos_attached = false;
        break;
        case 13: // camera test
          servos_attached = true;
        break; 
        case 14: // party mode
          servos_attached = true;
        break; 
      }

      if(servos_attached) {
        servo_left.attach(SERVO1_PIN);
        servo_left_backup.attach(SERVO2_PIN);
        servo_right.attach(SERVO3_PIN);
        servo_right_backup.attach(SERVO4_PIN);
      } else {
        servo_left.detach();
        servo_left_backup.detach();
        servo_right.detach();
        servo_right_backup.detach();
      }
      // --

    }
  }
  // -------------------------------------------

  // -------------------------------------------
  // -- state machine
  switch(MODE) {
    case 0: // waiting
      digitalWrite(LED, LOW);
      showMode(0);
      //rainbowNoDelay(3);
    break;
    case 1: // sail test
      MOVEMENT_TIME = 500;
      digitalWrite(LED, HIGH);
      servoMovements();
    break;
    case 2: // waiting - sail test
      digitalWrite(LED, LOW);
      showMode(2);
    break;
    case 3: // sail test
      MOVEMENT_TIME = 1000;
      digitalWrite(LED, HIGH);
      servoMovements();
    break;
    case 4: // waiting - sail test
      digitalWrite(LED, LOW);
      showMode(4);
    break;
    case 5: // sail test
      MOVEMENT_TIME = 100;
      digitalWrite(LED, HIGH);
      servoMovements();
    break;
    case 6: // waiting - wind auton test
      digitalWrite(LED, LOW);
      showMode(6);
    break;
    case 7: // wind auton test
      windAuton1();
    break;
    case 8: // waiting - wind auton test
      digitalWrite(LED, LOW);
      showMode(8);
    break;
    case 9: // wind auton test
      windAuton2();
    break;
    case 10: // waiting - camera test
      showMode(10);
    break;
    case 11: // camera test
      
    break;
    case 12: // waiting - camera test
      showMode(12);
    break;
    case 13: // camera test

    break;
    case 14: // party mode
      rainbowNoDelay(3);
      MOVEMENT_TIME = 100;
      digitalWrite(LED, HIGH);
      servoMovements();
    break;
  }
  // -------------------------------------------
  
  // -------------------------------------------
  // -- logger update

  // Updating our logging sensors
  if(CONSOLE_DEBUG == true && millis()-last_print >= 1000) {
    Serial << "Updating logging sensors..." << endl;
    last_print = millis();
  }

  logger.setLogData_t(LOG_TIME, now());

  logger.setLogData_f(LOG_PRESSURE, pressure.pressure);

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
  
  aqSensorUpdate();

  logger.updateLogging();

  // -------------------------------------------

  // -------------------------------------------
  // -- led update

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

  // -------------------------------------------


}


void windLEDs() {

  bool windLEDs_debug = false;

  uint16_t temp_wind_sensor_val = wind_sensor_val;
  if(wind_sensor_val > 170) temp_wind_sensor_val = 170;
  int leds_active = map(temp_wind_sensor_val, 0, 170, 1, 17);
  
  for(int i=LED_COUNT; i>4; i--) {
    
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
    uint16_t left_pos = LEFT_HOME_POS;
    uint16_t right_pos = RIGHT_HOME_POS;
    sailTrim(left_pos, right_pos);
    neopixelChange(left_pos, right_pos, false);

  } else if(wind_sensor_val >= WIND_THRESH1 && wind_sensor_val < WIND_THRESH3) {
    // turns, synchronised dirs
    MOVEMENT_TIME = 250;
    servoWindMovementsSynchronised();

  } else if(wind_sensor_val >= WIND_THRESH3) {
    // turns, opposite dirs
    MOVEMENT_TIME = 250;
    servoWindMovementsOpposite();
  }

}


void windAuton2() {

  if(wind_sensor_val < WIND_THRESH1) {
    // do nothing
    uint16_t left_pos = LEFT_HOME_POS;
    uint16_t right_pos = RIGHT_HOME_POS;
    sailTrim(left_pos, right_pos);
    neopixelChange(left_pos, right_pos, false);

  } else if(wind_sensor_val >= WIND_THRESH1 && wind_sensor_val < WIND_THRESH2) {
    // gently turn, synchronised dirs
    MOVEMENT_TIME = 2000;
    servoWindMovementsSynchronised();

  } else if(wind_sensor_val >= WIND_THRESH2 && wind_sensor_val < WIND_THRESH3) {
    // faster turns, opposite dirs
    MOVEMENT_TIME = 500;
    servoWindMovementsSynchronised();

  } else if(wind_sensor_val >= WIND_THRESH3 && wind_sensor_val < WIND_THRESH4) {
    // gently turns, opposite dirs
    MOVEMENT_TIME = 2000;
    servoWindMovementsOpposite();

  } else if(wind_sensor_val >= WIND_THRESH4) {
    // faster turns, opposite dirs
    MOVEMENT_TIME = 500;
    servoWindMovementsOpposite();

  }

}





