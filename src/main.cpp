#include "global_var.h"
void setup()
{
  esp_task_wdt_init(0xffffffff, false);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  xTaskCreatePinnedToCore(metor_serial_handler, "metor reader", 4096, NULL, PRIORITY_REALTIME, &METOR_SERIAL_TASK, CPU_0);
  // vTaskDelay(1000 / portTICK_PERIOD_MS);
  xTaskCreatePinnedToCore(modbus_serial_handler, "Modbus RTU slave", 4096, NULL, PRIORITY_REALTIME, &MODBUS_SERIAL_TASK, CPU_1);
  // vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void loop()
{
}