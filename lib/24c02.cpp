#include "24c02.h"

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

    _i2cPort->requestFrom(_address, 2);
    float registerValue;
    uint16_t bits;
    uint8_t MSB;
    uint8_t LSB;
    MSB = _i2cPort->read();
    LSB = _i2cPort->read();
    //SerialUSB.print("MSB: ");
    //SerialUSB.println(MSB);
    //SerialUSB.print("LSB: ");
    //SerialUSB.println(LSB);
    registerValue = MSB << 8 | LSB;
    registerValue = (float)registerValue / 100.0;
    //SerialUSB.print("registerValue: ");
    //SerialUSB.println(registerValue);
    return registerValue;
  } else {
    _i2cPort->requestFrom(_address, 1);
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
    //SerialUSB.print("mphSpeed: ");
    //SerialUSB.println(mphSpeed);
    //SerialUSB.print("mphValue: ");
    //SerialUSB.println(mphValue);
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(location);
    _i2cPort->endTransmission();

    delay(5);

    _i2cPort->requestFrom(_address, 2);
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

    _i2cPort->requestFrom(_address, 1);
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
  uint8_t pageStartAddress;
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
    //Serial.println("clearing chip");
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

//uint8_t EEPROMic::readIC() {
//
//
//
//  delay(5);
//
//  for (_currentPage = _previousPage; _currentPage < _maxPageLoad; _currentPage++) {
//    _page++;
//    //SerialUSB.println(_page);
//
//    if (_page == 1) {
//      _i2cPort->beginTransmission(_address);
//      _i2cPort->write(0x00);
//      _i2cPort->endTransmission();
//    }
//
//    if (_page == 2) {
//      _i2cPort->beginTransmission(_address);
//      _i2cPort->write(0x10);
//      _i2cPort->endTransmission();
//    }
//
//    if (_page == 3) {
//      _i2cPort->beginTransmission(_address);
//      _i2cPort->write(0x20);
//      _i2cPort->endTransmission();
//    }
//
//    if (_page == 4) {
//      _i2cPort->beginTransmission(_address);
//      _i2cPort->write(0x30);
//      _i2cPort->endTransmission();
//    }
//    if (_page == 5) {
//      _i2cPort->beginTransmission(_address);
//      _i2cPort->write(0x40);
//      _i2cPort->endTransmission();
//    }
//    if (_page == 6) {
//      _i2cPort->beginTransmission(_address);
//      _i2cPort->write(0x50);
//      _i2cPort->endTransmission();
//    }
//    if (_page == 7) {
//      _i2cPort->beginTransmission(_address);
//      _i2cPort->write(0x60);
//      _i2cPort->endTransmission();
//    }
//    if (_page == 8) {
//      _i2cPort->beginTransmission(_address);
//      _i2cPort->write(0x70);
//      _i2cPort->endTransmission();
//    }
//    if (_page == 9) {
//      _i2cPort->beginTransmission(_address);
//      _i2cPort->write(0x80);
//      _i2cPort->endTransmission();
//    }
//    if (_page == 10) {
//      _i2cPort->beginTransmission(_address);
//      _i2cPort->write(0x90);
//      _i2cPort->endTransmission();
//    }
//    if (_page == 11) {
//      _i2cPort->beginTransmission(_address);
//      _i2cPort->write(0xA0);
//      _i2cPort->endTransmission();
//    }
//    if (_page == 12) {
//      _i2cPort->beginTransmission(_address);
//      _i2cPort->write(0xB0);
//      _i2cPort->endTransmission();
//    }
//    if (_page == 13) {
//      _i2cPort->beginTransmission(_address);
//      _i2cPort->write(0xC0);
//      _i2cPort->endTransmission();
//    }
//    if (_page == 14) {
//      _i2cPort->beginTransmission(_address);
//      _i2cPort->write(0xD0);
//      _i2cPort->endTransmission();
//    }
//    if (_page == 15) {
//      _i2cPort->beginTransmission(_address);
//      _i2cPort->write(0xE0);
//      _i2cPort->endTransmission();
//    }
//    if (_page == 16) {
//      _i2cPort->beginTransmission(_address);
//      _i2cPort->write(0xF0);
//      _i2cPort->endTransmission();
//    }
//
//    _i2cPort->requestFrom(_address, 16);
//
//    for (int i = 0; i < 16; i++) {
//      if (_page == 1) {
//        _incomingBuffer[i] = _i2cPort->read();
//        //        SerialUSB.print("address location: ");
//        //        SerialUSB.print(i, HEX);
//        //        SerialUSB.print("\t");
//        //        SerialUSB.print("data read:");
//        //        SerialUSB.println(_incomingBuffer[i]);
//        return _incomingBuffer[i];
//
//
//      }
//      if (_page == 2) {
//        _incomingBuffer[i + 16] = _i2cPort->read();
//        //        SerialUSB.print("address location: ");
//        //        SerialUSB.print(i + 16, HEX);
//        //        SerialUSB.print("\t");
//        //        SerialUSB.print("data read:");
//        //        SerialUSB.println(_incomingBuffer[16 + i]);
//        return _incomingBuffer[i + 16];
//      }
//      if (_page == 3) {
//        _incomingBuffer[i + 32] = _i2cPort->read();
//        //        SerialUSB.print("address location: ");
//        //        SerialUSB.print(i + 32, HEX);
//        //        SerialUSB.print("\t");
//        //        SerialUSB.print("data read:");
//        //        SerialUSB.println(_incomingBuffer[32 + i]);
//        return _incomingBuffer[i + 32];
//      }
//      if (_page == 4) {
//        _incomingBuffer[i + 48] = _i2cPort->read();
//        //        SerialUSB.print("address location: ");
//        //        SerialUSB.print(i + 48, HEX);
//        //        SerialUSB.print("\t");
//        //        SerialUSB.print("data read:");
//        //        SerialUSB.println(_incomingBuffer[48 + i]);
//        return _incomingBuffer[i + 48];
//      }
//      if (_page == 5) {
//        _incomingBuffer[i + 64] = _i2cPort->read();
//        //        SerialUSB.print("address location: ");
//        //        SerialUSB.print(i + 64, HEX);
//        //        SerialUSB.print("\t");
//        //        SerialUSB.print("data read:");
//        //        SerialUSB.println(_incomingBuffer[64 + i]);
//        return _incomingBuffer[i + 64];
//      }
//      if (_page == 6) {
//        _incomingBuffer[i + 80] = _i2cPort->read();
//        //        SerialUSB.print("address location: ");
//        //        SerialUSB.print(i + 80, HEX);
//        //        SerialUSB.print("\t");
//        //        SerialUSB.print("data read:");
//        //        SerialUSB.println(_incomingBuffer[80 + i]);
//        return _incomingBuffer[i + 80];
//      }
//      if (_page == 7) {
//        _incomingBuffer[i + 96] = _i2cPort->read();
//        //        SerialUSB.print("address location: ");
//        //        SerialUSB.print(i + 96, HEX);
//        //        SerialUSB.print("\t");
//        //        SerialUSB.print("data read:");
//        //        SerialUSB.println(_incomingBuffer[96 + i]);
//        return _incomingBuffer[i + 96];
//      }
//      if (_page == 8) {
//        _incomingBuffer[i + 112 ] = _i2cPort->read();
//        //        SerialUSB.print("address location: ");
//        //        SerialUSB.print(i + 112, HEX);
//        //        SerialUSB.print("\t");
//        //        SerialUSB.print("data read:");
//        //        SerialUSB.println(_incomingBuffer[112 + i]);
//        return _incomingBuffer[i + 112];
//      }
//      if (_page == 9) {
//        _incomingBuffer[i + 128] = _i2cPort->read();
//        //        SerialUSB.print("address location: ");
//        //        SerialUSB.print(i + 128, HEX);
//        //        SerialUSB.print("\t");
//        //        SerialUSB.print("data read:");
//        //        SerialUSB.println(_incomingBuffer[128 + i]);
//        return _incomingBuffer[i + 128];
//      }
//      if (_page == 10) {
//        _incomingBuffer[i + 144] = _i2cPort->read();
//        //        SerialUSB.print("address location: ");
//        //        SerialUSB.print(i + 144, HEX);
//        //        SerialUSB.print("\t");
//        //        SerialUSB.print("data read:");
//        //        SerialUSB.println(_incomingBuffer[144 + i]);
//        return _incomingBuffer[i + 144];
//      }
//      if (_page == 11) {
//        _incomingBuffer[i + 160] = _i2cPort->read();
//        //        SerialUSB.print("address location: ");
//        //        SerialUSB.print(i + 160, HEX);
//        //        SerialUSB.print("\t");
//        //        SerialUSB.print("data read:");
//        //        SerialUSB.println(_incomingBuffer[160 + i]);
//        return _incomingBuffer[i + 160];
//      }
//      if (_page == 12) {
//        _incomingBuffer[i + 176] = _i2cPort->read();
//        //        SerialUSB.print("address location: ");
//        //        SerialUSB.print(i + 176, HEX);
//        //        SerialUSB.print("\t");
//        //        SerialUSB.print("data read:");
//        //        SerialUSB.println(_incomingBuffer[176 + i]);
//        return _incomingBuffer[i + 176];
//      }
//      if (_page == 13) {
//        _incomingBuffer[i + 192] = _i2cPort->read();
//        //        SerialUSB.print("address location: ");
//        //        SerialUSB.print(i + 192, HEX);
//        //        SerialUSB.print("\t");
//        //        SerialUSB.print("data read:");
//        //        SerialUSB.println(_incomingBuffer[192 + i]);
//        return _incomingBuffer[i + 192];
//      }
//      if (_page == 14) {
//        _incomingBuffer[i + 208] = _i2cPort->read();
//        //        SerialUSB.print("address location: ");
//        //        SerialUSB.print(i + 208, HEX);
//        //        SerialUSB.print("\t");
//        //        SerialUSB.print("data read:");
//        //        SerialUSB.println(_incomingBuffer[208 + i]);
//        return _incomingBuffer[i + 208];
//      }
//      if (_page == 15) {
//        _incomingBuffer[i + 224] = _i2cPort->read();
//        //        SerialUSB.print("address location: ");
//        //        SerialUSB.print(i + 224, HEX);
//        //        SerialUSB.print("\t");
//        //        SerialUSB.print("data read:");
//        //        SerialUSB.println(_incomingBuffer[224 + i]);
//        return _incomingBuffer[i + 224];
//      }
//      if (_page == 16) {
//        _incomingBuffer[i + 240] = _i2cPort->read();
//        //        SerialUSB.print("address location: ");
//        //        SerialUSB.print(i + 240, HEX);
//        //        SerialUSB.print("\t");
//        //        SerialUSB.print("data read:");
//        //        SerialUSB.println(_incomingBuffer[240 + i]);
//        return _incomingBuffer[i + 240];
//      }
//      //      byteCounter++;
//      //      SerialUSB.print("byte counter: ");
//      //      SerialUSB.println(byteCounter);
//    }
//    _i2cPort->endTransmission();
//
//  }
//}
uint16_t EEPROMic::readIC() {
  for (_currentPage = _previousPage; _currentPage < _maxPageLoad; _currentPage++) {

    _page++;
    //SerialUSB.print("page number: ");
    //SerialUSB.println(eeprom.page);
    //SerialUSB.println(eeprom.currentPage);
    for (uint8_t i = 0; i < 16; i++) {
      _incomingBuffer[i + (16 * _currentPage)] = readData(i + (16 * _currentPage));
      //      SerialUSB.print("address location: ");
      //      SerialUSB.print(i + (16 * _currentPage), HEX);
      //      SerialUSB.print(" data read: ");
      //      SerialUSB.println(_incomingBuffer[i + (16 * _currentPage)]);
    }
    if (_page > 15) {
      break;
    }
  }
}