#include "modbus_slave.h"
#define REGISTERS_SIZE 16
modbus_slave::modbus_slave(uint8_t id)
{
    _device_id = id;
}
void modbus_slave::begin(Stream *hwserial)
{
    this->_modbus_serial = hwserial;
}
void modbus_slave::tasks()
{
    if (this->_modbus_serial->available())
    {
        uint8_t request[8];
        int len = this->_modbus_serial->readBytes(request, 8); // Read 8 bytes (Modbus RTU request length)

        if (len == 8 && this->checkCRC(request, len))
        {
            uint8_t slaveAddress = request[0];
            uint8_t functionCode = request[1];

            if (slaveAddress == 1)
            { // Assuming Slave ID 1
                switch (functionCode)
                {
                case 0x03: // Read Holding Registers
                    this->handleReadHoldingRegisters(request);
                    break;
                case 0x06: // Write Single Holding Register
                    this->handleWriteSingleHoldingRegister(request);
                    break;
                }
            }
        }
    }
}
byte modbus_slave::init_hreg(byte reg_len)
{
    for (byte i = 0; i < reg_len; i++)
    {
        this->_holdingRegisters[i] = 0;
    }
}
byte modbus_slave::update_hreg(byte addr, uint16_t value)
{
    this->_holdingRegisters[addr] = value;
}
// Function to handle Modbus function code 0x03 (Read Holding Registers)
void modbus_slave::handleReadHoldingRegisters(uint8_t *request)
{
    uint16_t startAddress = (request[2] << 8) | request[3];
    uint16_t quantity = (request[4] << 8) | request[5];

    if (startAddress + quantity <= REGISTERS_SIZE)
    {
        uint8_t response[5 + 2 * quantity];
        response[0] = request[0];   // Slave Address
        response[1] = 0x03;         // Function Code
        response[2] = 2 * quantity; // Byte Count

        for (uint16_t i = 0; i < quantity; i++)
        {
            response[3 + 2 * i] = highByte(this->_holdingRegisters[startAddress + i]);
            response[4 + 2 * i] = lowByte(this->_holdingRegisters[startAddress + i]);
        }

        uint16_t crc = this->calculateCRC(response, 3 + 2 * quantity);
        response[3 + 2 * quantity] = lowByte(crc);
        response[4 + 2 * quantity] = highByte(crc);

        this->_modbus_serial->write(response, 5 + 2 * quantity);
    }
}

// Function to handle Modbus function code 0x06 (Write Single Holding Register)
void modbus_slave::handleWriteSingleHoldingRegister(uint8_t *request)
{
    uint16_t address = (request[2] << 8) | request[3];
    uint16_t value = (request[4] << 8) | request[5];

    if (address < REGISTERS_SIZE)
    {
        this->_holdingRegisters[address] = value;

        // Echo back the request as a response (Modbus standard behavior)
        uint16_t crc = this->calculateCRC(request, 6);
        request[6] = lowByte(crc);
        request[7] = highByte(crc);

        this->_modbus_serial->write(request, 8);
    }
}

// Function to calculate Modbus RTU CRC16
uint16_t modbus_slave::calculateCRC(uint8_t *data, uint16_t length)
{
    uint16_t crc = 0xFFFF;

    for (uint16_t i = 0; i < length; i++)
    {
        crc ^= data[i];

        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc;
}

// Function to check if the received Modbus RTU request has a valid CRC
bool modbus_slave::checkCRC(uint8_t *data, uint16_t length)
{
    uint16_t receivedCRC = (data[length - 1] << 8) | data[length - 2];
    uint16_t calculatedCRC = this->calculateCRC(data, length - 2);

    return receivedCRC == calculatedCRC;
}