/*



*/

#include <Streaming.h>
#include "PixyUART.h"

#define MY_DEBUG true
#define SIZE_THRESH 15

PixyUART pixy;
uint16_t blocks;
char buf[32];

int LED = 13;
long last_blink = 0;
long last_print = 0;

struct card {
  uint8_t index;
  uint16_t signature;
  uint16_t width;
  uint16_t height;
  uint16_t x;
  uint16_t y;
};

struct card good_blocks[20];
uint8_t good_blocks_count = 0;
uint16_t sig_combo = 0;
uint16_t sig_combo_prev = 0;
long last_combo_time = 0;
uint16_t combo_count = 0;

struct card card_left;
struct card card_right;
bool combo_detected = false;


void setup() {
  
  Serial.begin(9600); // 9600 baud for the serial *console* (not for the UART connected to Pixy)
  Serial.println("Starting...");
  
  pixy.init();

  pinMode(LED, OUTPUT);

  // init
  for(int i=0; i<20; i++) {
    good_blocks[i].index = -1;
  }
  card_left.index = -1;
  card_right.index = -1;

}

void loop() {
  
  blocks = pixy.getBlocks();

  if(blocks) {

    debugPixyPrint();

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


  if(millis()-last_blink >= 100) {
    digitalWrite(LED, !digitalRead(LED));
    last_blink = millis();
  }


}


void comboDetected(uint16_t sig_left, uint16_t sig_right) {

  Serial << "left: " << signatureColour(sig_left) << " right: " << signatureColour(sig_right) << endl;

  sig_combo_prev = sig_combo;
  sig_combo = (sig_left*10)+sig_right;

  Serial << "sig_combo_prev: " << sig_combo_prev << " sig_combo: " << sig_combo << endl;

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

  Serial << "---> " << combo_count << endl;

  if(combo_count >= 20) {
    if(sig_combo == sig_combo_prev) {
      // it's confirmed to do an action
      Serial << "\r\n\r\nACTION!" << endl;
      delay(100);

      // reset
      combo_count = 0;
      sig_combo_prev = 0;
    }
  }

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

    if(millis()-last_print >= 1000) {
      sprintf(buf, "Detected %d:\n", blocks);
      Serial.print(buf);
      for (j=0; j<blocks; j++) {
        sprintf(buf, "  block %d: ", j);
        Serial.print(buf); 
        pixy.blocks[j].print();
      }
      last_print = millis();
    }

  }

}

