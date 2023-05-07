#include <Streaming.h>
#include <Servo.h>

#define SERVO1_PIN 4
#define SERVO2_PIN 5
#define SERVO3_PIN 6
#define SERVO4_PIN 9
#define SERVO5_PIN 23
#define SERVO6_PIN 22

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

void setup() {
  Serial.begin(9600);

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);
  servo4.attach(SERVO4_PIN);
  servo5.attach(SERVO5_PIN);
  servo6.attach(SERVO6_PIN);

  Serial << "Servo test" << endl;

}

void loop() {
  
  // left
  // 1700 = right
  // 2100 = home
  // 2700 = left

  // right
  // 2200 = home
  // 2800 = left
  // 1800 = right

  //servo1.writeMicroseconds(1700); // left
  servo3.writeMicroseconds(2800); // right

  // for(int pos=0; pos<180; pos++) {
  //   //servo1.write(pos);
  //   //servo2.write(pos);
  //   servo3.write(pos);
  //   servo4.write(pos);
  //   servo5.write(pos);
  //   servo6.write(pos);
  //   Serial << pos << endl;
  //   delay(15);
  // }

  // for(int pos=180; pos>0; pos--) {
  //   //servo1.write(pos);
  //   //servo2.write(pos);
  //   servo3.write(pos);
  //   servo4.write(pos);
  //   servo5.write(pos);
  //   servo6.write(pos);
  //   Serial << pos << endl;
  //   delay(15);
  // }

}

