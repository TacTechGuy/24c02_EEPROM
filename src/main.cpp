#include "Arduino.h"
#include "24c02.h"
#include <Wire.h>

#define SUCCESS 0

#if defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_STM32)
  #define DEBUG_SERIAL SerialUSB
#else
  #define DEBUG_SERIAL Serial
#endif

#ifndef UNIT_TEST

const uint8_t writeControlPin = 4;
const uint8_t I2Caddress = 0x51;
// Address range from 0x50 --> 0x57 (1010 +[A2]+[A1]+[A0])
//                                   1010   0    0    1


// used to setup connection to the EEPROM IC
EEPROMic eepromDataStorage(I2Caddress, writeControlPin);

// Global Variables For Testing
uint8_t distance = 0;
uint8_t buf[20] = {0};

bool runOnce = true;
uint8_t count = 0;
void setup() {
  Wire.begin();
  DEBUG_SERIAL.begin(9600);

  while (!DEBUG_SERIAL){
    ;
  }
  
  // Establish Connection
  if (eepromDataStorage.begin() != SUCCESS) {
    DEBUG_SERIAL.println("we have a problem connecting to the 24C02 EEPROM IC!");
  } else {
    DEBUG_SERIAL.println("we are connected to the 24C02 EEPROM IC!");
  }


  // read data from a location on the eeprom
  distance = eepromDataStorage.readData(0x03);

  snprintf((char*)buf,sizeof(buf), "Distance:%d \n\r", distance);

  DEBUG_SERIAL.println((char*)buf);

  // Used to update the data at a location, it first checks the data and then replaces it if its new
  //eepromDataStorage.updateData(0x03, 17);


  // Print out the entire contents of the EEPROM
  //eepromDataStorage.readIC();

  //delay(10);
  // Clear the EEPROM with [value = 0] if not specified
  //eepromDataStorage.clearIC(0);
  //eepromDataStorage.clearPageIC(1,10);
  //delay(10);
  //eepromDataStorage.clearPageIC(14,125);
  //delay(10);

  //eepromDataStorage.readIC();
}

void loop() {
  // put your main code here, to run repeatedly:
  eepromDataStorage.stateMachine();
  
  // Trip the internal state machine -> print EEPROM locations x2
  if (runOnce){
    // IDLE = 0
    if (((uint8_t)eepromDataStorage.getTimerState()) == 0){
      count +=1;
      eepromDataStorage.requestRead();
    }
    
    if (count == 2){
      runOnce = false;
    }
    
  }
  
}

#endif