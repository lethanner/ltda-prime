#include "UI.h"

LEDUI::MenuScreen *LEDUI::MenuScreen::active = nullptr;
byte LEDUI::MenuScreen::visibleSel = 0, LEDUI::MenuScreen::entryRendererStart = 0,
     LEDUI::MenuScreen::selected = 0;
byte LEDUI::MenuScreen::_e_count = 0;

void LEDUI::MenuScreen::init(void *params)
{
    MenuScreen::active = this;
    selected = entryRendererStart = visibleSel = 0;
    _e_count = _e_count_static;
    calculateTitleCenter();
}

void LEDUI::MenuScreen::render()
{
    display.clear();
    // заголовок
    display.invertText(1);
    display.rect(0, 0, 127, 7, OLED_FILL);
    display.setCursor(title_xCoord, 0);
    display.print(_entries[0]);

    // сами пункты
    uint8_t currentLine = 1;
    byte endLine = ((_e_count < 7) ? _e_count + 1 : 7) + entryRendererStart;
    for (byte line = entryRendererStart; line < endLine; line++) {
        const char *ptr = _entries[line + 1];

        byte flagId = 0;
        bool isBoolEntry = false;
        if (ptr[0] == '?') {
            flagId = ptr[1] - 65;
            isBoolEntry = true;
            ptr += 2;
        }

        display.setCursor(0, currentLine);
        display.invertText(line == selected);
        display.print(ptr);

        // отображение плюсиков и минусиков возле boolean-пунктов меню
        if (isBoolEntry) {
            display.setCursor(120, currentLine);
            if ((*_booleans >> flagId) & 0x01) display.write('+');
            else display.write('-');
        }
        currentLine++;
    }
    display.invertText(0);
}

/* onClick() реализуется только при наследовании от класса MenuScreen */

void LEDUI::MenuScreen::onHold()
{
    // возврат в микшер
    open(&MixerScreen::it());
}

void LEDUI::MenuScreen::onTurn(int8_t dir)
{
    if (dir > 0) {  // вправо
        if (visibleSel < 6) {
            if (visibleSel < _e_count) visibleSel++;
            else return;
        } else {
            if (entryRendererStart + 6 < _e_count) entryRendererStart++;
            else return;
        }
    } else {  // влево
        if (visibleSel > 0) visibleSel--;
        else {
            if (entryRendererStart > 0) entryRendererStart--;
            else return;
        }
    }

    selected = entryRendererStart + visibleSel;
    if (_autoclick) onClick();
}