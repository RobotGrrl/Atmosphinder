/*
green = home
red = left
blue = right
*/

#define LEFT_HOME_POS 90
#define RIGHT_HOME_POS 90
#define LEFT_LEFT_POS 0
#define LEFT_RIGHT_POS 180
#define RIGHT_LEFT_POS 180
#define RIGHT_RIGHT_POS 0

void servoMovements() {

  // hold the movements for 15 seconds in order to get enough current sensor readings

  if(millis()-last_servo_movement >= 15UL*1000UL) {
    movement_stage++;
    if(movement_stage > 15) movement_stage = 0;
  }

  switch(movement_stage) {
    case 0:
      sailTrim(LEFT_HOME_POS, RIGHT_HOME_POS);
    break;
    case 1:
      sailTrim(LEFT_LEFT_POS, RIGHT_HOME_POS);
    break;
    case 2:
      sailTrim(LEFT_HOME_POS, RIGHT_HOME_POS);
    break;
    case 3:
      sailTrim(LEFT_RIGHT_POS, RIGHT_HOME_POS);
    break;
    case 4:
      sailTrim(LEFT_HOME_POS, RIGHT_HOME_POS);
    break;
    case 5:
      sailTrim(LEFT_HOME_POS, RIGHT_LEFT_POS);
    break;
    case 6:
      sailTrim(LEFT_HOME_POS, RIGHT_HOME_POS);
    break;
    case 7:
      sailTrim(LEFT_HOME_POS, RIGHT_RIGHT_POS);
    break;
    case 8:
      sailTrim(LEFT_HOME_POS, RIGHT_HOME_POS);
    break;
    case 9:
      sailTrim(LEFT_LEFT_POS, RIGHT_LEFT_POS);
    break;
    case 10:
      sailTrim(LEFT_HOME_POS, RIGHT_HOME_POS);
    break;
    case 11:
      sailTrim(LEFT_RIGHT_POS, RIGHT_RIGHT_POS);
    break;
    case 12:
      sailTrim(LEFT_HOME_POS, RIGHT_HOME_POS);
    break;
    case 13:
      sailTrim(LEFT_LEFT_POS, RIGHT_RIGHT_POS);
    break;
    case 14:
      sailTrim(LEFT_HOME_POS, RIGHT_HOME_POS);
    break;
    case 15:
      sailTrim(LEFT_RIGHT_POS, RIGHT_LEFT_POS);
    break;
  }

}

void sailTrim(uint16_t left_pos, uint16_t right_pos) {
  servo_left.write(left_pos);
  servo_left_backup.write(left_pos);
  servo_right.write(right_pos);
  servo_right_backup.write(right_pos);
  servo_left_pos = left_pos;
  servo_right_pos = right_pos;
  last_servo_movement = millis();
}


