#ifndef _modbus_slave_h_
#define _modbus_slave_h_
#include "Arduino.h"
class modbus_slave
{
private:
    /* data */
public:
    modbus_slave(uint8_t id);
    byte _device_id = 1;
    uint16_t _holdingRegisters[100];
    Stream *_modbus_serial;

    // functions
    uint16_t calculateCRC(uint8_t *data, uint16_t length);
    byte init_hreg(byte reg_len);
    byte update_hreg(byte addr, uint16_t value);
    bool checkCRC(uint8_t *data, uint16_t length);
    void handleReadHoldingRegisters(uint8_t *request);
    void handleWriteSingleHoldingRegister(uint8_t *request);
    void begin(Stream *hwserial);
    void tasks();
};

#endif