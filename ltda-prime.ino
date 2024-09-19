#include <Arduino.h>
#include "src/System/boot.h"

void setup()
{
    boot();
    vTaskDelete(NULL);
}

void loop()
{
    // стоковый loop не выполняется - таск удалён
    // вся логика переложена на отдельные таски во FreeRTOS
}