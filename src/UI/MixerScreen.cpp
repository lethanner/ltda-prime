#include "UI.h"
#include "screens.h"
#include "decibels.h"

void LEDUI::MixerScreen::init(void* params)
{
    //MixerScreen::active = this;
    gap_block = (128 - (_group->count * 18)) / (_group->count + 1);
    selected = 0;

    if (params != NULL && _group->sof > NO_SOF) {
        byte paramSoF = *static_cast<byte*>(params);
        usingSoF = true, SoFdest = paramSoF;
    } else
        usingSoF = false;
}

void LEDUI::MixerScreen::render()
{
    display.clear();
    if (usingSoF) {  // "sends on fader" вместо статусбара
        statusbarDecibels();
        printYX(ch_labels[_group->onScreenChannels[selected]], 0, 0);
        display.print(" to ");
        //display.print(sendto_labels[SOF_dest]);
    } else if (statusbar == 0) {  // непосредственно статусбар
        printYX("Mixer", 0, 0);         // заглушка заголовка
        printValue(0, "'C", -1, 0);     // заглушка датчика температуры
    } else if (statusbar == 1) {   // в момент изменения громкости канала
        statusbarDecibels();
        printYX(ch_labels[_group->onScreenChannels[selected]], 0, 0);
        display.print(":");
    }

    byte label_offset = gap_block / 2;
    for (byte ch = 0; ch < _group->count; ch++) {
        byte block_width = 18;  // TODO: разделение каналов на моно и стерео
        byte block_safe_zone = block_width + gap_block;
        byte x_coord = gap_block + (ch * block_safe_zone);
        byte levelL = 52 - DSP.getRelativeSignalLevel(db_calibr_onscreen, 42, _group->onScreenChannels[ch], false);
        byte levelR = 52 - DSP.getRelativeSignalLevel(db_calibr_onscreen, 42, _group->onScreenChannels[ch], true);
        byte fader_pos = map(usingSoF
                               ? DSP.sendFaders_dB[SoFdest][_group->onScreenChannels[ch]]
                               : DSP.faderPosition_dB[_group->onScreenChannels[ch]],
                             -97, 10, 52, 11);
        bool is_muted = usingSoF
                          ? DSP.sendMuteFlags[SoFdest][_group->onScreenChannels[ch]]
                          : DSP.muteFlags[_group->onScreenChannels[ch]];

        display.line(x_coord + 2, 52, x_coord + 2, 11);  // полоска фейдера
        if (is_muted) {
            // если MUTED, то рисуем (какую-то фигню) вместо ручки фейдера
            display.fastLineH(fader_pos + 1, x_coord + 1, x_coord + 3);
            display.fastLineH(fader_pos - 1, x_coord + 1, x_coord + 3);
            // если замьюченный канал выбран, то дорисовываем (фигню) до квадратика
            if (selected == ch) display.fastLineH(fader_pos, x_coord + 1, x_coord + 3);
        } else
            // если не MUTED, то рисуем обычную ручку фейдера
            display.rect(x_coord, fader_pos, x_coord + 4, fader_pos + static_cast<byte>(selected == ch), 2);

        display.rect(x_coord + 8, 52, x_coord + 11, levelL, OLED_FILL);   // столбик уровня левого канала
        display.rect(x_coord + 13, 52, x_coord + 16, levelR, OLED_FILL);  // столбик уровня правого канала

        // подписи каналов с выравниванием
        if (screen_state == 0) {
            byte label_len = strlen(ch_labels[_group->onScreenChannels[ch]]);
            byte max_label_chars = (block_safe_zone / 6 < label_len) ? block_safe_zone / 6 : label_len;
            display.setCursorXY(label_offset + ((block_safe_zone - (6 * max_label_chars)) / 2), 56);
            for (byte i = 0; i < max_label_chars; i++) {
                display.write(ch_labels[_group->onScreenChannels[ch]][i]);
            }
            label_offset += block_safe_zone;
        }
    }

    // селектор вместо подписей каналов (по необходимости)
    switch (screen_state) {
    case 1:  // выбор канала на дисплее
        if (selected > 0)
            printYX("<", 56, 0);
        if (selected < _group->count - 1)
            printYX(">", 56, 122);
        printYX(Localization::active()->select, 56);
        break;
    case 2:  // выбор группы каналов
        if (selectedGroup > 0)
            printYX("<<<", 56, 0);
        if (selectedGroup < GROUPS_COUNT - 1)
            printYX(">>>", 56, 110);
        printYX(_group->name, 56);
        break;
    }
}

