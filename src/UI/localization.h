#pragma once

#ifdef RUSSIAN
// экран загрузки
#define STR_DSP_INIT "Инициализация DSP"
#define STR_RTOS_INIT "Запуск RTOS..."
// меню канала
const char channelmenu_title[] = "Канал";
const char channelmenu_mon[] = "Отпр. на индикатор";
const char channelmenu_vfltr[] = "?AПодавление голоса";
#else
// boot screen
#define STR_DSP_INIT "Initializing DSP"
#define STR_RTOS_INIT "Configuring RTOS"
// channel menu
const char channelmenu_title[] = "Channel";
const char channelmenu_mon[] = "Send to monitor";
const char channelmenu_vfltr[] = "?AVocal filter";
#endif

#define STR_DEV_INFO "LTDA Prime v0.1.5a"

const char *const channelmenu[] = { channelmenu_title, channelmenu_mon, channelmenu_vfltr };