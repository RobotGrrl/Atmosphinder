void pixyUpdate() {

  blocks = pixy.getBlocks();

  if(blocks) {

    if(DEBUG_PIXY) debugPixyPrint();

    for(int j=0; j<blocks; j++) {
      // qualify the block
      if(pixy.blocks[j].width >= SIZE_THRESH && 
        pixy.blocks[j].height >= SIZE_THRESH && 
        pixy.blocks[j].signature < 5) {

          // good block
          good_blocks[good_blocks_count].index = j;
          good_blocks[good_blocks_count].signature = pixy.blocks[j].signature;
          good_blocks[good_blocks_count].x = pixy.blocks[j].x;
          good_blocks[good_blocks_count].y = pixy.blocks[j].y;
          good_blocks[good_blocks_count].width = pixy.blocks[j].width;
          good_blocks[good_blocks_count].height = pixy.blocks[j].height;
          good_blocks_count++;

      }
    }

    //Serial << "good_blocks_count: " << good_blocks_count << endl;
    // has to be more than two cards
    if(good_blocks_count > 1) {

      struct card temp;

      for(int j=0; j<good_blocks_count; ++j) {
        for(int k=j+1; k<good_blocks_count; ++k) {
          if(good_blocks[j].index >= 0 && good_blocks[k].index >= 0) {
            if(good_blocks[j].x > good_blocks[k].x) {
              temp = good_blocks[j];
              good_blocks[j] = good_blocks[k];
              good_blocks[k] = temp;
            }
          }
        }
      }

      // Serial << "X pos in ascending order: ";
      // for(int i=0; i<good_blocks_count; i++) {
      //   Serial << signatureColour(good_blocks[i].signature) << "(X: " << good_blocks[i].x << "), ";
      // }
      // Serial << endl;

      // send the 1st two to combo detected
      comboDetected( good_blocks[0].signature, good_blocks[1].signature );

    }

    // reset
    for(int i=0; i<good_blocks_count; i++) {
      good_blocks[i].index = -1;
      good_blocks_count = 0;
    }

  }

}


void comboDetected(uint16_t sig_left, uint16_t sig_right) {

  if(DEBUG_PIXY) Serial << "left: " << signatureColour(sig_left) << " right: " << signatureColour(sig_right) << endl;

  sig_combo_prev = sig_combo;
  sig_combo = (sig_left*10)+sig_right;

  if(DEBUG_PIXY) Serial << "sig_combo_prev: " << sig_combo_prev << " sig_combo: " << sig_combo << endl;

  if(millis()-last_combo_time <= 1000 || combo_count == 0) {
    if(sig_combo == sig_combo_prev) {
      combo_count++;
    }
    last_combo_time = millis();
  } else {
    // reset
    combo_count = 0;
    sig_combo_prev = 0;
  }

  if(DEBUG_PIXY) Serial << "---> " << combo_count << endl;

  if(combo_count >= 20) {
    if(sig_combo == sig_combo_prev) {
      // it's confirmed to do an action
      if(DEBUG_PIXY) Serial << "\r\n\r\nACTION!" << endl;
      
      if(CAMERA_AUTON_MODE == 0) {
        cameraAutonAction1(sig_left, sig_right);
      } else if(CAMERA_AUTON_MODE == 1) {
        cameraAutonAction2(sig_left, sig_right);
      }
      
      // reset
      combo_count = 0;
      sig_combo_prev = 0;
    }
  }

}


void cameraAutonAction1(uint16_t sig_left, uint16_t sig_right) {

  sig_combo = (sig_left*10)+sig_right;

  // pink = turn LEFT
  // green = turn RIGHT
  // orange = turn HOME
  // nope --- blue = start OSCILLATE ---  nope

  // L 1 (pink), R 2 (green) = 12
  // L 2 (green), R 1 (pink) = 21

  // L 1 (pink), R 3 (orange) = 13
  // L 3 (orange), R 1 (pink) = 31

  // L 1 (pink), R 4 (blue) = 14
  // L 4 (blue), R 1 (pink) = 41

  // L 2 (green), R 3 (orange) = 23
  // L 3 (orange), R 2 (green) = 32

  // L 2 (green), R 4 (blue) = 24
  // L 4 (blue), R 2 (green) = 42

  // L 4 (blue), R 3 (orange) = 43
  // L 3 (orange), R 4 (blue) = 34

  uint16_t left_pos;
  uint16_t right_pos;

  switch(sig_combo) {
    case 12: // L 1 (pink), R 2 (green) = 12
      if(DEBUG_PIXY) Serial << "auton action: " << 12 << endl;
      left_pos = LEFT_LEFT_POS;
      right_pos = RIGHT_RIGHT_POS;
    break;
    case 21: // L 2 (green), R 1 (pink) = 21
      if(DEBUG_PIXY) Serial << "auton action: " << 21 << endl;
      left_pos = LEFT_RIGHT_POS;
      right_pos = RIGHT_LEFT_POS;
    break;
    case 13: // L 1 (pink), R 3 (orange) = 13
      if(DEBUG_PIXY) Serial << "auton action: " << 13 << endl;
      left_pos = LEFT_LEFT_POS;
      right_pos = RIGHT_HOME_POS;
    break;
    case 31: // L 3 (orange), R 1 (pink) = 31
      if(DEBUG_PIXY) Serial << "auton action: " << 31 << endl;
      left_pos = LEFT_HOME_POS;
      right_pos = RIGHT_LEFT_POS;
    break;
    case 23: // L 2 (green), R 3 (orange) = 23
      if(DEBUG_PIXY) Serial << "auton action: " << 23 << endl;
      left_pos = LEFT_RIGHT_POS;
      right_pos = RIGHT_HOME_POS;
    break;
    case 32: // L 3 (orange), R 2 (green) = 32
      if(DEBUG_PIXY) Serial << "auton action: " << 32 << endl;
      left_pos = LEFT_HOME_POS;
      right_pos = RIGHT_RIGHT_POS;
    break;
  }

  sailTrim(left_pos, right_pos);
  neopixelChange(left_pos, right_pos, false);

}


