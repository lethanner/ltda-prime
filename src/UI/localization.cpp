#include "localization.h"

#define SET_STR_ARRAY(name, ...) \
    const char* const _##name[] = { __VA_ARGS__ }; \
    name = _##name

const Localization::Strings* Localization::active = &Localization::english;

const Localization::Strings Localization::russian = {
    .channel = "Канал",
    .send_to_monitor = "Отпр. на индикатор",
    .bassboost = "Усиление баса",
    .disconnect = "Разорвать соединение",
    .group = "Группа",
    .sends_on_fader = "Посылы на фейдерах",
    .to = "Куда?",
    .enable = "?AВключено",
    .intensity = "Интенсивность",
    .gain = "Усиление",
    .reverb = "Реверберация",
    .reverb_time = "Время затухания",
    .hf_damping = "Демпфирование ВЧ",
    .bassreverbgain = "Усиление баса",
    .parrots = " попугаев",
    .init_dsp = "Инициализация DSP",
    .init_rtos = "Запуск RTOS...",
    .select = "Выбор",
    .master = "Мастер",
    .bluetooth = "Bluetooth",
    .language = "Язык",
    .preferences = "[Установки]",
    .pitch = "Высота тона",
    .pitch_change = "Управление высотой",
    .semitones = " полутонов",
    .stereobalance = "Баланс/панорама",
    .stereomode = "Стереорежим",
    .stereo = "Стерео",
    .forcemono = "Принуд. моно",
    .vocalfilter = "Подавл. голоса"
};

const Localization::Strings Localization::english = {
    .channel = "Channel",
    .send_to_monitor = "Send to monitor",
    .bassboost = "Bassboost",
    .disconnect = "Disconnect",
    .group = "Group",
    .sends_on_fader = "Sends on fader",
    .to = "To?",
    .enable = "?AEnabled",
    .intensity = "Intensity",
    .gain = "Gain",
    .reverb = "Reverb",
    .reverb_time = "Reverb time",
    .hf_damping = "HF damping",
    .bassreverbgain = "Bass reverb gain",
    .parrots = " parrots",
    .init_dsp = "Initializing DSP",
    .init_rtos = "Configuring RTOS",
    .select = "Select",
    .master = "Master",
    .bluetooth = "Bluetooth",
    .language = "Language",
    .preferences = "[Preferences]",
    .pitch = "Pitch",
    .pitch_change = "Change pitch",
    .semitones = " semitones",
    .stereobalance = "Balance/Panning",
    .stereomode = "Stereo mode",
    .stereo = "Stereo",
    .forcemono = "Force mono",
    .vocalfilter = "Voice suppression"
};

void Localization::setLanguage(const Strings* n)
{
    active = n;

    SET_STR_ARRAY(chmenu_generic, n->channel, n->preferences, n->send_to_monitor,
                  n->stereobalance, n->stereomode);
    SET_STR_ARRAY(chmenu_master, n->master, n->preferences, n->send_to_monitor,
                  n->stereobalance, n->stereomode, n->bassboost);
    SET_STR_ARRAY(chmenu_reverb, n->reverb, n->preferences, n->stereobalance,
                  n->reverb_time, n->hf_damping, n->bassreverbgain);
    SET_STR_ARRAY(chmenu_pitch, n->pitch, n->preferences, n->send_to_monitor,
                  n->stereobalance, n->pitch_change);
    SET_STR_ARRAY(chmenu_blue, n->bluetooth, n->preferences, n->send_to_monitor,
                  n->stereobalance, n->stereomode, n->disconnect);

    SET_STR_ARRAY(groupmenu, n->group, n->sends_on_fader);
    SET_STR_ARRAY(sofdestmenu, n->to, SOF_DESTINATION_LABELS);
    SET_STR_ARRAY(bassboostmenu, n->bassboost, n->enable, n->intensity, n->gain);
    SET_STR_ARRAY(languagemenu, n->language, "English", "Русский");
    SET_STR_ARRAY(prefsmenu, n->preferences, n->language);
    SET_STR_ARRAY(stereomodemenu, n->stereomode, n->stereo, n->forcemono, n->vocalfilter);
}