void LEDUI::MixerScreen::onClick()
{
    if (screen_state == 1 && (turn_started || usingSoF)) {  // если уже начали листать каналы или если режим "sends on fader"
        screen_state = 0;                                  // сбрасываем действие
        turn_started = false;
    }
    // если на экране только один канал, переход сразу к переключению страниц
    else if (screen_state == 0 && _group->count == 1)
        screen_state = 2;
    else if (++screen_state > 2)  // а так просто по кругу переключаем действия
        screen_state = 0;

    statusbar = 1;
}

void LEDUI::MixerScreen::onHold()
{
    if (screen_state == 1)
        usingSoF  // удержание на выборе канала - MUTE
          ? DSP.toggleMute(_group->onScreenChannels[selected], SoFdest)
          : DSP.toggleMute(_group->onScreenChannels[selected]);
    else if (usingSoF)              // удержание на экране sends on fader
        open(&MixerScreen::it());  // - возврат
    else if (screen_state == 2) {   // удержание на выборе группы каналов - меню группы
        if (_group->sof > NO_SOF)
            open(&Menus::ChannelGroup::it());
    } else {
        // TODO: использовать массив соотношения каналов к их меню
        switch (_group->onScreenChannels[selected]) {
        case FADER_MASTER_ST:  // если выбрали канал Master, открываем меню для него
            open(&Menus::MasterChannel::it());
            break;
        case FADER_REVERB_ST:  // если выбрали канал reverb, то для него меню
            open(&Menus::ReverbChannel::it());
            break;
        case FADER_BLUETOOTH_ST:  // если выбрали канал bluetooth, то для него меню
            open(&Menus::BluetoothChannel::it());
            break;
        default:  // иначе меню для всех остальных
            open(&Menus::GenericChannel::it());
            break;
        }
    }
}

void LEDUI::MixerScreen::onTurn(int8_t dir)
{
    switch (screen_state) {
    case 0: {  // управление громкостью канала
        if (usingSoF) {
            DSP.setDecibelSendLevel(
              _group->onScreenChannels[selected], SoFdest,
              DSP.sendFaders_dB[SoFdest][_group->onScreenChannels[selected]] + dir);
        } else {
            DSP.setDecibelFaderPosition(
              _group->onScreenChannels[selected],
              DSP.faderPosition_dB[_group->onScreenChannels[selected]] + dir);
            statusbar = 1;
        }
        break;
    }
    case 1: {  // переход между каналами на странице
        turn_started = true;
        selected = constrain(selected + dir, 0, _group->count - 1);
        break;
    }
    case 2:  // переход между страницами каналов
        setGroup(selectedGroup + dir);
        break;
    }
}

void LEDUI::MixerScreen::statusbarDecibels() const {
    int8_t value = usingSoF
                     ? DSP.sendFaders_dB[SoFdest][_group->onScreenChannels[selected]]
                     : DSP.faderPosition_dB[_group->onScreenChannels[selected]];
    bool is_muted = usingSoF
                      ? DSP.sendMuteFlags[SoFdest][_group->onScreenChannels[selected]]
                      : DSP.muteFlags[_group->onScreenChannels[selected]];
    if (value == -97 || is_muted)
        printRightAlign("muted", 0);
    else
        printValue(value, "dB", -1, 0);
}

void LEDUI::MixerScreen::setGroup(int8_t num) {
    selectedGroup = constrain(num, 0, GROUPS_COUNT - 1);
    _group = &groups[selectedGroup];
}