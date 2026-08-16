#include <Arduino.h>
#include <Wire.h>
#include <unity.h>
#include <24c02.h>

#define writeControlPin  4
#define  I2Caddress 0x51


EEPROMic eepromDataStorage(I2Caddress, writeControlPin);

// Implement the C functions using C++ hardware objects
extern "C" {
    void unityOutputStart(void) {
        SerialUSB.begin(115200);
        while (!SerialUSB) {
            delay(10); // Wait for the SAMD21 native USB port to connect to the PC
        }
        delay(1000); // Quick stabilization pause
    }

    void unityOutputChar(char c) {
        SerialUSB.write(c);
    }

    void unityOutputFlush(void) {
        SerialUSB.flush();
    }

    void unityOutputComplete(void) {
        SerialUSB.end();
    }
}

void setUp(void) {}
void tearDown(void) {}

// test initial connection
void test_intial_connection(void){
    uint8_t success = 0xFF;
    success = eepromDataStorage.begin();
    char buf[10] = {0};
    itoa(success, buf, 10);

    TEST_MESSAGE(buf);

    TEST_ASSERT_EQUAL_INT_MESSAGE(0,success, "Sensor: No ACK for the I2C");
}

// Need to test register write/read
void test_write_read(void) {

    TEST_ASSERT_EQUAL_INT(1, 1);
}

// Test to check clearIC
void test_clearIC(void) {
    
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, eepromDataStorage.getEepromStatus(), "Error clearing IC");
}

void setup() {
    Wire.begin();
    Wire.setTimeout(25000);
    delay(1000);

    // Unity's macros will call unityOutputStart() right here behind the scenes
    UNITY_BEGIN();
    RUN_TEST(test_intial_connection);
    RUN_TEST(test_clearIC);
    UNITY_END();
}

void loop() {
    // Keep empty
}
