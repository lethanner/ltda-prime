#include "UI.h"
#include "screens.h"
#include "lut.h"
#include "../Hardware/ds18b20.h"

bool LEDUI::MixerScreen::init(void* params)
{
    count = 0;
    if (params != NULL) {
        byte paramSoF = *static_cast<byte*>(params);
        usingSoF = true, SoFdest = static_cast<bus>(paramSoF);
        for (byte ch = 0; ch < _group->count; ch++) {
            if (DSP.canBeRoutedTo(_group->onScreenChannels[ch], SoFdest)) count++;
        }
        if (count == 0) return false;
        screen_state = 0;
    } else {
        usingSoF = false;
        count = _group->count;
    }

    // просчёт координат и прочих числовых значений
    // для выравнивания по центру всех каналов на дисплее
    byte gap_m = (128 - (count * 12)) / (count + 1);
    byte gap_s = (128 - (count * 18)) / (count + 1);
    byte label_offset = (DSP.isMonoChannel(_group->onScreenChannels[0]) ? gap_m : gap_s) / 2;
    for (byte ch = 0; ch < count; ch++) {
        bool isMono = DSP.isMonoChannel(_group->onScreenChannels[ch]);
        byte block_width = isMono ? 12 : 18;
        byte gap = isMono ? gap_m : gap_s;
        byte block_safe_zone = block_width + gap;
        _static[0][ch] = gap + (ch * block_safe_zone);
        // определяем максимальную длину всех подписей, которая допустима
        // для влезания на экран в зависимости от кол-ва каналов
        byte label_len = strlen(ch_labels[_group->onScreenChannels[ch]]);
        byte max_label_chars = (block_safe_zone / 6 < label_len) ? block_safe_zone / 6 : label_len;
        _static[1][ch] = max_label_chars;
        _static[2][ch] = label_offset + ((block_safe_zone - (6 * max_label_chars)) / 2);
        label_offset += block_safe_zone;
    }

    return true;
}

void LEDUI::MixerScreen::render()
{
    display.clear();
    if (usingSoF) {  // "sends on fader" вместо статусбара
        statusbarDecibels();
        printYX(ch_labels[_group->onScreenChannels[selected]], 0, 0);
        display.print(" to ");
        display.print(sendto_labels[SoFdest]);
    } else if (statusbar == 0) {  // непосредственно статусбар
        printYX("Mixer", 0, 0);
        printValue(heatsink.getTempInt(), "'C", -1, 0); // инфа о температуре радиатора
    } else if (statusbar == 1) {  // в момент изменения громкости канала
        statusbarDecibels();
        printYX(ch_labels[_group->onScreenChannels[selected]], 0, 0);
        display.print(":");
    }

    for (byte ch = 0; ch < count; ch++) {
        if (usingSoF && !DSP.canBeRoutedTo(_group->onScreenChannels[ch], SoFdest))
            continue;

        byte fader_pos =
         map(usingSoF ? DSP.getFaderPosition(_group->onScreenChannels[ch], SoFdest)
                      : DSP.getFaderPosition(_group->onScreenChannels[ch]),
             -97, 10, 52, 11);
        bool is_muted = usingSoF ? DSP.getMute(_group->onScreenChannels[ch], SoFdest)
                                 : DSP.getMute(_group->onScreenChannels[ch]);

        // clang-format off
        byte levelL = 52 - DSP.getRelativeSignalLevel(LUT::onscreen, 42, _group->onScreenChannels[ch], false);
        display.rect(_static[0][ch] + 8, 52, _static[0][ch] + 11, levelL, OLED_FILL);  // столбик уровня левого канала

        if (!DSP.isMonoChannel(_group->onScreenChannels[ch])) {  // если канал не моно
            byte levelR = 52 - DSP.getRelativeSignalLevel(LUT::onscreen, 42, _group->onScreenChannels[ch], true);
            display.rect(_static[0][ch] + 13, 52, _static[0][ch] + 16, levelR, OLED_FILL);  // столбик уровня правого канала
        }
        // clang-format on

        display.line(_static[0][ch] + 2, 52, _static[0][ch] + 2, 11);  // полоска фейдера
        if (is_muted) {
            // если MUTED, то рисуем (какую-то фигню) вместо ручки фейдера
            display.fastLineH(fader_pos + 1, _static[0][ch] + 1, _static[0][ch] + 3);
            display.fastLineH(fader_pos - 1, _static[0][ch] + 1, _static[0][ch] + 3);
            // если замьюченный канал выбран, то дорисовываем (фигню) до квадратика
            if (selected == ch)
                display.fastLineH(fader_pos, _static[0][ch] + 1, _static[0][ch] + 3);
        } else
            // если не MUTED, то рисуем обычную ручку фейдера
            display.rect(_static[0][ch], fader_pos, _static[0][ch] + 4,
                         fader_pos + static_cast<byte>(selected == ch), 2);

        // подписи каналов с выравниванием
        if (screen_state == 0) {
            display.setCursorXY(_static[2][ch], 56);
            for (byte i = 0; i < _static[1][ch]; i++) {
                display.write(ch_labels[_group->onScreenChannels[ch]][i]);
            }
        }
    }

    // селектор вместо подписей каналов (по необходимости)
    switch (screen_state) {
    case 1:  // выбор канала на дисплее
        if (selected > 0) printYX("<", 56, 0);
        if (selected < count - 1) printYX(">", 56, 122);
        printYX(Localization::act->select, 56);
        break;
    case 2:  // выбор группы каналов
        if (selectedGroup > 0) printYX("<<<", 56, 0);
        if (selectedGroup < GROUPS_COUNT - 1) printYX(">>>", 56, 110);
        printYX(_group->name, 56);
        break;
    }
}