void cameraAutonAction2(uint16_t sig_left, uint16_t sig_right) {

  sig_combo = (sig_left*10)+sig_right;

  // pink = turn LEFT
  // green = turn RIGHT
  // orange = turn HOME
  // nope --- blue = start OSCILLATE ---  nope

  // L 1 (pink), R 2 (green) = 12
  // L 2 (green), R 1 (pink) = 21

  // L 1 (pink), R 3 (orange) = 13
  // L 3 (orange), R 1 (pink) = 31

  // L 1 (pink), R 4 (blue) = 14
  // L 4 (blue), R 1 (pink) = 41

  // L 2 (green), R 3 (orange) = 23
  // L 3 (orange), R 2 (green) = 32

  // L 2 (green), R 4 (blue) = 24
  // L 4 (blue), R 2 (green) = 42

  // L 4 (blue), R 3 (orange) = 43
  // L 3 (orange), R 4 (blue) = 34

  uint16_t left_pos;
  uint16_t right_pos;

  switch(sig_combo) {
    case 12: // L 1 (pink), R 2 (green) = 12
      if(DEBUG_PIXY) Serial << "auton action: " << 12 << endl;
      MOVEMENT_TIME = 500;
      servoWindMovementsSynchronised();
    break;
    case 21: // L 2 (green), R 1 (pink) = 21
      if(DEBUG_PIXY) Serial << "auton action: " << 21 << endl;
      MOVEMENT_TIME = 500;
      servoWindMovementsOpposite();
    break;
    case 13: // L 1 (pink), R 3 (orange) = 13
      if(DEBUG_PIXY) Serial << "auton action: " << 13 << endl;
      MOVEMENT_TIME = 1000;
      servoWindMovementsSynchronised();
    break;
    case 31: // L 3 (orange), R 1 (pink) = 31
      if(DEBUG_PIXY) Serial << "auton action: " << 31 << endl;
      MOVEMENT_TIME = 1000;
      servoWindMovementsOpposite();
    break;
    case 23: // L 2 (green), R 3 (orange) = 23
      if(DEBUG_PIXY) Serial << "auton action: " << 23 << endl;
      MOVEMENT_TIME = 100;
      servoWindMovementsSynchronised();
    break;
    case 32: // L 3 (orange), R 2 (green) = 32
      if(DEBUG_PIXY) Serial << "auton action: " << 32 << endl;
      MOVEMENT_TIME = 100;
      servoWindMovementsOpposite();
    break;
  }

  sailTrim(left_pos, right_pos);
  neopixelChange(left_pos, right_pos, false);

}


String signatureColour(uint16_t sig) {

  switch(sig) {
    case 1:
      return "Pink";
    break;
    case 2:
      return "Green";
    break;
    case 3:
      return "Orange";
    break;
    case 4:
      return "Blue";
    break;
  }

  return "";
}


void debugPixyPrint() {

  for(int j=0; j<blocks; j++) {

    if(millis()-last_pixy_print >= 1000) {
      sprintf(buf, "Detected %d:\n", blocks);
      Serial.print(buf);
      for (j=0; j<blocks; j++) {
        sprintf(buf, "  block %d: ", j);
        Serial.print(buf); 
        pixy.blocks[j].print();
      }
      last_pixy_print = millis();
    }

  }

}


void pixyBegin() {

  pixy.init();
  for(int i=0; i<20; i++) {
    good_blocks[i].index = -1;
  }
  card_left.index = -1;
  card_right.index = -1;

}


