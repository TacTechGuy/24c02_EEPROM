#ifndef _24c02_h_
#define _24c02_h_

#include <Arduino.h>
#include <Wire.h>

class EEPROMic {

  public:
    //this sets the address for the target board
    EEPROMic(uint8_t address, uint8_t writeControlPin);

    //this starts the communication and establishes the target board on the I2C bus
    bool begin(TwoWire &wirePort = Wire);  // begin function

    float readData(uint8_t location);

    void updateData(uint8_t location, float incomingValue);

    void clearIC(uint8_t value);

    void clearPageIC(uint8_t pageStart, uint8_t value);
    
    uint16_t readIC();




  private:
    uint8_t _page = 0;
    uint8_t _currentPage;
    uint8_t _previousPage = 0;
    uint8_t _maxPageLoad = 16;
    uint16_t _incomingBuffer[255];

    uint8_t _address;
    uint8_t _writeControlPin;
    TwoWire *_i2cPort;
    uint16_t receive16bits ();

};

#endif