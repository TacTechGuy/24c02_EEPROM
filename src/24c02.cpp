#include "24c02.h"

#if defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_STM32)
  #define DEBUG_SERIAL SerialUSB
#else
  #define DEBUG_SERIAL Serial
#endif

uint16_t EEPROMic::receive16bits() {
  uint16_t registerBits;
  uint8_t MSB = _i2cPort->read();
  uint8_t LSB = _i2cPort->read();

  return (registerBits = MSB << 8 | LSB);
}
EEPROMic::EEPROMic(uint8_t address, uint8_t writeControlPin) {
  _address = address;
  _writeControlPin = writeControlPin;
}

bool EEPROMic::begin(TwoWire &wirePort) {

  _i2cPort = &wirePort;

  _i2cPort->beginTransmission(_address);
  uint8_t _ret = _i2cPort->endTransmission();
  if (!_ret)
    return true;
  else
    return false;
}
//changed from uint8_t to float so I dont have to do anything on the front end as far as converting goes
//changed 4-5-2023
float EEPROMic::readData(uint8_t location) {

  _i2cPort->beginTransmission(_address);
  _i2cPort->write(location);
  _i2cPort->endTransmission();

  delay(5);

  if (location == 0x13) {

    _i2cPort->requestFrom(_address, (uint8_t)2);
    float registerValue;
    uint8_t MSB;
    uint8_t LSB;
    MSB = _i2cPort->read();
    LSB = _i2cPort->read();
    //DEGUB_SERIAL.print("MSB: ");
    //DEBUG_SERIAL.println(MSB);
    //DEBUG_SERIAL.print("LSB: ");
    //DEBUG_SERIAL.println(LSB);
    registerValue = MSB << 8 | LSB;
    registerValue = (float)registerValue / 100.0;
    //DEBUG_SERIAL.print("registerValue: ");
    //DEBUG_SERIAL.println(registerValue);
    return registerValue;
  } else {
    _i2cPort->requestFrom(_address, (uint8_t)1);
    float registerValue;
    registerValue = _i2cPort->read();
    return registerValue;
  }
}

void EEPROMic::updateData(uint8_t location, float incomingValue) {
  uint8_t value;
  uint16_t mphValue;
  float mphSpeed;


  //this is the MPH address so if this matches we want to multiply the value by 10
  //added 4-5-2023

  //made changes to allow for better accuracy when setting the speed into the eeprom chip
  //
  if (location == 0x13) {
    //need to change this for the added accuracey for the speed adjustment its set to 1/10 of a speed and I'm changing it to 0.5/10
    //value = incomingValue * 10;
    //converts it into an integer number so we can store it
    //ADDED 5-20-2023 THIS LINES RETURNS THE RIGHT VALUE BUT ITS A FLOAT NOT TO CONVERT TO ACTUAL INT
    //ADDED THE ROUND() 5-21-2023 IT WILL ROUND THE FLOAT VALUE TO THE CORRECT INT VALUE WITHOUT CAUSING ERRORS FOR FURTURE CODING ESPECIALLY ON THE TRANSMITTER
    //SIDE WHEN WE ADJUST THE SPEED
    mphSpeed = (float)incomingValue * 100.0;
    mphValue = round(mphSpeed);
    //DEBUG_SERIAL.print("mphSpeed: ");
    //DEBUG_SERIAL.println(mphSpeed);
    //DEBUG_SERIAL.print("mphValue: ");
    //DEBUG_SERIAL.println(mphValue);
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(location);
    _i2cPort->endTransmission();

    delay(5);

    _i2cPort->requestFrom(_address, (uint8_t)2);
    uint16_t registerValue;
    uint8_t MSB;
    uint8_t LSB;
    MSB = _i2cPort->read();
    LSB = _i2cPort->read();

    registerValue = MSB << 8 | LSB;

    //we are goning to request info from the address locations before we write to it, only change the address value if it doesnt match the incoming value
    if (registerValue != mphValue) {
      //enable the chip to where we can write to it
      digitalWrite(_writeControlPin, LOW);
      _i2cPort->beginTransmission(_address);
      _i2cPort->write(location);
      _i2cPort->write(mphValue >> 8 & 0xFF);
      _i2cPort->write(mphValue & 0xFF);
      _i2cPort->endTransmission();
    }
    digitalWrite(_writeControlPin, HIGH);
    delay(5);

  } else {
    value = incomingValue;

    _i2cPort->beginTransmission(_address);
    _i2cPort->write(location);
    _i2cPort->endTransmission();

    delay(5);

    _i2cPort->requestFrom(_address, (uint8_t)1);
    uint8_t registerValue;
    registerValue = _i2cPort->read();

    //here we are checking the incoming value with the once from the address location
    if (registerValue != value) {
      //enable the chip to where we can write to it
      digitalWrite(_writeControlPin, LOW);
      _i2cPort->beginTransmission(_address);
      _i2cPort->write(location);
      _i2cPort->write(value);
      _i2cPort->endTransmission();
    }
    digitalWrite(_writeControlPin, HIGH);
    delay(5);
  }
}
void EEPROMic::clearPageIC(uint8_t pageStart, uint8_t value) {
  delay(5);
  uint8_t pageStartAddress = 0xff;
  digitalWrite(_writeControlPin, LOW);

  switch (pageStart) {

    case 1: pageStartAddress = 0x00; break;
    case 2: pageStartAddress = 0x10; break;
    case 3: pageStartAddress = 0x20; break;
    case 4: pageStartAddress = 0x30; break;
    case 5: pageStartAddress = 0x40; break;
    case 6: pageStartAddress = 0x50; break;
    case 7: pageStartAddress = 0x60; break;
    case 8: pageStartAddress = 0x70; break;
    case 9: pageStartAddress = 0x80; break;
    case 10: pageStartAddress = 0x90; break;
    case 11: pageStartAddress = 0xA0; break;
    case 12: pageStartAddress = 0xB0; break;
    case 13: pageStartAddress = 0xC0; break;
    case 14: pageStartAddress = 0xD0; break;
    case 15: pageStartAddress = 0xE0; break;
    case 16: pageStartAddress = 0xF0; break;
  }
  _i2cPort->beginTransmission(_address);
  _i2cPort->write(pageStartAddress);
  for (int i = 0; i < 16; i++) {
    //if (clearPage == 1) {
    _i2cPort->write(value);
    //DEBUG_SERIAL.println("clearing chip");
  }
  _i2cPort->endTransmission();
}

void EEPROMic::clearIC(uint8_t value) {
  for (uint8_t pageCounter = 1; pageCounter < 17; pageCounter++) {
    delay(5);
    for (uint8_t byteCounter = 0; byteCounter < 16; byteCounter++) {
      clearPageIC(pageCounter, value);
    }
  }
}

void EEPROMic::readIC() {
  for (_currentPage = _previousPage; _currentPage < _maxPageLoad; _currentPage++) {

    _page++;
    //DEBUG_SERIAL.print("page number: ");
    //DEBUG_SERIAL.println(eeprom.page);
    //DEBUG_SERIAL.println(eeprom.currentPage);
    for (uint8_t i = 0; i < 16; i++) {
      _incomingBuffer[i + (16 * _currentPage)] = readData(i + (16 * _currentPage));
      //      DEBUG_SERIAL.print("address location: ");
      //      DEBUG_SERIAL.print(i + (16 * _currentPage), HEX);
      //      DEBUG_SERIAL.print(" data read: ");
      //      DEBUG_SERIAL.println(_incomingBuffer[i + (16 * _currentPage)]);
    }
    if (_page > 15) {
      break;
    }
  }
}