void LEDUI::MixerScreen::onClick()
{
    log_i("MixerScreen clicked: %u, %u", screen_state, statusbar);
    if (screen_state == 1 && (turn_started || usingSoF)) {  // если уже начали листать каналы или если режим "sends on fader"
        screen_state = 0;  // сбрасываем действие
        turn_started = false;
    }
    // если на экране только один канал, переход сразу к переключению страниц
    else if (screen_state == 0 && count == 1)
        screen_state = 2;
    else if (++screen_state > 2)  // а так просто по кругу переключаем действия
        screen_state = 0;

    statusbar = 1;
}

void LEDUI::MixerScreen::onHold()
{
    log_i("MixerScreen hold: %u", screen_state);
    if (screen_state == 1)
        usingSoF  // удержание на выборе канала - MUTE
         ? DSP.toggleMute(_group->onScreenChannels[selected], SoFdest)
         : DSP.toggleMute(_group->onScreenChannels[selected]);
    else if (usingSoF)             // удержание на экране sends on fader
        open(&MixerScreen::it());  // - возврат
    else if (screen_state == 2) {  // удержание на выборе группы каналов - меню группы
        open(&Menus::ChannelGroup::it());
    } else {
        // UPD: а если я могу в будущем менять числа которые скрываются за
        // дефайнами, может ну его нафиг, этот массив скринов? хотя, учитывая,
        // сколько у меня потом будет каналов, как-то стремно.
        switch (_group->onScreenChannels[selected]) {
        case DSPChannels::MASTER: open(&Menus::MasterChannel::it()); break;
        case DSPChannels::REVERB: open(&Menus::ReverbChannel::it()); break;
        case DSPChannels::BLUETOOTH: open(&Menus::BluetoothChannel::it()); break;
        case DSPChannels::PITCH: open(&Menus::PitchChannel::it()); break;
        default:
            if (_group->onScreenChannels[selected] >= DSPChannels::AUX1
                && _group->onScreenChannels[selected] <= DSPChannels::AUX6)
                open(&Menus::AUXChannel::it());
            else open(&Menus::GenericChannel::it());
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
             DSP.getFaderPosition(_group->onScreenChannels[selected], SoFdest) + dir);
        } else {
            DSP.setDecibelFaderPosition(
             _group->onScreenChannels[selected],
             DSP.getFaderPosition(_group->onScreenChannels[selected]) + dir);
            statusbar = 1;
        }
        break;
    }
    case 1: {  // переход между каналами на странице
        turn_started = true;
        selected = constrain(selected + dir, 0, count - 1);
        break;
    }
    case 2:  // переход между страницами каналов
        setGroup(selectedGroup + dir);
        break;
    }
}

void LEDUI::MixerScreen::statusbarDecibels() const
{
    int8_t value = usingSoF ? DSP.getFaderPosition(_group->onScreenChannels[selected], SoFdest)
                            : DSP.getFaderPosition(_group->onScreenChannels[selected]);
    bool is_muted = usingSoF ? DSP.getMute(_group->onScreenChannels[selected], SoFdest)
                             : DSP.getMute(_group->onScreenChannels[selected]);
    if (value == -97 || is_muted) printRightAlign("muted", 0);
    else printValue(value, "dB", -1, 0);
}

void LEDUI::MixerScreen::setGroup(int8_t num)
{
    selectedGroup = constrain(num, 0, GROUPS_COUNT - 1);
    _group = &groups[selectedGroup];
    selected = 0;
    init();
}