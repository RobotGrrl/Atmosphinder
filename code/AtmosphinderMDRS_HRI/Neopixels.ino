void neopixelChange(uint16_t left_pos, uint16_t right_pos, bool key_step) {
  
  uint32_t color = strip.Color(0, 0, 0);

  if(left_pos == LEFT_HOME_POS) {
    if(!move_blink) {
      color = green_color;
    } else {
      color = off_color;
    }
    strip.setPixelColor(0, color); // left back
    strip.setPixelColor(3, color); // left front
  }
  if(left_pos == LEFT_LEFT_POS) {
    if(!move_blink) {
      color = red_color;
    } else {
      color = off_color;
    }
    strip.setPixelColor(0, color); // left back
    strip.setPixelColor(3, color); // left front
  }
  if(left_pos == LEFT_RIGHT_POS) {
    if(!move_blink) {
      color = blue_color;
    } else {
      color = off_color;
    }
    strip.setPixelColor(0, color); // left back
    strip.setPixelColor(3, color); // left front
  }

  if(right_pos == RIGHT_HOME_POS) {
    if(!move_blink) {
      color = green_color;
    } else {
      color = off_color;
    }
    strip.setPixelColor(1, color); // right back
    strip.setPixelColor(2, color); // right front
  }
  if(right_pos == RIGHT_LEFT_POS) {
    if(!move_blink) {
      color = red_color;
    } else {
      color = off_color;
    }
    strip.setPixelColor(1, color); // right back
    strip.setPixelColor(2, color); // right front
  }
  if(right_pos == RIGHT_RIGHT_POS) {
    if(!move_blink) {
      color = blue_color;
    } else {
      color = off_color;
    }
    strip.setPixelColor(1, color); // right back
    strip.setPixelColor(2, color); // right front
  }

  move_blink = false;
  // if(millis()-last_move_blink >= 100) {
  //   move_blink = !move_blink;
  //   last_move_blink = millis();
  // }
  
  if(key_step) {
    color = white_color;
    strip.setPixelColor(0, color); // left back
    strip.setPixelColor(1, color); // right back
    strip.setPixelColor(2, color); // right front
    strip.setPixelColor(3, color); // left front
  }

  strip.show();
}


void showMode(uint8_t pixel_mode) {

  uint32_t color = strip.Color(0, 0, 0);

  uint16_t mode_delay = 1000;

  for(int i=0; i<4; i++) {
    strip.setPixelColor(i, off_color);
  }

  switch(pixel_mode) {
    case 0: // sail test
      mode_delay = 500;
      if(mode_blink) {
        color = purple_color;
      } else {
        color = off_color;
      }
    break;
    case 2: // sail test
      mode_delay = 1000;
      if(mode_blink) {
        color = purple_color;
      } else {
        color = off_color;
      }
    break;
    case 4: // sail test
      mode_delay = 100;
      if(mode_blink) {
        color = purple_color;
      } else {
        color = off_color;
      }
    break;
    case 6: // wind auton test
      mode_delay = 100;
      if(mode_blink) {
        color = yellow_color;
      } else {
        color = off_color;
      }
    break;
    case 8: // wind auton test
      mode_delay = 500;
      if(mode_blink) {
        color = yellow_color;
      } else {
        color = off_color;
      }
    break;
    case 10: // camera test
      mode_delay = 100;
      if(mode_blink) {
        color = cyan_color;
      } else {
        color = off_color;
      }
    break;
    case 12: // camera test
      mode_delay = 500;
      if(mode_blink) {
        color = cyan_color;
      } else {
        color = off_color;
      }
    break;
  }


  strip.setPixelColor(1, color); // right back
  strip.setPixelColor(3, color); // left front
  strip.show();


  if(millis()-last_mode_blink >= mode_delay) {
    mode_blink = !mode_blink;
    last_mode_blink = millis();
  }

}


// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    strip.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}


// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbowNoDelay(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  
  if(millis()-last_rainbow > wait) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    strip.rainbow(firstPixelHueRB);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    strip.show(); // Update strip with new contents
    firstPixelHueRB += 256;
    if(firstPixelHueRB < 5*65536) {
      //
    } else {
      firstPixelHueRB = 0;
    }
    last_rainbow = millis();
    //delay(wait);  // Pause for a moment
  }
}



