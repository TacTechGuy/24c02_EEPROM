#include <24c02.h>

const uint8_t writeControlPin = 2;
const uint8_t I2Caddress = 0x51;
// Address range from 0x50 --> 0x57 (1010 +[A2]+[A1]+[A0])
//                                   1010   0    0    1


// used to setup connection to the EEPROM IC
EEPROMic eepromDataStorage(I2Caddress, writeControlPin);

uint8_t distance = 0;

struct myStorage {
  float incomingData[255];
  uint8_t currentPage;
  uint8_t previousPage = 0;
  uint8_t maxPageValue = 16;
  bool read = false;
  uint8_t page = 0;
} eeprom;

//***THIS WILL READ THE ENTIRE CHIP! NEED TO SEE IF I CAN ADD THIS TO THE ACTUAL I2C_MOTORDRIVER LIBRARY***
//changed from uint8_t to float on the return value to pick up the 0x13 MPH address
// changed 4-5-2023
float checkEEPROM(bool set = true) {

  if (set) {
    for (eeprom.currentPage = eeprom.previousPage; eeprom.currentPage < eeprom.maxPageValue; eeprom.currentPage++) {

      eeprom.page++;
      SerialUSB.print("page number: ");
      SerialUSB.println(eeprom.page);
      //SerialUSB.println(eeprom.currentPage);
      for (uint8_t i = 0; i < 16; i++) {
        eeprom.incomingData[i + (16 * eeprom.currentPage)] = eepromDataStorage.readData(i + (16 * eeprom.currentPage));
        SerialUSB.print("address location: ");
        SerialUSB.print(i + (16 * eeprom.currentPage), HEX);
        SerialUSB.print(" data read: ");
        SerialUSB.println(eeprom.incomingData[i + (16 * eeprom.currentPage)]);
      }
      if (eeprom.page > 15) {
        eeprom.currentPage = 0;
        eeprom.page = 0;
        set = false;
        break;
      }
    }
  }
}


void setup() {

  if (!eepromDataStorage.begin()) {
    SerialUSB.println("we have a problem connecting to the 24C02 EEPROM IC!");
  } else {
    SerialUSB.println("we are connected to the 24C02 EEPROM IC!");
    //set the flag so we can read from the EEPROM
    eeprom.read = true;
  }




  // read data from a location on the eeprom
  distance = eepromDataStorage.readData(0x13);

  // Used to update the data at a location, it first checks the data and then replaces it if its new
  eepromDataStorage.updateData(0x12, 15);


  // Print out the entire contents of the EEPROM
  checkEEPROM();
}

void loop() {
  // put your main code here, to run repeatedly:
}
