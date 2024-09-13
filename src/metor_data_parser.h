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

#endif