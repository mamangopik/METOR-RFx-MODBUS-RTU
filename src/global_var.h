#if !defined _global_var_h
#define _global_var_h

#include <Arduino.h>

TaskHandle_t METOR_SERIAL_TASK;
TaskHandle_t MODBUS_SERIAL_TASK;

#define CPU_0 0
#define CPU_1 1

#define PRIORITY_IDLE 0
#define PRIORITY_VERY_LOW 1
#define PRIORITY_LOW 2
#define PRIORITY_MEDIUM 3
#define PRIORITY_HIGH 4
#define PRIORITY_VERY_HIGH 5
#define PRIORITY_REALTIME 6

#define RX2 16
#define TX2 17
#define RX1 18
#define TX1 19

unsigned char find_hex_checksum(char *data_string);
float string_to_float_data(String measurement_data_string);
void metor_serial_handler(void *param);
void modbus_serial_handler(void *param);
#endif