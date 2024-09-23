#define SERIAL2_RX_PIN 16 // Define RX pin for Serial2
#define SERIAL2_TX_PIN 17 // Define TX pin for Serial2

const int REGISTERS_SIZE = 16;
uint16_t holdingRegisters[REGISTERS_SIZE]; // 16 Holding Registers
uint16_t inputRegisters[REGISTERS_SIZE];   // 16 Input Registers

void setup()
{
    // Initialize Serial2 for Modbus RTU communication
    Serial2.begin(9600, SERIAL_8N1, SERIAL2_RX_PIN, SERIAL2_TX_PIN);

    // Initialize holding and input registers with example values
    for (int i = 0; i < REGISTERS_SIZE; i++)
    {
        holdingRegisters[i] = i * 10;
        inputRegisters[i] = i * 5;
    }
}

void loop()
{
    if (Serial2.available())
    {
        uint8_t request[8];
        int len = Serial2.readBytes(request, 8); // Read 8 bytes (Modbus RTU request length)

        if (len == 8 && checkCRC(request, len))
        {
            uint8_t slaveAddress = request[0];
            uint8_t functionCode = request[1];

            if (slaveAddress == 1)
            { // Assuming Slave ID 1
                switch (functionCode)
                {
                case 0x03: // Read Holding Registers
                    handleReadHoldingRegisters(request);
                    break;
                case 0x04: // Read Input Registers
                    handleReadInputRegisters(request);
                    break;
                case 0x06: // Write Single Holding Register
                    handleWriteSingleHoldingRegister(request);
                    break;
                }
            }
        }
    }
}

// Function to handle Modbus function code 0x03 (Read Holding Registers)
void handleReadHoldingRegisters(uint8_t *request)
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
            response[3 + 2 * i] = highByte(holdingRegisters[startAddress + i]);
            response[4 + 2 * i] = lowByte(holdingRegisters[startAddress + i]);
        }

        uint16_t crc = calculateCRC(response, 3 + 2 * quantity);
        response[3 + 2 * quantity] = lowByte(crc);
        response[4 + 2 * quantity] = highByte(crc);

        Serial2.write(response, 5 + 2 * quantity);
    }
}

// Function to handle Modbus function code 0x04 (Read Input Registers)
void handleReadInputRegisters(uint8_t *request)
{
    uint16_t startAddress = (request[2] << 8) | request[3];
    uint16_t quantity = (request[4] << 8) | request[5];

    if (startAddress + quantity <= REGISTERS_SIZE)
    {
        uint8_t response[5 + 2 * quantity];
        response[0] = request[0];   // Slave Address
        response[1] = 0x04;         // Function Code
        response[2] = 2 * quantity; // Byte Count

        for (uint16_t i = 0; i < quantity; i++)
        {
            response[3 + 2 * i] = highByte(inputRegisters[startAddress + i]);
            response[4 + 2 * i] = lowByte(inputRegisters[startAddress + i]);
        }

        uint16_t crc = calculateCRC(response, 3 + 2 * quantity);
        response[3 + 2 * quantity] = lowByte(crc);
        response[4 + 2 * quantity] = highByte(crc);

        Serial2.write(response, 5 + 2 * quantity);
    }
}

// Function to handle Modbus function code 0x06 (Write Single Holding Register)
void handleWriteSingleHoldingRegister(uint8_t *request)
{
    uint16_t address = (request[2] << 8) | request[3];
    uint16_t value = (request[4] << 8) | request[5];

    if (address < REGISTERS_SIZE)
    {
        holdingRegisters[address] = value;

        // Echo back the request as a response (Modbus standard behavior)
        uint16_t crc = calculateCRC(request, 6);
        request[6] = lowByte(crc);
        request[7] = highByte(crc);

        Serial2.write(request, 8);
    }
}

// Function to calculate Modbus RTU CRC16
uint16_t calculateCRC(uint8_t *data, uint16_t length)
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
bool checkCRC(uint8_t *data, uint16_t length)
{
    uint16_t receivedCRC = (data[length - 1] << 8) | data[length - 2];
    uint16_t calculatedCRC = calculateCRC(data, length - 2);

    return receivedCRC == calculatedCRC;
}
