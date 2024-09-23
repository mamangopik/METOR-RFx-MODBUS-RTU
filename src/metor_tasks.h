#if !defined _metor_tasks_h_
#define _metor_tasks_h_
#include "global_var.h"

void metor_serial_handler(void *param)
{
    pinMode(2, OUTPUT);
    metorSerial.begin(115200, SERIAL_8N1, RX2, TX2, false, 5000); // metor serial
    String measurement_string = "";
    while (1)
    {
        byte rx_buff = metorSerial.available();
        if (rx_buff >= 16)
        {
            byte data_counter = 0;
            for (char i = 0; i < 16; i++)
            {
                char data_serial = metorSerial.read();
                if (data_serial != '\r')
                {
                    measurement_string += data_serial;
                }
                else
                {
                    digitalWrite(2, !digitalRead(2));
                    displacement_data = process_data(measurement_string);
                    measurement_string = "";
                }
            }
        }
        esp_task_wdt_reset();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

#endif