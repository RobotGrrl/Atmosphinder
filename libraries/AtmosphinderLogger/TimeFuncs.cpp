#include "AtmosphinderLogger.h"

void AtmosphinderLogger::digitalClockDisplay() {
  // from the TimeTeensy3 example code
  // most likely by Paul S
  
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void AtmosphinderLogger::initTime() {
  // from the TimeTeensy3 example code
  // most likely by Paul S
  
  // set the Time library to use Teensy 3.0's RTC to keep time
  setSyncProvider(getTeensy3Time);

  if (timeStatus()!= timeSet) {
    Serial.println("Unable to sync with the RTC");
  } else {
    Serial.println("RTC has set the system time");
  }
}

time_t AtmosphinderLogger::getTeensy3Time() {
  return Teensy3Clock.get();
}

void AtmosphinderLogger::printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
