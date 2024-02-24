#pragma once

// я обязательно освою FreeRTOS...
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../Hardware/DSP.h"
#include "../Hardware/bluetooth.h"
#include "../UI/UI.h"
#include "../UI/localization.h"

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
	bluetooth.init();
    // TODO: DSP init

    /* Инициализация FreeRTOS */
    UI.printStatus(STR_RTOS_INIT, 56);
    xTaskCreateUniversal(&task_ctrlProcess, "ctrl_proc", 3072, NULL, 1, NULL, 0);
    delay(200);
    xTaskCreateUniversal(&task_uiRefresh, "ui_refresh", 3072, NULL, 2, NULL, 1);

    /* Выход в рабочий режим и отрисовка интерфейса на экране */
    UI.reload();
}