#include "global_var.h"
void setup()
{
  Serial2.begin(115200, SERIAL_8N1, RX2, TX2, false, 5000); // metor serial
  Serial2.begin(115200, SERIAL_8N1, RX1, TX1, false, 5000); // modbus serial
  Serial.begin(115200);                                     // debug serial
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  xTaskCreatePinnedToCore(metor_serial_handler, "metor reader", 2048, NULL, PRIORITY_REALTIME, &METOR_SERIAL_TASK, CPU_0);
  xTaskCreatePinnedToCore(metor_serial_handler, "Modbus RTU slave", 2048, NULL, PRIORITY_REALTIME, &MODBUS_SERIAL_TASK, CPU_1);
}

void loop()
{
}