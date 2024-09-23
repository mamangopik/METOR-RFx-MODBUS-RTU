#if !defined _modbus_tasks_h_
#define _modbus_tasks_h_
#include "global_var.h"
#include <modbus_slave.h>
modbus_slave slave(SLAVE_ID);
void modbus_serial_handler(void *param)
{
    mbSerial.begin(115200);
    slave.init_hreg(100);
    slave.begin(&mbSerial);
    while (1)
    {
        uint32_t reg_buff;
        memcpy(&reg_buff, &displacement_data, sizeof(reg_buff));
        slave.update_hreg(0, (reg_buff >> 16));        // Hreg0 displacement H
        slave.update_hreg(0 + 1, (reg_buff & 0xffff)); // Hreg1 displacement L
        slave.tasks();
        esp_task_wdt_reset();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

#endif