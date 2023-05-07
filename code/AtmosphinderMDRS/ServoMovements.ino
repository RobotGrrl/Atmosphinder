/*
red = left
green = home
blue = right
*/

void servoMovements() {

  // hold the movements for 15 seconds in order to get enough current sensor readings

  if(millis()-last_movement >= MOVEMENT_TIME) {
    movement_stage++;
    if(movement_stage > 15) movement_stage = 0;
    logger.setLogData_u16(LOG_SERVO_POS_END, movement_stage);
    Serial << "\r\n\r\n----->Movement stage: " << movement_stage << endl;
    last_movement = millis();
  }

  uint16_t left_pos;
  uint16_t right_pos;

  //movement_stage = 0;

  switch(movement_stage) {
    case 0:
      left_pos = LEFT_HOME_POS;
      right_pos = RIGHT_HOME_POS;
    break;
    case 1:
      left_pos = LEFT_LEFT_POS;
      right_pos = RIGHT_HOME_POS;
    break;
    case 2:
      left_pos = LEFT_HOME_POS;
      right_pos = RIGHT_HOME_POS;
    break;
    case 3:
      left_pos = LEFT_RIGHT_POS;
      right_pos = RIGHT_HOME_POS;
    break;
    case 4:
      left_pos = LEFT_HOME_POS;
      right_pos = RIGHT_HOME_POS;
    break;
    case 5:
      left_pos = LEFT_HOME_POS;
      right_pos = RIGHT_LEFT_POS;
    break;
    case 6:
      left_pos = LEFT_HOME_POS;
      right_pos = RIGHT_HOME_POS;
    break;
    case 7:
      left_pos = LEFT_HOME_POS;
      right_pos = RIGHT_RIGHT_POS;
    break;
    case 8:
      left_pos = LEFT_HOME_POS;
      right_pos = RIGHT_HOME_POS;
    break;
    case 9:
      left_pos = LEFT_LEFT_POS;
      right_pos = RIGHT_LEFT_POS;
    break;
    case 10:
      left_pos = LEFT_HOME_POS;
      right_pos = RIGHT_HOME_POS;
    break;
    case 11:
      left_pos = LEFT_RIGHT_POS;
      right_pos = RIGHT_RIGHT_POS;
    break;
    case 12:
      left_pos = LEFT_HOME_POS;
      right_pos = RIGHT_HOME_POS;
    break;
    case 13:
      left_pos = LEFT_LEFT_POS;
      right_pos = RIGHT_RIGHT_POS;
    break;
    case 14:
      left_pos = LEFT_HOME_POS;
      right_pos = RIGHT_HOME_POS;
    break;
    case 15:
      left_pos = LEFT_RIGHT_POS;
      right_pos = RIGHT_LEFT_POS;
    break;
  }

  sailTrim(left_pos, right_pos);
  neopixelChange(left_pos, right_pos);
  
}

void sailTrim(uint16_t left_pos, uint16_t right_pos) {
  //if(millis()-last_servo_movement >= 1000) {
    servo_left.writeMicroseconds(left_pos);
    servo_left_backup.writeMicroseconds(left_pos);
    servo_right.writeMicroseconds(right_pos);
    servo_right_backup.writeMicroseconds(right_pos);
    servo_left_pos = left_pos;
    servo_right_pos = right_pos;
    last_servo_movement = millis();
  //}
  logger.setLogData_u16(LOG_SERVO_POS_ARM_L, left_pos);
  logger.setLogData_u16(LOG_SERVO_POS_ARM_R, right_pos);
}


