#pragma once

#ifdef RUSSIAN
// экран загрузки
#define STR_DSP_INIT "Инициализация DSP"
#define STR_RTOS_INIT "Запуск RTOS..."
#else
// boot screen
#define STR_DSP_INIT "Initializing DSP"
#define STR_RTOS_INIT "Configuring RTOS"
#endif

#define STR_DEV_INFO "LTDA Prime v0.0.6a"

const char testmenu_title[] = "Суперменю";
const char testmenu_1[] = "Тест 1";
const char testmenu_2[] = "ТеКст";
const char testmenu_3[] = "Я хочу пиццы";
const char testmenu_4[] = "смари это сцена";

const char *const testmenu[] = { testmenu_title, testmenu_1, testmenu_2, testmenu_3, testmenu_4 };