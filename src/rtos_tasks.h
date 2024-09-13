#if !defined _rtos_tasks_h_
#define _rtos_tasks_h_
#include "global_var.h"

void writeFloatReg(uint16_t reg, float data)
{
    uint32_t reg_buff;
    memcpy(&reg_buff, &data, sizeof(reg_buff));
    mb.Ireg(reg, (reg_buff >> 16));
    mb.Ireg(reg + 1, (reg_buff & 0xffff));
}

void writeIntReg(uint16_t reg, int data)
{
    uint32_t reg_buff;
    memcpy(&reg_buff, &data, sizeof(reg_buff));
    mb.Ireg(reg, (reg_buff >> 16));
    mb.Ireg(reg + 1, (reg_buff & 0xffff));
}

short process_data(String data)
{
    String recv_checksum = "";
    String calculated_checksum = String(find_hex_checksum(data), HEX);
    uint16_t measurement_string_len = data.length();
    byte data_valid = 0;
    recv_checksum += data[measurement_string_len - 2];
    recv_checksum += data[measurement_string_len - 1];

    if (recv_checksum == calculated_checksum)
    {
        data_valid = 1;
    }

    if (data_valid)
    {
        String measurement_value_string = "";
        for (uint16_t j = 0; j < measurement_string_len; j++)
        {
            if (data[j] == '|')
            {
                break;
            }
            switch (data[j])
            {
            case 'H':
                NULL;
                break;
            case 'I':
                NULL;
                break;
            case 'J':
                NULL;
                break;
            case 'K':
                NULL;
                break;
            case 'L':
                NULL;
                break;
            case 'M':
                NULL;
                break;
            case 'N':
                NULL;
                break;
            case 'O':
                NULL;
                break;
            default:
                measurement_value_string += data[j];
                break;
            }
        }
        short ret_data = (short)(measurement_value_string.toInt());
        return ret_data;
    }
}

void metor_serial_handler(void *param)
{
    pinMode(2, OUTPUT);
    metorSerial.begin(115200, SERIAL_8N1, RX2, TX2, false, 5000); // metor serial
    String measurement_string = "";
    while (1)
    {
        // mbSerial.println(displacement_data);
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
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

void modbus_serial_handler(void *param)
{
    mbSerial.begin(115200);
    mb.begin(&mbSerial);
    mb.slave(SLAVE_ID);
    mb.addHreg(0, 30);
    mb.addIreg(0, 30);
    while (1)
    {
        mb.Ireg(0, displacement_data);
        mb.task();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

#if defined DEBUG_MODE
void debug(void *param)
{
    Serial.begin(115200);
    byte start_detected = 0;
    String measurement_string = "";
    uint16_t start_index = 0;
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
                    String recv_checksum = "";
                    String calculated_checksum = String(find_hex_checksum(measurement_string), HEX);
                    uint16_t measurement_string_len = measurement_string.length();
                    byte data_valid = 0;
                    float displacement_data = 0.0;
                    recv_checksum += measurement_string[measurement_string_len - 2];
                    recv_checksum += measurement_string[measurement_string_len - 1];

                    if (recv_checksum == calculated_checksum)
                    {
                        data_valid = 1;
                    }

                    // Serial.print("Data:");
                    // Serial.print(measurement_string);
                    // Serial.print("  calculated_checksum:");
                    // Serial.print(calculated_checksum);
                    // Serial.print("  recv_checksum:");
                    // Serial.print(recv_checksum);
                    // Serial.print("  validity:");
                    // Serial.print(data_valid);

                    if (data_valid)
                    {
                        String measurement_value_string = "";
                        for (uint16_t j = 0; j < measurement_string_len; j++)
                        {
                            if (measurement_string[j] == '|')
                            {
                                break;
                            }
                            switch (measurement_string[j])
                            {
                            case 'H':
                                NULL;
                                break;
                            case 'I':
                                NULL;
                                break;
                            case 'J':
                                NULL;
                                break;
                            case 'K':
                                NULL;
                                break;
                            case 'L':
                                NULL;
                                break;
                            case 'M':
                                NULL;
                                break;
                            case 'N':
                                NULL;
                                break;
                            case 'O':
                                NULL;
                                break;
                            default:
                                measurement_value_string += measurement_string[j];
                                break;
                            }
                        }
                        displacement_data = (measurement_value_string.toInt()) / 100.0;
                        Serial.print(displacement_data);
                    }

                    data_counter++;
                    measurement_string = "";
                    Serial.println();
                }
            }
        }
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}
#endif

#endif