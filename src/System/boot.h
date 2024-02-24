#pragma once

// я обязательно освою FreeRTOS...
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <BluetoothA2DPSink.h>
#include "../Hardware/DSP.h"
#include "../UI/UI.h"
#include "../UI/localization.h"

BluetoothA2DPSink bluetooth;

void task_uiRefresh(void *pvParameters)
{
    TickType_t xLastWakeTime;
    TickType_t refreshInterval = 35 / portTICK_PERIOD_MS;

    for (;;) {
        xLastWakeTime = xTaskGetTickCount();

        // всё, что нужно периодически запрашивать у DSP
        DSP.retrieveRTAValues();
        // обновить данные в пользовательском интерфейсе
        UI.refresh();

        vTaskDelayUntil(&xLastWakeTime, refreshInterval);
    }
}

void task_ctrlProcess(void *pvParameters)
{
    for (;;) {
        UI.processCtrl();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

void boot()
{
    /* Инициализация юзер-интерфейса */
    UI.prepare();
    UI.printStatus(STR_DEV_INFO, 56);
    delay(500);

    /* Инициализация периферии микроконтроллера */
    Wire.setClock(400000L);
    i2s_pin_config_t my_pin_config = {
        .bck_io_num = 26,
        .ws_io_num = 25,
        .data_out_num = 23,
        .data_in_num = I2S_PIN_NO_CHANGE
    };
    bluetooth.set_pin_config(my_pin_config);
    bluetooth.start("LTDA Prime");

    // TODO: DSP init

    /* Инициализация FreeRTOS */
    UI.printStatus(STR_RTOS_INIT, 56);
    xTaskCreateUniversal(&task_ctrlProcess, "ctrl_proc", 3072, NULL, 1, NULL, 0);
    delay(200);
    xTaskCreateUniversal(&task_uiRefresh, "ui_refresh", 3072, NULL, 2, NULL, 1);

    /* Выход в рабочий режим и отрисовка интерфейса на экране */
    UI.reload();
}