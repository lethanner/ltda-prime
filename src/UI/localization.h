#pragma once

#ifdef RUSSIAN
// экран загрузки
#define STR_DSP_INIT "Инициализация DSP"
#define STR_RTOS_INIT "Запуск RTOS..."
// меню канала
const char channelmenu_title[] = "Канал";
const char channelmenu_mon[] = "Отпр. на индикатор";
const char channelmenu_vfltr[] = "?AПодавление голоса";
// меню группы каналов
const char groupmenu_title[] = "Группа";
const char groupmenu_sof[] = "Посылы на фейдерах";
// меню выбора назначения посылов
const char sofdestmenu_title[] = "To?";
const char sofdestmenu_master[] = "Master";
const char sofdestmenu_reverb[] = "Reverb";
#else
// boot screen
#define STR_DSP_INIT "Initializing DSP"
#define STR_RTOS_INIT "Configuring RTOS"
// channel menu
const char channelmenu_title[] = "Channel";
const char channelmenu_mon[] = "Send to monitor";
const char channelmenu_vfltr[] = "?AVocal filter";
// channel group menu
const char groupmenu_title[] = "Group";
const char groupmenu_sof[] = "Sends on fader";
// SOF destination menu
const char sofdestmenu_title[] = "To?";
const char sofdestmenu_master[] = "Master";
const char sofdestmenu_reverb[] = "Reverb";
#endif

#define STR_DEV_INFO "LTDA Prime v0.2.2a"

const char *const channelmenu[] = { channelmenu_title, channelmenu_mon, channelmenu_vfltr };
const char *const groupmenu[] = { groupmenu_title, groupmenu_sof };
const char *const sofdestmenu[] = { sofdestmenu_title, sofdestmenu_master, sofdestmenu_reverb };