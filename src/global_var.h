#if !defined _global_var_h
#define _global_var_h

#include <Arduino.h>
#include <esp_task_wdt.h>
#include <ModbusRTU.h>

ModbusRTU mb;

TaskHandle_t METOR_SERIAL_TASK;
TaskHandle_t MODBUS_SERIAL_TASK;

#define CPU_0 0
#define CPU_1 1

#define REGN 10
#define SLAVE_ID 1
#define mbSerial Serial
#define metorSerial Serial2

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

int displacement_data = 0;

unsigned char find_hex_checksum(String data_string);
float string_to_float_data(String measurement_data_string);
void metor_serial_handler(void *param);
void modbus_serial_handler(void *param);
void writeFloatReg(uint16_t reg, float data);
void writeIntReg(uint16_t reg, int data);
float process_data(String data);

#include "metor_tasks.h"
#include "modbus_tasks.h"
#include "metor_data_parser.h"
#endif