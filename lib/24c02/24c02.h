#ifndef _24c02_h_
#define _24c02_h_

#include <Arduino.h>
#include <Wire.h>

#define ZERO 0



class EEPROMic {

  private:
    // Convert the void functions into returning an error code
    // I'll add as I go
    enum class Status: uint8_t{
      SUCCESS = 0,
      CONNECTION_ERROR = 1,
      INVALID_LOCATION = 2,
      OUT_OF_BOUNDS = 3,
      CLEARING = 4,
      STANDBY = 0xFF,
    };
    Status eepromStatus = Status::STANDBY;    // Initalization
    Status eepromError = Status::STANDBY;     // Return Error Code  

    enum class State: uint8_t{
      IDLE = 0,
      TIMER_INIT = 1,
      TIMER_ACTIVE = 2,
      STANDBY = 0xFF
    };
    State stateController = State::STANDBY;

    enum class Command: uint8_t {
      CLEAR_IC,
      WRITE_IC,
      READ_IC,
      STANDBY
    };
    Command currentCommand = Command::STANDBY;

    struct myStorage {
    uint8_t incomingData[256] = {0};
    uint8_t currentPage = 0;
    uint8_t previousPage = 0;
    const uint8_t MAX_PAGE = 16;
    uint8_t page = 0;
    bool read = false;
  };
  myStorage _eeprom;
  
    // Used to handle Non-blocking timer
    unsigned long _previousTime = 0;
    uint8_t _interval = 10;
    bool _timeReset = false;

    uint8_t _address;
    uint8_t _writeControlPin;
    TwoWire *_i2cPort = nullptr;
    
    uint16_t receive16bits ();
   

  public:
    //this sets the address for the target board
    EEPROMic(uint8_t address, uint8_t writeControlPin);

    //this starts the communication and establishes the target board on the I2C bus
    uint8_t begin(TwoWire &wirePort = Wire);  // begin function

    uint8_t readData(uint8_t location);

    void updateData(uint8_t location, float incomingValue);

    void clearIC(uint8_t value= ZERO);

    uint8_t clearPageIC(uint8_t pageStart, uint8_t value);

    Status getEepromStatus();
    
    void readIC(void);
    
    void stateMachine();
};

#endif