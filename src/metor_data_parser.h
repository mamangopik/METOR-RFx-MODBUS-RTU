#if !defined _metor_data_parser_h_
#define _metor_data_parser_h_

#include "global_var.h"
unsigned char find_hex_checksum(String data_string)
{
    unsigned char checksum = 0;
    int i = 0;
    for (i = 0; i < 20; i++)
    {
        checksum += (unsigned char)data_string[i];
        if (data_string[i] == '|')
            break;
    }
    return checksum;
}

float string_to_float_data(String measurement_data_string)
{
    int numeric = measurement_data_string.toInt();
    return (float)numeric / 100.0;
}

float process_data(String data)
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
        float ret_data = (float)(measurement_value_string.toInt() / 100.00);
        return ret_data;
    }
}

#endif