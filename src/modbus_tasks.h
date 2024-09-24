#if !defined _modbus_tasks_h_
#define _modbus_tasks_h_
#include "global_var.h"
void modbus_serial_handler(void *param)
{
    mbSerial.begin(115200);
    mbSerial.begin(115200);
    mb.begin(&mbSerial);
    mb.slave(SLAVE_ID);
    mb.addIreg(0, 0, 30);
    while (1)
    {
        for (byte i = 0; i < 8; i++)
        {
            uint32_t buffer;
            memcpy(&buffer, &displacement_data[i], sizeof(buffer)); // typeCast to unsigned 32bit integer (4-byte).
            uint16_t Hval = buffer >> 16;                           // High Word 16-bit
            uint16_t Lval = buffer & 0xFFFF;                        // Low Word 16-bit
            mb.Ireg(i * 2, Hval);
            mb.Ireg(i * 2 + 1, Lval);
        }
        mb.task();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

#endif