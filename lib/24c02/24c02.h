#ifndef _24c02_h_
#define _24c02_h_

#include <Arduino.h>
#include <Wire.h>

#define ZERO 0



class EEPROMic {

  public:
    //this sets the address for the target board
    EEPROMic(uint8_t address, uint8_t writeControlPin);

    //this starts the communication and establishes the target board on the I2C bus
    uint8_t begin(TwoWire &wirePort = Wire);  // begin function

    uint8_t readData(uint8_t location);

    void updateData(uint8_t location, float incomingValue);

    void clearIC(uint8_t value= ZERO);

    void clearPageIC(uint8_t pageStart, uint8_t value);
    
    void readIC(void);

    
  private:
    // Convert the void functions into returning an error code
    // I'll add as I go
    enum class status: uint8_t{
      SUCCESS = 0,
      CONNECTION_ERROR = 1,
      INVALID_LOCATION,
      STANDBY = 0xFF,
    };
    status eepromStatus = status::STANDBY;

    struct myStorage {
    uint8_t incomingData[256];
    uint8_t currentPage;
    uint8_t previousPage = 0;
    const uint8_t MAX_PAGE = 16;
    uint8_t page = 0;
    bool read = false;
  };
  myStorage _eeprom;

    uint8_t _page = 0;
    uint8_t _currentPage = 0; 
    uint8_t _previousPage = 0;
    uint8_t _maxPageLoad = 16;
    uint16_t _incomingBuffer[255] = {0};
  
    uint8_t _address;
    uint8_t _writeControlPin;
    TwoWire *_i2cPort = nullptr;
    
    uint16_t receive16bits ();

};

#endif