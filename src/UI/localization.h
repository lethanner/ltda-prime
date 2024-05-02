#pragma once

#ifdef RUSSIAN
// экран загрузки
#define STR_DSP_INIT "Инициализация DSP"
#define STR_RTOS_INIT "Запуск RTOS..."
// меню канала
const char channelmenu_title[] = "Канал";
const char channelmenu_mon[] = "Отпр. на индикатор";
const char channelmenu_vfltr[] = "?AПодавление голоса";
const char channelmenu_sbass[] = "?AУсиление баса";
// меню группы каналов
const char groupmenu_title[] = "Группа";
const char groupmenu_sof[] = "Посылы на фейдерах";
// меню выбора назначения посылов
const char sofdestmenu_title[] = "To?";
const char sofdestmenu_master[] = "Master";
const char sofdestmenu_reverb[] = "Reverb";
// главный экран
#define SELECTION "Выбор"
#else
// boot screen
#define STR_DSP_INIT "Initializing DSP"
#define STR_RTOS_INIT "Configuring RTOS"
// channel menu
const char channelmenu_title[] = "Channel";
const char channelmenu_mon[] = "Send to monitor";
const char channelmenu_vfltr[] = "?AVocal filter";
const char channelmenu_sbass[] = "?ABassboost";
// channel group menu
const char groupmenu_title[] = "Group";
const char groupmenu_sof[] = "Sends on fader";
// SOF destination menu
const char sofdestmenu_title[] = "To?";
const char sofdestmenu_master[] = "Master";
const char sofdestmenu_reverb[] = "Reverb";
// main screen
#define SELECTION "Select"
#endif

#define STR_DEV_INFO "LTDA Prime v0.3.3a"

const char *const chmenu_generic[] = { channelmenu_title, channelmenu_mon };
const char *const chmenu_master[] = { channelmenu_title, channelmenu_mon, channelmenu_sbass };
const char *const groupmenu[] = { groupmenu_title, groupmenu_sof };
const char *const sofdestmenu[] = { sofdestmenu_title, sofdestmenu_master, sofdestmenu_reverb };