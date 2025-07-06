#pragma once

// я обязательно освою FreeRTOS...
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../Hardware/DSP.h"
#include "../Hardware/bluetooth.h"
#include "../Hardware/shiftreg.h"
#include "../Hardware/ds18b20.h"
#include "../UI/UI.h"
#include "../UI/localization.h"
#include "memory.h"

void task_dataRefresh(void *pvParameters)
{
    TickType_t xLastWakeTime;
    TickType_t refreshInterval = 35 / portTICK_PERIOD_MS;
    uint8_t heatsinkCounter = 30;

    for (;;) {
        xLastWakeTime = xTaskGetTickCount();

        // всё, что нужно периодически запрашивать у DSP
        DSP.retrieveRTAValues();

        // раз в 30 итераций цикла (примерно раз в секунду)
        // выполняем опрос датчика температуры
        // можно было создать отдельную таску FreeRTOS,
        // но зачем ею расходовать ресурсы?
        if (++heatsinkCounter > 29) {
            heatsinkCounter = 0;
            heatsink.readTemp();

            // подстроить обороты кулера под температуру
            int16_t temp = heatsink.getTempInt();
            uint8_t fan_duty = map(constrain(temp, 30, 70), 30, 70, 10, 255);
            ledcWrite(FAN_PWM_CHANNEL, fan_duty);

            // запросить данные к следующему разу
            heatsink.requestTemp();
        }

        // обновить данные в пользовательском интерфейсе
        LEDUI::render();

        vTaskDelayUntil(&xLastWakeTime, refreshInterval);
    }
}

void task_ctrlProcess(void *pvParameters)
{
    for (;;) {
        LEDUI::pollCtrl();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

void boot()
{
    /* Инициализация сдвигового регистра */
    shifters.quickInit();

    /* Инициализация ШИМ для управления кулером */
    ledcSetup(FAN_PWM_CHANNEL, FAN_PWM_FREQ, 8);
    ledcAttachPin(FAN_PWM, FAN_PWM_CHANNEL);
    // ненадолго взвоем кулером при запуске
    ledcWrite(FAN_PWM_CHANNEL, 128);

    /* Инициализация памяти настроек */
    memory.begin("ltda", false);

    /* Инициализация датчика температуры */
    heatsink.reset();
    heatsink.setResolution(10);
    // сразу запросить температуру, чтобы к окончании
    // инициализации данные лежали на столе
    heatsink.requestTemp();

    /* Загрузка языкового пакета */
    Localization::setLanguage(memory.getUInt("lang", 0));

    /* Инициализация юзер-интерфейса */
    LEDUI::init();
    LEDUI::bootStatus(STR_DEV_INFO, 56);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    // замедлим кулер
    ledcWrite(FAN_PWM_CHANNEL, 10);

    /* Инициализация периферии микроконтроллера */
    Wire.setClock(400000L);
    bluetooth.init();
    vTaskDelay(50 / portTICK_PERIOD_MS);

    /* Инициализация DSP */
    LEDUI::bootStatus(Localization::act->init_dsp, 56);
    DSP.init();

    /* Инициализация FreeRTOS */
    LEDUI::bootStatus(Localization::act->init_rtos, 56);
    xTaskCreateUniversal(&task_ctrlProcess, "ctrl_proc", 3072, NULL, 1, NULL, 0);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    xTaskCreateUniversal(&task_dataRefresh, "data_refresh", 3072, NULL, 2, NULL, 1);

    /* Выход в рабочий режим и отрисовка интерфейса на экране */
    LEDUI::reset();
}