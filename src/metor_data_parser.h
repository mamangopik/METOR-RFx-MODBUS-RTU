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

void process_data(String data)
{
    String measurement_value_string1 = "";
    String measurement_value_string2 = "";
    String measurement_value_string3 = "";
    String measurement_value_string4 = "";
    String measurement_value_string5 = "";
    String measurement_value_string6 = "";
    String measurement_value_string7 = "";
    String measurement_value_string8 = "";
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
        for (uint16_t j = 0; j < measurement_string_len; j++)
        {
            if (data[j] == '|')
            {
                break;
            }
            switch (data[j])
            {
            case 'H':
                if (j == 0)
                    j++;
                measurement_value_string1 += data[j];
                break;
            case 'I':
                if (j == 0)
                    j++;
                measurement_value_string2 += data[j];
                break;
            case 'J':
                if (j == 0)
                    j++;
                measurement_value_string3 += data[j];
                break;
            case 'K':
                if (j == 0)
                    j++;
                measurement_value_string4 += data[j];
                break;
            case 'L':
                if (j == 0)
                    j++;
                measurement_value_string5 += data[j];
                break;
            case 'M':
                if (j == 0)
                    j++;
                measurement_value_string6 += data[j];
                break;
            case 'N':
                if (j == 0)
                    j++;
                measurement_value_string7 += data[j];
                break;
            case 'O':
                if (j == 0)
                    j++;
                measurement_value_string8 += data[j];
                break;
            default:
                if (j == 0)
                    j++;
                measurement_value_string1 += data[j];
                break;
            }
        }

        if (measurement_value_string1.length() > 0)
            displacement_data[0] = (float)(measurement_value_string1.toInt() / 100.00);
        if (measurement_value_string2.length() > 0)
            displacement_data[1] = (float)(measurement_value_string2.toInt() / 100.00);
        if (measurement_value_string3.length() > 0)
            displacement_data[2] = (float)(measurement_value_string3.toInt() / 100.00);
        if (measurement_value_string4.length() > 0)
            displacement_data[3] = (float)(measurement_value_string4.toInt() / 100.00);
        if (measurement_value_string5.length() > 0)
            displacement_data[4] = (float)(measurement_value_string5.toInt() / 100.00);
        if (measurement_value_string6.length() > 0)
            displacement_data[5] = (float)(measurement_value_string6.toInt() / 100.00);
        if (measurement_value_string7.length() > 0)
            displacement_data[6] = (float)(measurement_value_string7.toInt() / 100.00);
        if (measurement_value_string8.length() > 0)
            displacement_data[7] = (float)(measurement_value_string8.toInt() / 100.00);
    }
}

#endif